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

#include "2_game.h"

// gcc 1_main.c 2_game.c 4_modes.c -o connectFour

int main()
{
    do {
        switch (mainMenu())
        {
            case 1:
                playGame();
                break;

            case 2:
                exitGame();
                break;
        }
    } while (game.continueProgram);

    return 0;
}
