#ifndef GLOBALS_H
#define GLOBALS_H

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------


// declarations ----------------------------------
#define maxRows 8                       // the max num of rows possible
#define maxCols 9                       // the max num of cols possible
#define emptyChar ' '                   // the char used to represent an empty/unfilled position
#define player1Mark 'X'                 // the symbol/mark/token for player1's spaces
#define player2Mark 'O'                 // the symbol/mark/token for player2's spaces
#define arbitrarySize 35                // arbitrary size (of 35 bytes/char) of string pointers (used mainly in fgets)
#define animateTextDelay_13ms 13        // the arbitrary timeDelay_ms of 33ms used as the argument of the animateText() chiefly in view()'s subfunctions
#define animateTextDelay_33ms 33        // the arbitrary timeDelay_ms of 33ms used as the argument of the animateText() in printing menus
#define animateTextDelay_63ms 63        // the arbitrary timeDelay_ms of 63ms used as the argument of the animateText() for printing AI's dialogues
#define animateGameBoardDelay_ms 300    // the time, in ms, used for the argument of the wait() function, for animating gameBoard updates


// global structs ----------------------------------
#include <time.h>                       // for time_t (used in gameConfig)
#include <stdbool.h>                    // for bool (used in programConfig & gameConfig)
typedef struct
{
    bool continueProgram;                   // flag used for the main while loop in main()
    bool mainMenuShown;                     // flag used to indicate whether the mainMenu() ran once (used to terminate the connectFour animation after it ran once)      
    bool randomSeeded;                      // flag used to indicate whether rand() has already been seeded in the program (using srand())

} programConfig;                        // for whole program-level variables/globals
typedef struct 
{    
    // flags
    bool quickMatch;                        // flag which holds true if the user chose quickMatch & false if not (ie if chose customMatch)
    bool playGame;                          // flag used for the individual game loops
    
    // game setup
    time_t startTime;                       // stores the result of time(NULL) at the start of a game/round (time_t is a typedef for long or long long); is used while saving gameData in file
    time_t endTime;                         // stores the result of time(NULL) at the end of a game/round (time(NULL) gives the elapsed time (in sec) since the unix epoch (ie jan 1st, 1970))
    char gameMode[6];                       // stores the abbreviated name of the current gameMode (PvP/PvAI/AIvAI) (5 max size, 1 '\0')
    int gameState;                          // flag for the current gameBoard state; -1: continue, 0: draw, 1: player1 won, 2: player2 won
    int totalMoves;                         // counter for the total number of moves made
    int activePlayer;                       // 0: game not started, 1: player1, 2: player2 (can be real players or AI)
    int playerMove;                         // holds the move (ie the numOfAvailableColumns position of the chosen column) of the current activePlayer
    int winningIndices[4][2];               // an array containing indices of the winning-row

    // gameBoard setup
    char gameBoard[maxRows][maxCols];       // the gameBoard, initialized to the max possible size
    int rowCount;                           // number of rows of the chosen gameBoard size
    int colCount;                           // number of columns of the chosen gameBoard size

    // player setup         
    char player1Name[arbitrarySize];        // name/title of player1
    char player2Name[arbitrarySize];        // name/title of player2

} gameConfig;                           // for individual game-level variables/globals

extern programConfig program;           // declared here, defined in main.c
extern gameConfig game;


// helper functions ----------------------------------
void clearScreen();
void emptyBuffer();
void wait(int milliseconds);
void pressEnterToContinue();
void animateText(char strToAnimate[], int timeDelay_ms);
void printConnectFourTitle();


// -------------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif