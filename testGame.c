/****************
 *  testGame.c  *
 *****************************************
 * Tests the interface functions of the  *
 * Game ADT to ensure the model of the   *
 * game state responds appropriately to  *
 * actions and requests.                 *
 *****************************************
 *
 * Group Members:
 * Bruce Huang, David Zhao, James Payor, Lucas Pickup
 *
 * HS1917 - Hornsby Tute (Karla Burnett, Beth Crane)
 *
 *  Created 21/04/12
 *  Version 1.3 - Last Modified 10:00 24/4
[Please increment the sub-version & update the rough time]
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Game.h"

#define BQN STUDENT_BQN
#define MMON STUDENT_MMONEY
#define MJ STUDENT_MJ
#define BPS STUDENT_BPS
#define MTV STUDENT_MTV
#define THD STUDENT_THD

#define NUM_STUDENT_TYPES 6
#define VAL_BQN 0
#define VAL_MMON 1
#define VAL_MJ 2
#define VAL_BPS 3
#define VAL_MTV 4
#define VAL_THD 5

#define DEFAULT_DISCIPLINES {BQN,MMON,MJ,MMON,MJ,BPS,MTV,MTV,BPS,MTV,BQN,MJ,BQN,THD,MJ,MMON,MTV,BQN,BPS}
#define DEFAULT_DICE {9,10,8,12,6,5,3,11,3,11,4,6,4,9,9,2,8,10,5}

#define MIN_DICE_VALUE 2
#define MAX_DICE_VALUE 12

#define TEST_NEW_GAME_CASES 4
#define TEST_GET_TURN_NUMBER_TURNS 150

#define NUM_ACTIONS 8

void testNewGame (void);
void testGetTurnNumber (void);
void testStudentAllocationAndMakeAction (void);
int randomValue (int start, int end);
void testisLegalAction (void);

int main (int argc, const char * argv[]) {

    // Tests creating the new game, the initial turn number,
    // and getDiscipline and getDiceValue for random maps.
    testNewGame();

    // Checks that the turn number is incremented correctly
    // after each dice throw, using throwDice and getTurnNumber.
    testGetTurnNumber();

    // Checks that the game state is updated correctly after each dice
    // throw (e.g. students allocated to universities, conversions to
    // THDs) and that the game state is updated correctly after each
    // action (students removed, game state updated) using a sample
    // sequence of dice throws and actions.
    testStudentAllocationAndMakeAction();
    
    // Tests weather a games isLegalAction function will allow a player to
    // directly obtain IP or Paper. As well as moves during 'terra-nullis'.
    testisLegalAction();

    printf ("All tests passed!\nYou are Awesome\n");
    return EXIT_SUCCESS;
}

int randomValue (int start, int end) {

    int range = end - start;
    int value = rand() / (double)RAND_MAX * range + start;

    return value;

}

void testNewGame (void) {

    printf ("Now testing 'newGame':\n");
    printf ("1) First turn number == -1\n");
    prinft ("2) Regions initialised correctly (displine and dice values correct)\n");

    // check makes a game without crashing
    int disciplines[NUM_REGIONS];
    int dice [NUM_REGIONS];

    int testCase = 0;
    while (testCase < TEST_NEW_GAME_CASES) {

        int regionID = 0;
        while (regionID < NUM_REGIONS) {

            int studentVal = randomValue(0, NUM_STUDENT_TYPES);
            int student = 0;

            if (studentVal == VAL_BQN) {
                student = BQN;
            } else if (studentVal == VAL_MMON) {
                student = MMON;
            } else if (studentVal == VAL_MJ) {
                student = MJ;
            } else if (studentVal == VAL_BPS) {
                student = BPS;
            } else if (studentVal == VAL_MTV) {
                student = MTV;
            } else {
                student = THD;
            }

            disciplines[regionID] = student;
            dice[regionID] = randomValue(MIN_DICE_VAL, MAX_DICE_VAL);

            regionID++;
        }

        Game g = newGame (disciplines, dice);

        assert (getTurnNumber(g) == -1);

        regionID = 0;
        while (regionID < NUM_REGIONS) {

            assert (getDiscipline(g, regionID) == disciplines[regionID]);
            assert (getDiceValue(g, regionID) == dice[regionID]);

            regionID++;
        }

        disposeGame (g);

        testCase++;
    }

    printf ("Passed\n");
}

void testGetTurnNumber (void) {

    printf ("Now testing getTurnNumber...   ");

    // Create game:
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame (disciplines, dice);

    // Check turns:
    int turnNum = -1;
    while (turnNum < TEST_GET_TURN_NUMBER_TURNS) {
        assert (getTurnNumber(g) == turnNum);

        throwDice(g, randomValue(2,12))
        turnNum++;
    }

    disposeGame (g);
    printf ("Passed\n");
}


// NOTE:  IS MAKEACTION(g, PASS_ACTION) required to end turn?  or just THROWDICE?
// Unsure on this, might ahve to ask a tutor.
void testStudentAllocationAndMakeAction (void) {

    printf ("Now testing updating of students after dice throws and actions.\n");

    // Create game:
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame (disciplines, dice);

    // Throw the dice once, not giving any players students, to move the
    // current turn to '0' rather than '-1':
    throwDice(g, 3);

    // Allocate 300 students to each player through numerous dice throws!
    int throws = 0;
    while (throws < 150) {

        // Trigger each hexagon with an adjoined campus:

        throwDice(g, 11); // 1 MTV to player 1
        throwDice(g, 6);  // 1 MJ to player 1

        throwDice(g, 9); // 1 BQN to player 2
        throwDice(g, 9); // 1 BQN to player 2
        throwDice(g, 9); // 1 BQN to player 2
        throwDice(g, 5); // 1 BPS to player 2
        throwDice(g, 5); // 1 BPS to player 2
        throwDice(g, 5); // 1 BPS to player 2

        throwDice(g, 8); // 1 MJ and 1 MTV to player 3
    }

    /* At this point:
     *  - Turn number is [150 * 9 = ] 1350
     *  - Number of turns mod 3 is 0, therefore player 1's (UNI_A) turn to roll.
     *  - Player 1 has 150 MJs and 150 MTVs
     *  - Player 2 has 450 BQNs and 450 BPSs
     *  - Player 3 has 150 MJs and 150 MTVs
    */

    // Ensure game state has been updated correctly:

    printf ("Ensuring 'getTurnNumber' and 'getWhoseTurn' return correct values.\n");
    assert (getTurnNumber(g) == 750);
    assert (getWhoseTurn(g) == UNI_A);

    printf ("Ensuring UNI_A has been allocated the correct number of players.\n");
    assert (getStudents(g, UNI_A, STUDENT_BPS)    == 0);
    assert (getStudents(g, UNI_A, STUDENT_BQN)    == 0);
    assert (getStudents(g, UNI_A, STUDENT_MJ)     == 150);
    assert (getStudents(g, UNI_A, STUDENT_MTV)    == 150);
    assert (getStudents(g, UNI_A, STUDENT_MMONEY) == 0);
    assert (getStudents(g, UNI_A, STUDENT_THD)    == 0);

    printf ("Ensuring UNI_B has been allocated the correct number of players.\n");
    assert (getStudents(g, UNI_B, STUDENT_BPS)    == 450);
    assert (getStudents(g, UNI_B, STUDENT_BQN)    == 450);
    assert (getStudents(g, UNI_B, STUDENT_MJ)     == 0);
    assert (getStudents(g, UNI_B, STUDENT_MTV)    == 0);
    assert (getStudents(g, UNI_B, STUDENT_MMONEY) == 0);
    assert (getStudents(g, UNI_B, STUDENT_THD)    == 0);

    printf ("Ensuring UNI_C has been allocated the correct number of players.\n");
    assert (getStudents(g, UNI_C, STUDENT_BPS)    == 0);
    assert (getStudents(g, UNI_C, STUDENT_BQN)    == 0);
    assert (getStudents(g, UNI_C, STUDENT_MJ)     == 150);
    assert (getStudents(g, UNI_C, STUDENT_MTV)    == 150);
    assert (getStudents(g, UNI_C, STUDENT_MMONEY) == 0);
    assert (getStudents(g, UNI_C, STUDENT_THD)    == 0);



    // Convert UNI_B's BQNs and BPSs into MJs and MTVs, so that each
    // UNI has the same numbers of students:
    printf ("Testing basic student conversion.\n");

    // Move turn to UNI_B from UNI_A (without giving the UNIs students):
    throwDice(g, 3);

    action convertStudents;
    convertStudents.actionCode = RETRAIN_STUDENTS;

    // Convert BPSs into MJs:
    convertStudents.disciplineFrom = STUDENT_BPS;
    convertStudents.disciplineTo = STUDENT_MJ;

    int numConversions = 0;
    while (numConversions < 150) {
        makeAction(g, convertStudents);
    }

    // Ensure the counts have been updated correctly:
    assert (getStudents(g, UNI_B, STUDENT_BPS) == 0);
    assert (getStudents(g, UNI_B, STUDENT_MJ)  == 150);


    // Convert BQNs into MTVs:
    convertStudents.disciplineFrom = STUDENT_BQN;
    convertStudents.disciplineTo = STUDENT_MTV;

    int numConversions = 0;
    while (numConversions < 150) {
        makeAction(g, convertStudents);
    }

    // Ensure the counts have been updated correctly:
    assert (getStudents(g, UNI_B, STUDENT_BQN) == 0);
    assert (getStudents(g, UNI_B, STUDENT_MTV) == 150);


    // Now each UNI has 150 MTVs and 150 MJs.

    // Set turn to UNI_A:
    throwDice(g, 3); // Now UNI_C's turn
    throwDice(g, 3); // Now UNI_A's turn



    // Get each UNI to perform path-based and path-less actions:
    int activeUNI = UNI_A;
    while (activeUNI != NO_ONE) {

        action testAction;

        testAction.actionCode = PASS


        // Set activeUNI to the next UNI
        // Note: incrementing not used because the values of the
        //       constants are not guaranteed.
        throwDice(g, 3);
        if (activeUNI == UNI_A) {
            activeUNI = UNI_B;
        } else if (activeUNI == UNI_B) {
            activeUNI = UNI_C;
        } else {
            activeUNI = NO_ONE;
        }
    }



    disposeGame (g);
    printf ("Passed\n");
}

// Tests weather a games isLegalAction function will allow a player to
// directly obtain IP or Paper. As well as moves during 'terra-nullis'.
void testisLegalAction (void) {
    action testAction;

    // Create game:
    int disciplines[] = DEFAULT_DISCIPLINES;
    int dice[] = DEFAULT_DICE;
    Game g = newGame (disciplines, dice);

    int actionLoop = 0;
    while (actionLoop < NUM_ACTIONS){
        testAction.actionCode = actionLoop;
        assert (isLegalAction(g, testAction.actionCode) == FALSE);
    }

    // Advance from 'terra-nullis' where normal actions are legal.
    throwDice(g, 3);
    throwDice(g, 3);

    // Test OBTAIN_IP_PATENT
    testAction.actionCode = OBTAIN_IP_PATENT;
    assert (isLegalAction(g, testAction.actionCode) == FALSE);

    // Test OBTAIN_PUBLICATION
    testAction.actionCode = OBTAIN_PUBLICATION;
    assert (isLegalAction(g, testAction.actionCode) == FALSE);

    // Test that PASS is allowed
    testAction.actionCode = PASS;
    assert (isLegalAction(g, testAction.actionCode) == TRUE);
}
