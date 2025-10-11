#ifndef _2_GAME_H
#define _2_GAME_H
//  ---------------------------------------------------------------------------------------------------------------------------


#include <stdbool.h>

#define maxRows 8
#define maxCols 7

typedef struct 
{    
    // game setup
    bool continueProgram;                   // flag used for the main while loop in main()
    bool playGame;                          // flag used for the individual game loops
    int totalMoves;                         // counter for the total number of moves made
    int activePlayer;                       // 0: game not started, 1: player1, 2: player2 (can be real players or AI)
    int playerMove;                         // holds the move (ie the column position) of the current activePlayer
    int gameState;                          // flag for the current gameBoard state; -1: continue, 0: draw, 1: player1 won, 2: player2 won
    int sleepTime;                          // the time, in ms, used for the argument of the Sleep() function, for animating gameBoard updates
    int winningIndices[4][2];                   // an array containing indices of the winning-row

    // gameBoard setup
    char gameBoard[maxRows][maxCols];       // the gameBoard, initialized to the max possible size
    // int maxRows;                         // max number of rows possible of the gameBoard
    // int maxCols;                         // max number of columns possible of the gameBoard
    int rowCount;                           // number of rows of the chosen gameBoard size
    int colCount;                           // number of columns of the chosen gameBoard size
    char emptyChar;                         // the char used to represent an empty/unfilled position

    // player setup         
    char player1Name[25];                   // name/title of player1
    char player1Symbol;                     // the symbol/mark/token for player1's spaces
    char player2Name[25];                   // name/title of player2;
    char player2Symbol;                     // the symbol/mark/token for player2's spaces

    // ai setup
    char aiSymbol;                          // the symbol/mark/token for ai's spaces
    char gameBoardCopy[maxRows][maxCols];   // a replica of the gameBoard; used for simulating turns by AI

} gameConfig;

extern gameConfig game;


void animateText(char strToAnimate[], int timeDelayms);
void clearBuffer();

int mainMenu();
void playGame();
void exitGame();

void setGame();
void setPlayers();
void setGameBoard();
void initializeGame();

void printGameBoard();
void showGameBoard();
void getPlayerMove();
void updateGameBoard();

int checkHorizontally();
int checkVertically();
int checkPosDiagonals();
int checkNegDiagonals();
int checkDraw();
void checkGameBoard();
void shaykh_Debugger();

int lvl2_win_dontLose_playRandom();

void evaluateGameBoard();


//  ---------------------------------------------------------------------------------------------------------------------------
#endif