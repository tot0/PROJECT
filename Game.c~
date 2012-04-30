/*
 *  Game.c
 *  1917 v1.0
 *  Add to and change this file as you wish to implement the
 *  interface functions in Game.h
 *
 *  Created by Richard Buckland on 20/04/11.
 *  Copyright 2011 Licensed under Creative Commons SA-BY-NC 3.0.  
 *
 */

#include <stdlib.h>
#include <assert.h>

#include "Game.h"



/* **** Functions which change the game aka SETTERS **** */
// make a new game, given the disciplines produced by each
// region, and the value on the dice discs in each region.
// note: each array must be NUM_REGIONS long
// eg if you are using my sample game struct above this function
// would need to set the field currentTurn to -1.  (because the turn
// number is -1 at the start of the game)
Game newGame (int discipline[], int dice[]) {
   Game g;
   
   return g;
}

void disposeGame (Game g) {

}

// make the specified action for the current player and update the
// game state accordingly.
// The function may assume that the action requested is legal.
// START_SPINOFF is not a legal action here
Game makeAction (Game g, action a) {

   return g;
}

// advance the game to the next turn,
// assuming that the dice has just been rolled and produced diceScore
// the game starts in turn -1 (we call this state "Terra Nullis") and
// moves to turn 0 as soon as the first dice is thrown.
Game throwDice (Game g, int diceScore) {

   return g;
}

/* **** Functions which GET data about the game aka GETTERS **** */

// what type of students are produced by the specified region?
// see discipline codes above
int getDiscipline (Game g, int regionID) {

   return discipline;
}

// what dice value produces students in the specified region?
// 2..12
int getDiceValue (Game g, int regionID) {

   return diceValue;
}

// which university currently has the prestige award for the most ARCs?
// this is NO_ONE until the first arc is purchased after the game
// has started.
int getMostARCs (Game g) {

   return university;
}

// which university currently has the prestige award for the most pubs?
// this is NO_ONE until the first IP is patented after the game
// has started.
int getMostPublications (Game g) {

   return university;
}

// return the current turn number of the game -1,0,1, ..
int getTurnNumber (Game g) {
   
   return -1;
}

// return the player id of the player whose turn it is
// the result of this function is NO_ONE during Terra Nullis
int getWhoseTurn (Game g) {

   return playerID;
}

// return the contents of the given vertex (ie campus code or
// VACANT_VERTEX)
int getCampus(Game g, path pathToVertex) {

   return vertexContents;
}

// the contents of the given edge (ie ARC code or vacent ARC)
int getARC(Game g, path pathToEdge) {

   return edgeContents;
}

// returns TRUE if it is legal for the current
// player to make the specified move, FALSE otherwise.
// legal means everything is legal eg when placing a campus consider
// such things as:
//   * is the specified path a valid path?
//   * does it lead to a vacent vertex?
//   * under the rules of the game are they allowed to place a
//     campus at that vertex?  (eg is it adjacent to one of their ARCs?)
//   * does the player have the 4 specific students required to pay for
//     that campus?
// It is not legal to make any action during Terra Nullis ie
// before the game has started.
// It is not legal for a player to make the moves OBTAIN_PUBLICATION
// or OBTAIN_IP_PATENT (they can make the move START_SPINOFF)
int isLegalAction (Game g, action a) {

   return legalOrNot;
}

// --- get data about a specified player ---

// return the number of KPI points the specified player currently has
int getKPIpoints (Game g, int player) {

   return numKPI;
}

// return the number of ARC grants the specified player currently has
int getARCs (Game g, int player) {

   return numARCs;
}

// return the number of GO8 campuses the specified player currently has
int getGO8s (Game g, int player) {

   return numGO8;
}

// return the number of normal Campuses the specified player currently
has
int getCampuses (Game g, int player) {

   return numCampuses;
}

// return the number of IP Patents the specified player currently has
int getIPs (Game g, int player) {

   return numIPs;
}

// return the number of Publications the specified player currently has
int getPublications (Game g, int player) {

   return numPubs;
}

// return the number of students of the specified discipline type
// the specified player currently has
int getStudents (Game g, int player, int discipline) {

   return numStudents;
}

// return how many students of discipline type disciplineFrom
// the specified player would need to retrain in order to get one
// student of discipline type disciplineTo.  This will depend
// on what retraining centers, if any, they have a campus at.
int getExchangeRate (Game g, int player,
                     int disciplineFrom, int disciplineTo) {

   return numStudents;
}

