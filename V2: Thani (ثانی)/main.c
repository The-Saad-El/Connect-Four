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


#include "PvP.h"
#include "game.h"
#include "PvAI.h"
#include "AIvAI.h"

#include <stdio.h>
#include <windows.h>
#include <stdbool.h>


int main()
{
    while (true)
    {
        system("cls");      // clears the terminal screen

        animateText("==================\n=> Connect Four <=\n==================", 123);   // keep at 123
        Sleep(500);    // wait .5s

        printf("\n\n\n[Main Menu]\n  [1] Play\n  [2] Exit");
        int userChoice;
        while (true)
        {
            printf("\nEnter your choice [1/2]: ");
            scanf("%d", &userChoice);

            switch (userChoice)
            {
                case 1:
                    printf("\n\n\n[Choose GameMode]\n  [1] PvP: Player vs Player\n  [2] PvAI: Player vs AI\n  [3] AIvAI: AI vs AI");
                    printf("\nEnter your choice [1-3]: ");
                    scanf("%d", &userChoice);   
                    switch(userChoice)
                    {
                        case 1:
                            PvP();
                            break;
                        case 2:
                            PvAI();
                            break;
                        case 3:
                            AIvAI();
                            break;
                        default:
                            printf("> [!] Enter either 1, 2, or 3");
                            continue;
                    }
                    break;

                case 2:
                    printf("\n");
                    for (int i = 5; i >= 1; i--)
                    {
                        printf("\rExiting in %d", i);
                        Sleep(1000);
                    }
                    printf("\rExiting now. <^-^>");
                    return 0;

                default:
                    printf("> [!] Enter either 1 or 2");
                    continue;
            }
        }
    }

    return 0;
}
