
#ifndef GAME_H
#define GAME_H


#include <stdbool.h>

typedef struct 
{    
    // game setup
    bool playGame;              // flag used for the main while loop in main()
    int totalMoves;             // counter for the total number of moves made
    int activePlayer;           // 0: game not started, 1: player1, 2: player2
    int playerMove;             // holds the move (ie the column position) of the current activePlayer
    int gameState;              // flag for the current gameBoard state; -1: continue, 0: draw, 1: player1 won, 2: player2 won
    int sleepTime;              // the time, in ms, used for the argument of the Sleep() function, for animating gameBoard updates
    int winningIndices[4][2];   // an array containing indices of the winning-row

    // player setup         
    char player1Name[25];       // name/title of player1
    char player1Symbol;         // the symbol/mark/token for player1's spaces
    char player2Name[25];       // name/title of player2
    char player2Symbol;         // the symbol/mark/token for player2's spaces

    // gameBoard setup
    char gameBoard[8][7];       // the gameBoard, initialized to the max possible size
    int rowCount;               // number of rows of the gameBoard
    int colCount;               // number of columns of the gameBoard
    char emptyChar;             // the char used to represent an empty/unfilled position

} gameConfig;

extern gameConfig game;

void animateText(char strToAnimate[], int timeDelayms);
void mainMenu();

void setGame();
void setPlayers();
void setGameBoard();
void initializeGame();

void printGameBoard();
void showGameBoard();
int getPlayerMove();
void updateGameBoard();

int checkHorizontally();
int checkVertically();
int checkPosDiagonals();
int checkNegDiagonals();
int checkDraw();
void checkGameBoard();
void shaykh_Debugger();

void evaluateGameBoard();


#endif