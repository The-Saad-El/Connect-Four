#include "2_game.h"
#include <stdio.h>

void PvP()
{
    initializeGame();
    showGameBoard();

    while (game.playGame)
    {
        getPlayerMove();
        updateGameBoard();
        checkGameBoard();
        // shaykh_Debugger();
        evaluateGameBoard();
    }
}

void PvAI()
{
    printf("PvAI()");
    while (game.playGame)
    {
        break;
    }
}

void AIvAI()
{
    printf("\n AIvAI()");
}