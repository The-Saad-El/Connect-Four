#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>
#define arbitrarySize 10
#define animateTextDelay_33ms 33


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
void animateText(char strToAnimate[], int timeDelay_ms)
{
    int len = strlen(strToAnimate);
    for (int i = 0; i < len; i++)
    {
        printf("%c", strToAnimate[i]);
        wait(timeDelay_ms);
    }
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
void printConnectFourTitle()
{
    clearScreen();
    printf("==================\n=> \033[1;33mConnect Four\033[0m <=\n==================\n\n\n");
}


void kjmain(int numOfGames, int* gameNum, int* mode)
{
    char userInput[arbitrarySize];
    while (true)            // getting gameNum
    {
        printf("\nEnter your choice [0-%d]: ", numOfGames);
        fgets(userInput, sizeof(userInput), stdin);
        if (userInput[strlen(userInput) - 1] != '\n')           // user entered a string greater than arbitrarySize
        { 
            emptyBuffer(); 
            printf("> [!] Are you trying to crash my program?!");
            continue;
        } 
        else
        {
            userInput[strlen(userInput) - 1] = '\0';                               // removing the newline char
            
            bool isNum = true;
            for (int i = 0; i < strlen(userInput); i++)         // parses through the length of the userInput string & checks if the user entered a non-digit char
            {
                if (userInput[i] < '0' || userInput[i] > '9')              // the userInput contains a non-numeric char
                {
                    printf("> [!] Enter a number!", numOfGames);
                    isNum = false;
                    break;
                }
            }
            
            if (isNum)              // the userInput contains only numbers
            {
                int numUserInput;
                sscanf(userInput, "%d", &numUserInput);             // the string userInput in extracted to numUserInput

                if (numUserInput == 0)
                { 
                    (*gameNum) = 0; 
                    return;
                }                               // user entered 0;  returning to mainMenu()
                else if ((numUserInput < 0) || (numUserInput > numOfGames))            // userInput was not in range [1, count]
                {
                    printf("> [!] Enter a number in range [0, %d]", numOfGames);
                    continue;
                }
                else
                { (*gameNum) = numUserInput; }
            }
        }
    }

    animateText("[Choose]\n   [1] Full Game Reply\n   [2] See GameBoard at End", animateTextDelay_33ms);
    while (true)            // getting mode
    {
        printf("\n\nEnter your choice [0-%d]: ", numOfGames);
        fgets(userInput, sizeof(userInput), stdin);
        if (userInput[strlen(userInput) - 1] != '\n')           // user entered a string greater than arbitrarySize
        { 
            emptyBuffer(); 
            printf("> [!] Are you trying to crash my program?!");
            continue;
        } 
        else
        {
            userInput[strlen(userInput) - 1] = '\0';                               // removing the newline char
            
            bool isNum = true;
            for (int i = 0; i < strlen(userInput); i++)         // parses through the length of the userInput string & checks if the user entered a non-digit char
            {
                if (userInput[i] < '0' || userInput[i] > '9')              // the userInput contains a non-numeric char
                {
                    printf("> [!] Enter a number in range [0, %d]", numOfGames);
                    isNum = false;
                    break;
                }
            }
            
            if (isNum)              // the userInput contains only numbers
            {
                int numUserInput;
                sscanf(userInput, "%d", &numUserInput);             // the string userInput in extracted to numUserInput

                if (numUserInput == 0)
                { 
                    (*gameNum) = 0; 
                    return;
                }                               // user entered 0;  returning to mainMenu()
                else if ((numUserInput < 0) || (numUserInput > numOfGames))            // userInput was not in range [1, count]
                {
                    printf("> [!] Enter a number in range [0, %d]", numOfGames);
                    continue;
                }
                else
                { (*gameNum) = numUserInput; }
            }
        }
    }
}

int main()
{
    int gameNum, mode;
    kjmain(5, &gameNum, &mode);
}