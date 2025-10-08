#include "game.h"
#include <time.h>   // time()
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
int lvl3_checksWin();   // iterates through each column and calls the checkGameBoard() to check for win

int lvl4_checksLoss();
int lvl5_simulates2Turns();
int lvl6_simulates3Turns();
