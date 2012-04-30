/***************
 *  runGame.c  *
 *****************************************
 * Uses the Game ADT to allow a user to  *
 * run a simulation of the 'Geographic   *
 * Discipline Model', the actions of VCs *
 * received as text input.               *
 *****************************************
 *
 * Group Members:
 * Bruce Huang, David Zhao, James Payor, Lucas Pickup
 *
 * HS1917 - Hornsby Tute (Karla Burnett, Beth Crane)
 *
 *  Created 21/04/12
 *  Version 1.6 - Last modified at 12 AM on 28/4
[Please increment the sub-version & update the rough time]
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "Game.h"

#define THD STUDENT_THD
#define BPS STUDENT_BPS
#define BQN STUDENT_BQN
#define MJ STUDENT_MJ
#define MTV STUDENT_MTV
#define MMON STUDENT_MMONEY

#define BOARD_DEFAULT_DISCIPLINES {BQN, MMON, MJ, \
                                   MMON, MJ, BPS, MTV, \
                                   MTV, BPS, MTV, BQN, MJ, \
                                   BQN, THD, MJ, MMON, \
                                   MTV, BQN, BPS}

#define BOARD_DEFAULT_DICE_NOS    {9, 10, 8, \
                                   12, 6, 5, 3, \
                                   11, 3, 11, 4, 6, \
                                   4, 9, 9, 2, \
                                   8, 10, 5}

#define TRUE 1
#define FALSE 0

// Used to rescale random numbers to a number between 0 and 1: <- Much better
#define RAND_SCALE ((double)1.0 / (double)RAND_MAX)

#define DICE_START 1
#define DICE_NUMBERS 6

#define BOARD_DICE_START 2
#define BOARD_DICE_END 12
#define BOARD_DICE_RANGE (BOARD_DICE_END - BOARD_DICE_START + 1)

#define WINNING_KPI_SCORE 150

#define BOARDROWS 11
#define BOARDCOLS 21
//max no of arcs on map
#define MAXNOARCS 100


typedef struct _arc {
  int row;
  int col;
  int sign;
  path pth;
} arc;

typedef struct _place {
  int row;
  int col;
} place;

 typedef char board[BOARDROWS][BOARDCOLS];

typedef struct _diceDiscipline {
   int disciplineAt[NUM_REGIONS];
   int diceAt[NUM_REGIONS];
} diceDiscipline;



int getDiceScore (void);
action getPlayerAction (void);
int isValidPath (char * pathStr);
diceDiscipline generateRandomGame (void);

void displayGameState (Game gameState);
void displayInvalidActionMessage (void);
void displayEndGameInformation (void);
double scaledRand (void);

void printGame (Game g);
static void fillBoard(Game g, board b);
static int visit(Game g, arc e, board b);
static int start(int row);
static int end(int row);
static int valid(arc e);
static arc right(arc e);
static arc left(arc e);
char campusToChar(int code);


int main (int argc, char *argv[]) {

    time_t seconds;
    seconds = time(NULL); // will return seconds since January 1, 1970
    // Seed the random number generator:
    srand(seconds);


    Game gameState;
    diceDiscipline board;


    printf ("Would you like to use a random board?  (y/n)\n>> ");
    char randomOrNot;
    assert(scanf("%s", randomOrNot) == 1);


    if (randomOrNot == 'y') {
        board = generateRandomGame();
    } else {

        // Cannot assign a set of values in curly braces to an already initialised array.
        // Deep copy of values used instead.

        int disciplines[] = BOARD_DEFAULT_DISCIPLINES;
        int diceNumbers[] = BOARD_DEFAULT_DICE_NOS;

        int region = 0;
        while (region < NUM_REGIONS) {
            board.disciplineAt[region] = disciplines[region];
            board.diceAt[region] = diceNumbers[region];
            region++;
        }

    }

    gameState = newGame (board.disciplineAt, board.diceAt);



    int gameRunning = TRUE;

    while (gameRunning) {

        int diceScore = getDiceScore();
        gameState = throwDice (gameState, diceScore);

        printf("\n\n\n\nA score of %d was thrown.\n\n", diceScore);
        displayGameState (gameState);


        action playerAction = getPlayerAction();

        while (playerAction.actionCode != PASS) {

            if ( isLegalAction(gameState, playerAction) ) {

                if (playerAction.actionCode == START_SPINOFF) {
                    if ( (int)(scaledRand() * 3) == 0 ) {
                        // 33% chance of obtaining IP
                        playerAction.actionCode == OBTAIN_IP_PATENT;
                    } else {
                        playerAction.actionCode == OBTAIN_PUBLICATION;
                    }
                }
                gameState = makeAction (gameState, playerAction);
            } else {
                displayInvalidActionMessage();
            }


            playerAction = getPlayerAction();

        }


        int currentPlayer = getWhoseTurn (gameState);
        if (getKPIpoints(gameState, currentPlayer) >= WINNING_KPI_SCORE) {
            gameRunning = FALSE;
        }

    }

    displayEndGameInformation();



    return EXIT_SUCCESS;

}

// Returns a random number between 0 and 1:
double scaledRand (void) {
    double result = rand() * RAND_SCALE;
    return result;
}

// Very primitive will need to add a few if statements to make sure
// there aren't too many of each discipline or dice number.
diceDiscipline generateRandomGame (void) {
    diceDiscipline randomGame;
    int region = 0;

    while (region < NUM_REGIONS) {
        randomGame.disciplineAt[region] = scaledRand() * NUM_DISCIPLINES;

        randomGame.diceAt[region] = scaledRand() * BOARD_DICE_RANGE + BOARD_DICE_START;

        region++;
    }

    return randomGame;
}

// Returns a score between
int getDiceScore (void) {

    // Two random events must be simulated here (as they are independent):
    int diceScore = scaledRand() * DICE_NUMBERS + DICE_START +
                    scaledRand() * DICE_NUMBERS + DICE_START;

    return diceScore;
}

action getPlayerAction (void) {
    action playerAction;

    printf("Please select an action:\n"
       "PASS = 0\n"
       "BUILD_CAMPUS = 1\n"
       "BUILD_GO8 = 2\n"
       "OBTAIN_ARC = 3\n"
       "START_SPINOFF = 4\n"
       "RETRAIN_STUDENTS = 7\n"
       ">> "
    );

    scanf("What action would you like to take? %d\n", &playerAction.actionCode);

    if (playerAction.actionCode == BUILD_CAMPUS ||
        playerAction.actionCode == BUILD_GO8 ||
        playerAction.actionCode == OBTAIN_ARC) {

        int validPathEntered = FALSE;
        while (!validPathEntered) {

            printf ("Please enter a path beginning at the top-left vertex to clarify your action:\n>> ");
            scanf ("%s", playerAction.destination);

            validPathEntered = isValidPath(playerAction.destination);
        }
    }

    if (playerAction.actionCode == RETRAIN_STUDENTS) {
        printf ("Please enter the student types involved in the format \"FROM, TO\" (e.g. 1, 3):\n"
                "BPS = 1\n"
                "BQN = 2\n"
                "MJ  = 3\n"
                "MTV = 4\n"
                "M$  = 5\n"
                ">> ");
        scanf("%d, %d", &playerAction.disciplineFrom, &playerAction.disciplineTo);
    }

    return playerAction;
}


// Checks if a path contains legal characters.
int isValidPath (char * pathStr) {

    int i = 0;
    int len = strlen(pathStr);
    int validPath = TRUE;

    while ((i < len) && validPath) {

        if (pathStr[i] != 'L' &&
            pathStr[i] != 'R' &&
            pathStr[i] != 'B') {

            validPath = FALSE;
        }

        i++;
    }

    return validPath;
}


// Displays the current game state.
void displayGameState (Game gameState) {

    // Display player info:

    printf ("             |---------|---------|---------|");
    printf ("             |  Uni A  |  Uni B  |  Uni C  |");
    printf ("|------------|---------|---------|---------|");

    printf ("| Campuses   |   %3d   |   %3d   |   %3d   |",
            getCampuses(gameState, UNI_A), getCampuses(gameState, UNI_B), getCampuses(gameState, UNI_C));
    printf ("| GO8s       |   %3d   |   %3d   |   %3d   |",
            getGO8s(gameState, UNI_A), getGO8s(gameState, UNI_B), getGO8s(gameState, UNI_C));
    printf ("| ARC Grants |   %3d   |   %3d   |   %3d   |",
            getARCs(gameState, UNI_A), getARCs(gameState, UNI_B), getARCs(gameState, UNI_C));

    printf ("|------------|---------|---------|---------|");
    printf ("| THDs       |   %3d   |   %3d   |   %3d   |",
            getStudents(gameState, UNI_A, STUDENT_THD), getStudents(gameState, UNI_B, STUDENT_THD), getStudents(gameState, UNI_C, STUDENT_THD));
    printf ("| BPSs       |   %3d   |   %3d   |   %3d   |",
            getStudents(gameState, UNI_A, STUDENT_BPS), getStudents(gameState, UNI_B, STUDENT_BPS), getStudents(gameState, UNI_C, STUDENT_BPS));
    printf ("| BQNs       |   %3d   |   %3d   |   %3d   |",
            getStudents(gameState, UNI_A, STUDENT_BQN), getStudents(gameState, UNI_B, STUDENT_BQN), getStudents(gameState, UNI_C, STUDENT_BQN));

    printf ("|------------|---------|---------|---------|");
    printf ("| MJs        |   %3d   |   %3d   |   %3d   |",
            getStudents(gameState, UNI_A, STUDENT_MJ), getStudents(gameState, UNI_B, STUDENT_MJ), getStudents(gameState, UNI_C, STUDENT_MJ));
    printf ("| MTVs       |   %3d   |   %3d   |   %3d   |",
            getStudents(gameState, UNI_A, STUDENT_MTV), getStudents(gameState, UNI_B, STUDENT_MTV), getStudents(gameState, UNI_C, STUDENT_MTV));
    printf ("| MMONEYS    |   %3d   |   %3d   |   %3d   |",
            getStudents(gameState, UNI_A, STUDENT_MMONEY), getStudents(gameState, UNI_B, STUDENT_MMONEY), getStudents(gameState, UNI_C, STUDENT_MMONEY));
    printf ("|------------|---------|---------|---------|");

    printf ("\n\n");

    // Display the board:
    printGame(gameState);

    // Display blank lines:
    printf("\n\n");

}

void displayInvalidActionMessage (void) {

    // Warn the user that the action they entered was invalid:
    printf ("Invalid action.  Please make another decision.\n");

}

void displayEndGameInformation (void) {

    // Display the game state and inform the player of the win.

}





//print an ASCII representation of the game
void printGame(Game g) {
  char * dicekey1 = "          111";
  char * dicekey2 = " 123456789012";
  char * disciplinekey = "BRCYGM";
  place tilepos[] = {{1,6},{3,4},{5,2},
			{1,10},{3,8},{5,6},{7,4},
			{1,14},{3,12},{5,10},{7,8},{9,6},
			       {3,16},{5,14},{7,12},{9,10},
			{5,18},{7,16},{9,14}};
  int i,j;
  int dice;
  int regionID;
  board b;
  place p;

  fillBoard(g,b);
  for (regionID=0; regionID < NUM_REGIONS; regionID++) {
    p = tilepos[regionID];
    dice = getDiceValue(g, regionID);
    b[p.row][p.col-1] = dicekey1[dice];
    b[p.row][p.col] = dicekey2[dice];
    b[p.row][p.col+1] = disciplinekey[getDiscipline(g, regionID)];
  }
  putchar('\n');
  for (i=0; i < BOARDROWS; i++){
    for (j=0; j < BOARDCOLS; j++){
      putchar(b[i][j]);
    }
    putchar('\n');
  }


}

//breadth first search to find a path to each arc on board
static void fillBoard(Game g, board b){
  arc queue[MAXNOARCS];
  int i,j;
  int head = 0;//of queue
  int tail = 1;//of queue
  arc e = {-1,14,1,""}; //start of all paths
  arc next;

  for (i=0; i < BOARDROWS; i++){
    for (j=0; j < BOARDCOLS; j++){
      b[i][j] =' ';
    }
  }
  //special case of campus at end of empty path
  //which is at position(0,14) on our board
  b[0][14] = campusToChar(getCampus(g, ""));

  queue[0] = e;
  while (head < tail){
    e = queue[head];
    //printf("%d %d %d %s\n",head , e.row, e.col, e.pth);
    head++;
    next = right(e);
    if (visit(g, next, b)) {
      queue[tail] = next;
      tail++;
    }
    next = left(e);
    if (visit(g, next, b)) {
      queue[tail] = next;
      tail++;
    }
  }
}

//visit an arc, filling in the board with a character
// such as a, b, c, -, + for the arc and the adjacent vertex
// returns TRUE if we update the board
// rturn FALSE if we don't have to do anything because we've
// already visited this spot
static int visit(Game g, arc e, board b){

  int answer;
  int arcContents;
  int campus;
  char arcChar; //character representation
  answer = FALSE;
  if(valid(e) && b[e.row][e.col] == ' '){
    answer = TRUE;
    arcContents = getARC(g, e.pth);
    campus = getCampus(g, e.pth);
    if (e.row % 2 == 0) {
      arcChar = '-';
      b[e.row][e.col+e.sign] = campusToChar(campus);
    } else {
      arcChar = '|';
      b[e.row+e.sign][e.col] = campusToChar(campus);
    }
    if (arcContents != VACANT_VERTEX) {
      arcChar = campusToChar(arcContents);
    }
    b[e.row][e.col] = arcChar;

  }
  return answer;
}


//start position of board on a row
static int start(int row) {
  return 2*(abs(row - BOARDROWS/2)/2);
}

//end position of board on a row
static int end(int row) {
  return BOARDCOLS - start(row) - 1;
}

// is this arc on the map or has it gone into the ocean?
static int valid(arc e){
  return 0 <= e.row && e.row < BOARDROWS &&
    start(e.row) <= e.col && e.col <= end(e.row);
}

//where you get if you go right from an arc
static arc right(arc e){
  arc answer;
  strcpy(answer.pth,e.pth);
  strcat(answer.pth,"R");
  if(e.row%2 != 0){
    answer.row = e.row + e.sign;
    answer.col = e.col - e.sign;
    answer.sign = - e.sign;
  } else if((e.row+e.col)%4 == 1) {
    answer.row = e.row;
    answer.col = e.col + 2*e.sign;
    answer.sign = e.sign;
  } else {
    answer.row = e.row + e.sign;
    answer.col = e.col + e.sign;
    answer.sign = e.sign;

  }
  return answer;
}

//where you get if you go left from an arc
static arc left(arc e){
  arc answer;
  strcpy(answer.pth,e.pth);
  strcat(answer.pth,"L");
  if(e.row%2 != 0){
    answer.row = e.row + e.sign;
    answer.col = e.col + e.sign;
    answer.sign = e.sign;
  } else if((e.row+e.col)%4 == 1) {
    answer.row = e.row - e.sign;
    answer.col = e.col + e.sign;
    answer.sign = - e.sign;
  } else {
    answer.row = e.row;
    answer.col = e.col + 2*e.sign;
    answer.sign = e.sign;

  }
  return answer;
}


//turn a campus code into a character for printing
// '+' is an empty vertex, a,b,c are campuses A,B,C are GO8 campuses
char campusToChar(int code){
  char * key = "+abcABC";
  return key[code];
}
