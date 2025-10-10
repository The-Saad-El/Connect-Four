#include "2_game.h"
#include <time.h>       // time()
#include <stdio.h>
#include <stdlib.h>     // rand, srand
#include <stdbool.h>

/*
    AI.h Declarations
    ->  int lvl1_firstAvailable();
    ->  int lvl2_randomPosition();
    ->  int lvl3_checksWin();       // iterates through each column and calls the checkGameBoard() to check for win
    ->  int lvl4_checksLoss();
    ->  int lvl5_simulates2Turns();
    ->  int lvl6_simulates3Turns();
*/

typedef struct
{
    int emptyColumnIndices[7];
    int index;
    int count;
    int simulateTurn[]

} ai;

int lvl1_firstAvailable()
{
    for (int i = 0; i < game.colCount; i++)
    {
        if (game.gameBoard[0][i] == game.emptyChar){ return i; }
    }
}
int lvl2_randomPosition()
{
    int emptyColumnIndex[game.colCount];
    int index = 0;

    for (int i = 0; i < game.colCount; i++)
    {
        if (game.gameBoard[0][i] == game.emptyChar)
        {
            emptyColumnIndex[index] = i;
            index++;
        }
    }

    srand(time(NULL));
    int randomIndex = (rand() % index);
    return emptyColumnIndex[randomIndex];
}



void simpleUpdateGameBoard(char gameBoardToUpdate[][7], int columnToUpdate)
{
    for (int i = 0; i < game.rowCount; i++)
    {
        if (gameBoardToUpdate[i][columnToUpdate] == game.emptyChar){
            gameBoardToUpdate[i][columnToUpdate] = game.aiSymbol;
            if (i > 0){ gameBoardToUpdate[i-1][columnToUpdate] = game.emptyChar; }
        }
        else{ break; }
    }
}
int simpleCheckHorizontally(char gameBoardToCheck[][]);
int simpleCheckVertically(char gameBoardToCheck[][]);
int simpleCheckPosDiagonals(char gameBoardToCheck[][]);
int simpleCheckNegDiagonals(char gameBoardToCheck[][]);
int simpleCheckGameBoard();

int lvl3_simulates2Moves()   // iterates through each column and calls the checkGameBoard() to check for win
{
    int emptyColumnIndices[game.colCount][3];   // 1st for index, 2nd for outcome if played move at that index/column 3rd for 2nd outcome
    int index = 0;

    for (int i = 0; i < game.colCount; i++)
    {
        if (game.gameBoard[0][i] == game.emptyChar)
        {
            emptyColumnIndices[index][0] = i;
            index++;
        }
    }

    char gameBoardCopy[8][7];   // size of .gameBoard
    for (int i = 0; i < index; i++)
    {
        strcpy(gameBoardCopy, game.gameBoard);
        int indexToUpdate = emptyColumnIndices[i][0];
        simpleUpdateGameBoard(gameBoardCopy, indexToUpdate);
        int result = simpleCheckGameBoard();
        emptyColumnIndices[i][1] = result;
    }

}

int lvl4_simulates3Moves();
int lvl5_simulates4Moves();
