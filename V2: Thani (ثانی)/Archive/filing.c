#include <stdio.h>

int main()
{
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n|| %4s | %-25s | %-25s | %-25s | %8s | %8s | %10s | %8s | %-29s ||\n-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n", "Game", "dateTime", "player1Name", "player2Name", "gameMode", "gameBoard", "totalMoves", "Duration", "Result");
    /*
    FILE *fPtr = fopen("test.txt", "w");
    if (fPtr == NULL){ printf("[!] Error opening file!"); }
    else
    {
        fputs("<----------------->\n [1] Ya Allah!\n [2] Ya Rahman!\n [3] Ya Raheem!\n [4] Ya Malik!\n [5] Ya Quddoos!\n [6] Ya Salam!\n [7] Ya Azeez!\n",fPtr);
        char line[123];
        while (fgets(line, sizeof(line), fPtr) != NULL)
        {
            printf("%s", line);
        }        
        fclose(fPtr);
    }
    */
    /*
    FILE *filePtr = fopen("test.txt", "r");
    if (filePtr == NULL){ printf("[!] Error opening file!"); }
    else
    {
        char line[123];
        while (fgets(line, sizeof(line), filePtr) != NULL)
        {
            printf("%s", line);
        }        
        fclose(filePtr);
    }
    */

    return 0;
}