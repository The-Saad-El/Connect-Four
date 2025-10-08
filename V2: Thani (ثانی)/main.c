/*

                                        بسم اللہ الرحمان الرحیم   

    CONNECT FOUR
    >    V2: Thani (ثانی)

    Features
        -> AI
        -> Filing
        -> Header files

    [Started]   Sep 8th, 25; ≈ 22:00
    [Completed]

                                                الحمد للہ
*/

/*
    TODO
        -> 
*/

#include "ai.h"
#include "game.h"
#include <stdio.h>
#include <windows.h>


int main()
{
    while (true)
    {
        mainMenu();
        
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

    return 0;
}
