/*

                                        بسم اللہ الرحمان الرحیم   

    CONNECT FOUR
    >    V2: Thani (ثانی)

    Features
        -> AI
        -> Filing
        -> Header files

    [Started]    Sep 8th, 25;  ≈ 22:00
    [Completed]  Sep 19th, 25; ≈ 21:46

                                                الحمد للہ
*/

/*
    TODO
    [1]  resumeGame
    [2]  replayGame
    [3]  gameHistory
    [4]  enable leaderBoards with quickMatch
    [5]  improve the 3 ais
    
    [!]  all done alhamdulillahi taala :)
*/



// headers
#include "play.h"
#include "view.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"

// declarations
programConfig program;      // for whole program-level variables/globals
gameConfig game;            // for individual game-level variables/globals

// main() stuff
int mainMenu()
{
    // printing the connect four title
    clearScreen();      // clears the terminal screen
    if   (!program.mainMenuShown)
    { 
        animateText("==================\n=> \033[1;33mConnect Four\033[0m <=\n==================\n", animateTextDelay_63ms);   // keep at animateTextDelay_63ms
        wait(500);    // wait .5s
    }
    else { printf("==================\n=> \033[1;33mConnect Four\033[0m <=\n==================\n"); }
    program.mainMenuShown = true;

    // the main menu
    animateText("\n\n[Main Menu]\n  [1] Play\n  [2] View\n  [3] Exit", animateTextDelay_33ms);
    char userChoice[arbitrarySize];
    while (true)
    {
        printf("\nEnter your choice [1-3]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || (userChoice[0] == '2') || (userChoice[0] == '3')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break;
        }
        else
        {
            if (userChoice[strlen(userChoice) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] Enter either 1, 2, or 3");
            continue;
        }

    }

    // returning value based on userChoice
    switch(userChoice[0])
    {
        case '1': return 1;
        case '2': return 2;
        case '3': return 3;
    }
}
void play()
{
    /*
        > choose match 
        > validates input
        > clears buffer
        > calls the respective functions: quickMatch(), customMatch()
    */

    printConnectFourTitle();

    // choosing the gameMode
    animateText("[Choose Match]\n  [1] Custom Match\n  [2] Quick Match\n  [3] Go Back", animateTextDelay_33ms);
    char userChoice[arbitrarySize];
    while (true)
    {
        printf("\nEnter your choice [1-3]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || (userChoice[0] == '2') || (userChoice[0] == '3')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break; 
        }
        else
        { 
            if (userChoice[strlen(userChoice) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] Enter either 1, 2, or 3");
            continue;
        }
    }

    // calling gameMode function based on userChoice
    switch(userChoice[0])
    {
        case '1': game.quickMatch = false;  break;      // customMatch()
        case '2': game.quickMatch = true;   break;      // quickMatch()
        case '3':
            // mainMenu();      will result in infinite recursive loop
            //break;
            return;
    }

    printConnectFourTitle();

    // choosing the gameMode
    animateText("[Choose GameMode]\n  [1] PvP: Player vs Player\n  [2] PvAI: Player vs AI\n  [3] AIvAI: AI vs AI\n  [4] Go Back", animateTextDelay_33ms);
    while (true)
    {
        printf("\nEnter your choice [1-4]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || (userChoice[0] == '2') || (userChoice[0] == '3') || (userChoice[0] == '4')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break; 
        }
        else
        { 
            if (userChoice[strlen(userChoice) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] Enter either 1, 2, 3, or 4");
            continue;
        }
    }

    // calling gameMode function based on userChoice
    switch(userChoice[0])
    {
        case '1':
            strcpy(game.gameMode, "PvP");       // for setPlayers() & gamehistory
            PvP();
            break;
        case '2':
            strcpy(game.gameMode, "PvAI");
            PvAI();
            break;
        case '3':
            strcpy(game.gameMode, "AIvAI");
            AIvAI();
            break;
        case '4':
            // mainMenu();      will result in infinite recursive loop
            //break;
            return;
    }

}
void view()
{
    /*
        1> Games' History
        2> leaderBoards
        3> Achievements
        4> Help
        5> Go Back
    */

    printConnectFourTitle();

    // choosing the gameMode
    animateText("[View]\n  [1] Resume\n  [2] History\n  [3] LeaderBoards\n  [4] About\n  [5] Go Back", animateTextDelay_33ms);
    char userChoice[arbitrarySize];
    while (true)
    {
        printf("\nEnter your choice [1-5]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || (userChoice[0] == '2') || (userChoice[0] == '3') || (userChoice[0] == '4') || (userChoice[0] == '5')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");       // (is effectively useless)
            break; 
        }
        else
        { 
            if (userChoice[strlen(userChoice) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] Enter either 1, 2, 3, or 4");
            continue;
        }
    }

    // calling functions based on userChoice
    switch(userChoice[0])
    {
        case '1':  displayUnfinishedGames();  break;     // resume
        case '2':  displayGameHistory();      break;     // history                 // alhamdulillah taala (jo khuwari hui hai ismay...)
        case '3':  displayLeaderBoards();     break;     // leaderBoards
        case '4':  displayAbout();            break;     // about
        case '5':  return;                               // go back [will return to the mainMenu()]
    }
}
void exitGame()
{
    // exiting animation mainly
    printConnectFourTitle();

    for (int i = 5; i >= 1; i--)
    {
        printf("\rExiting in %d", i);
        wait(1000);
    }
    //printf("\rExiting now. <^-^>\n");

    animateText("\r            \rMade by Saad, bi-idhni'Allah Ta'ala  :)", animateTextDelay_33ms);
    wait(3000);
    animateText("\r                                       ", animateTextDelay_33ms);
}



int main()
{
    program.continueProgram = true;
    program.mainMenuShown = false;
    program.randomSeeded = false;

    while (program.continueProgram)
    {
        switch (mainMenu())
        {
            case 1:
                play();
                break;

            case 2:
                view();
                break;

            case 3:
                program.continueProgram = false;
                exitGame();
                break;
        }
    } 

    return 0;
}
