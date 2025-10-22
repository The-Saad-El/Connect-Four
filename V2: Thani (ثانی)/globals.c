
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

// helpers ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void emptyBuffer()
{
    int count = 0;
    int tempChar;
    while (count < 250)     // arbitrary max count limit
    {
        tempChar = getchar();
        if ((tempChar == '\n') || (tempChar == EOF))   // continue while tempChar is not the newline char or the eof is not reach
        {    
            break;
        }
        else
        {
            // do nothing, just consume characters
            count++;
            continue;
        }
    }
}
void wait(int milliseconds)
{
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);    // takes microseconds as argument
    #endif
}
void pressEnterToContinue()
{
    wait(1000);                                                         // wait 1s
    printf("\n\n\n");
    
    animateText("Press Enter to continue: ", animateTextDelay_33ms);
    char uselessStr[arbitrarySize];                                     // size 25 so the user can enter anything, even a faltoo long string, without the program breaking
    fgets(uselessStr, sizeof(uselessStr), stdin);                       // used %s instead of %c and getchar() so that the program wont break with any possible input given by the user
    if (uselessStr[strlen(uselessStr) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
}
void animateText(char strToAnimate[], int timeDelay_ms)
{
    int len = strlen(strToAnimate);
    for (int i = 0; i < len; i++)
    {
        printf("%c", strToAnimate[i]);
        wait(timeDelay_ms);
    }
}
void printConnectFourTitle()
{
    clearScreen();
    printf("==================\n=> \033[1;33mConnect Four\033[0m <=\n==================\n\n\n");
}
