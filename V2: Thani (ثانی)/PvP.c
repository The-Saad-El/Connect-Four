#include "game.h"

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