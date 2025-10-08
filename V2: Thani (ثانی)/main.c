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

#include "game.h"
#include <stdio.h>
#include <windows.h>


int main()
{
    while (true)
    {
        mainMenu();

        initializeGame();
        printf("\nThis is the gameBoard:");
        printGameBoard();
        Sleep(2000);    // wait 2s
        system("cls");
        printGameBoard();

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
