// play() ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// headers
#include "ai.h"
#include "view.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include "globals.h"
#include <stdbool.h>
#ifdef _WIN32
    #include <windows.h>    // for windows
#else
    #include <unistd.h>     // for linux & mac
#endif

// declarations
#define minMovesToWin 7
#define noTerminalState -1


// initializing game
void setGame()
{
    // game setup 
    game.totalMoves = 0;
    game.activePlayer = 0;
    game.playerMove = 0;
    game.gameState = -1;
    for (int i = 0; i < 4; i++){ game.winningIndices[i][0] = -1; game.winningIndices[i][1] = -1; }

    // initializing the spaces in the gameBoard to emptyChar
    for (int i = 0; i < maxRows; i++)
    {
        for (int j = 0; j < maxCols; j++){ game.gameBoard[i][j] = emptyChar; }
    }
}

// setting players
void choosePlayer(int promptType)
{
    // promptType --->   0: Choose Player (in PvAI)  |  1: Choose Player 1 (in PvP)  |  2: Choose Player 2 (in PvP)


    // player1Info
    char strToAnimate[15];
    sprintf(strToAnimate, "\n> \033[1;33mPlayer%s\033[0m", (promptType == 0)? "" : " 1");
    animateText(strToAnimate, animateTextDelay_33ms);
    if (promptType == 0 || promptType == 1)
    {
        while (true)
        {
            printf("\n      Enter your name: ");
            fgets(game.player1Name, sizeof(game.player1Name), stdin);
            if      (strlen(game.player1Name) == 1)     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
            {    
                printf("      > [!] Please enter a valid name");
                continue; 
            }
            else if (game.player1Name[strlen(game.player1Name) - 1] != '\n')        // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            {    
                emptyBuffer();
                printf("      > [!] Are you trying to break my program?!");
                continue; 
            }
            else
            {                   
                game.player1Name[strlen(game.player1Name) - 1] = '\0';        // setting the trailing newline char to the null terminator char
                break;
            }
        }
    }
    else            // promptType == 2 (player2 in PvP mode)
    {
        animateText("\n> \033[1;34mPlayer 2\033[0m", animateTextDelay_33ms);            // no use of sprintf

        while (true)
        {
            printf("\n      Enter your name: ");
            fgets(game.player2Name, sizeof(game.player2Name), stdin);
            if      (strlen(game.player2Name) == 1)     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
            {    
                printf("      > [!] Please enter a valid name");
                continue; 
            }
            else if (game.player2Name[strlen(game.player2Name) - 1] != '\n')        // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            {    
                emptyBuffer();
                printf("      > [!] Are you trying to break my program?!");
                continue; 
            }
            else if (!strcmp(game.player1Name, game.player2Name))        // both players' names are the same
            {
                printf("      > [!] The name must be different from Player 1's name");
                continue; 
            }
            else        // all good :)
            {                   
                game.player2Name[strlen(game.player2Name) - 1] = '\0';        // setting the trailing newline char to the null terminator char
                break;
            }
        }
    }
}
void chooseAI(int promptType)
{
    // promptType --->   0: Choose AI  |  1: Choose AI 1  |  2: Choose AI 2 
    
    char strToAnimate[99];
    sprintf(strToAnimate, "\n> \033[1;5%dmChoose AI%s\033[0m\n      [1] Awwal (Easy)\n      [2] Thani (Medium)\n      [3] Thalith (Hard)",
                (promptType == 1)? 33 : 34,                                 // for string color   
                (promptType == 0)? "" : (promptType == 1)? " 1" : " 2"      // for ai, ai 1, or ai 2
            );
    animateText(strToAnimate, animateTextDelay_33ms);   

    char userChoice[arbitrarySize];
    while (true)
    {
        printf("\nEnter your choice [1-3]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || (userChoice[0] == '2') || (userChoice[0] == '3')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            break;
        }
        else
        {
            if (userChoice[strlen(userChoice) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] Enter either 1, 2, or 3");
            continue;
        }
    }

    if (promptType == 1)            // means AI1 in AIvAI mode (which is player 1)
    {
        switch (userChoice[0])
        {
            case '1': strcpy(game.player1Name, "Awwal");   break;
            case '2': strcpy(game.player1Name, "Thani");   break;
            case '3': strcpy(game.player1Name, "Thalith"); break;
        }
    }
    else                            // (promptType == 0 || promptType == 2);   AI is player 2 (either in PvAI or AIvAI)
    {
        switch (userChoice[0])
        {
            case '1': strcpy(game.player2Name, "Awwal");   break;
            case '2': strcpy(game.player2Name, "Thani");   break;
            case '3': strcpy(game.player2Name, "Thalith"); break;
        }
    }
}
void setPvPPlayers()
{
    // player1 info
    choosePlayer(1);

    // player2 info
    choosePlayer(2);
}
void setPvAIPlayers()
{
    // player info
    choosePlayer(0);

    // ai info
    chooseAI(0);
}
void setAIvAIPlayers()
{
    char userChoice[arbitrarySize];

    // ai1 info
    chooseAI(1);

    // ai2 info
    chooseAI(2);

    // if both the ais are the same, suffixes each of their names appropriately to distinguish them
    if (!strcmp(game.player1Name, game.player2Name)) 
    {
        strcat(game.player1Name, "_theFirst");
        strcat(game.player2Name, "_theSecond");
    }
}
void setPlayers()
{
    // players setup

    printConnectFourTitle();
    animateText("[Players' Information]", animateTextDelay_33ms);

    if      (!strcmp(game.gameMode, "PvP"))  { setPvPPlayers();   }     // PvP
    else if (!strcmp(game.gameMode, "PvAI")) { setPvAIPlayers();  }     // PvAI
    else                                     { setAIvAIPlayers(); }     // AIvAI

}

// setting gameBoard
void setGameBoard()
{
    // gameBoard setup

    printConnectFourTitle();
    
    // choosing the gameBoard's size
    animateText("[GameBoard Size]\n  [1] Mini (5 x 4)\n  [2] Blitz (6 x 5)\n  [3] Classic (7 x 6)\n  [4] Grand (8 x 7)\n  [5] Titan (9 x 8)\n  [6] Go Back", animateTextDelay_33ms);
    char userChoice[arbitrarySize];
    while (true)
    {
        printf("\nChoose your gameBoard [1-6]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || (userChoice[0] == '2') || (userChoice[0] == '3') || (userChoice[0] == '4') || (userChoice[0] == '5') || (userChoice[0] == '6')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break;
        }
        else
        {
            if (userChoice[strlen(userChoice) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] Enter either 1, 2, 3, 4, 5, or 6");
            continue;
        }
    }

    // setting globals to user's selected size's settings
    switch (userChoice[0])
    {
        // although (n x m) conventially means n rows and m columns, it is the opposite in connectFour & so refers to n columns and m rows

        case '1':               // Mini (5 x 4)
            game.rowCount = 4;
            game.colCount = 5;
            break;
        case '2':               // Blitz (6 x 5)
            game.rowCount = 5;
            game.colCount = 6;
            break;
        case '3':               // CLassic (7 x 6)
            game.rowCount = 6;
            game.colCount = 7;
            break;
        case '4':               // Grand (8 x 7)
            game.rowCount = 7;
            game.colCount = 8;
            break;
        case '5':               // Titan (9 x 8)
            game.rowCount = 8;
            game.colCount = 9;
            break;    
        case '6':               // go back to setPlayers()
            setPlayers();
            return;             // breaks from the whole function
    }
}

// printing gameBoard
void printGameBoard()
{
    int numOfDashes = (6 * game.colCount) + 1;    // calculating the num of dashes required for rows
    char rowDashes[numOfDashes + 1];
    for (int i = 0; i <  numOfDashes; i++){ rowDashes[i] = '-'; }
    rowDashes[numOfDashes] = '\0';

    printf("\n  %s\n", rowDashes);    // topmost row of dashes
    for (int i = 0; i < game.rowCount; i++)
    {
        for (int j = 0; j < game.colCount; j++)
        {
            bool isWinningIndex = false;
            if (game.gameState == 1 || game.gameState == 2)     // only possible when someone won and [0][0] has a non-(-1) value    
            {
                for (int k = 0; k < 4; k++)
                {
                    if ((game.winningIndices[k][0] == i) && (game.winningIndices[k][1] == j)){   // [k][0] has the i value of the winning position's numOfAvailableColumns & [k][1] the j value
                        isWinningIndex = true;
                        break;
                    }
                }
            }

            printf("  |  ");
            if (isWinningIndex){
                if   (game.gameBoard[i][j] == player1Mark){ printf("\033[1;3;4;33;40m%c\033[0m", game.gameBoard[i][j]); }     // bold, italic yellow color with grey highlighting for player1 (winner)
                else                                      { printf("\033[1;3;4;34;40m%c\033[0m", game.gameBoard[i][j]); }     // bold, italic blue color with grey highlighting for player2 (winner)
            }
            else
            {
                if      (game.gameBoard[i][j] == player1Mark){ printf("\033[1;33m%c\033[0m", game.gameBoard[i][j]); }     // bold yellow color for player1
                else if (game.gameBoard[i][j] == player2Mark){ printf("\033[1;34m%c\033[0m", game.gameBoard[i][j]); }     // bold blue color for player2
                else                                         { printf("%c",                  game.gameBoard[i][j]); }     // no color formatting if empty char
            }
        }
        printf("  |\n  %s\n", rowDashes);   // a row of dashes after every completed row
    }
    
    // for printing column numbers
    printf("  ");    // empty space of width 2 (is fixed)
    for (int i = 0; i < game.colCount; i++)
    {
        printf("  [%d] ", i+1);
    }

    printf("\n");
}

// gamePlay stuff
void startGame()
{
    // didnt wanna write all this in main so trashed it here :D
    
    clearScreen();
    printf("\nThis is the gameBoard:");
    printGameBoard();
    wait(2000);    // wait 2s
    clearScreen();
    printGameBoard();

    game.playGame = true;
    game.startTime = time(NULL);    // starting the stopwatch
}
void switchActivePlayer()
{
    switch (game.activePlayer)
    {
        case 0: game.activePlayer = 1; break;
        case 1: game.activePlayer = 2; break;
        case 2: game.activePlayer = 1; break;
    }
} 
void getPlayerMove()
{
    // sets the column number of a player's move to game.playerMove

    if  (game.activePlayer == 1){ printf("\n\n\n[\033[1;33m%s\033[0m]", game.player1Name); }      // printing the playerName in color
    else                        { printf("\n\n\n[\033[1;34m%s\033[0m]", game.player2Name); }

    char userInput[arbitrarySize];
    int userMove;
    while (true)
    {
        printf("\nEnter your move (Column 1-%d): ", game.colCount);
        fgets(userInput, sizeof(userInput), stdin);
        if ((strlen(userInput) == 2) && ((userInput[0] == '1') || (userInput[0] == '2') || (userInput[0] == '3') || (userInput[0] == '4') || (userInput[0] == '5') || (userInput[0] == '6') || (userInput[0] == '7') || (userInput[0] == '8') || (userInput[0] == '9')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            userMove = userInput[0] - '0';
            if (userMove > game.colCount)
            {
                printf("> [!] That column doesn't exist!");
                continue;
            }
            else if (game.gameBoard[0][userMove - 1] != emptyChar)       // checks if the very top row/position of the chosen column is full or not
            {
                printf("> [!] That column is already filled!");
                continue;
            }
            else
            {
                game.playerMove = userMove - 1;
                break; 
            }
        }
        else
        { 
            if (userInput[strlen(userInput) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] That column doesn't exist!");
            continue;
        }
    }
}
void getAIMove()
{
    if  (game.activePlayer == 1)        // ai is player1
    { 
        printf("\n\n\n[\033[1;33m%s\033[0m]", game.player1Name);      // printing the playerName in color
        // just compares the first 5 digits since, if the ais are the same, their names could end with _first & _second. so just comparing the first 5 digits as they are sure to be the same
        if (!strncmp(game.player1Name, "Awwal", 5))         // lvl1_awwal
        {
            game.playerMove = lvl1_awwal_1win_2playRandom(player1Mark);
        }
        else if (!strncmp(game.player1Name, "Thani", 5))    // lvl2_thani
        {
            game.playerMove = lvl2_thani_1win_2dontLose_3playRandom(player1Mark, player2Mark);
        }
        // else wouldve worked fine but used elseif for expandibility
        else if (!strncmp(game.player1Name, "Thali", 5))    // lvl3_thalith
        {
            game.playerMove = lvl3_thalith_1win_2dontLose_3prevent3InARow_4playRandom(player1Mark, player2Mark);
        }
    }
    else    // game.activePlayer == 2;  ai is player2                     
    { 
        printf("\n\n\n[\033[1;34m%s\033[0m]", game.player2Name); 
        // just compares the first 5 digits since, if the ais are the same, their names could end with _first & _second. so just comparing the first 5 digits as they are sure to be the same
        if (!strncmp(game.player2Name, "Awwal", 5))          // lvl1_awwal
        {
            game.playerMove = lvl1_awwal_1win_2playRandom(player2Mark);
        }
        else if (!strncmp(game.player2Name, "Thani", 5))    // lvl2_thani
        {
            game.playerMove = lvl2_thani_1win_2dontLose_3playRandom(player2Mark, player1Mark);
        }
        // else wouldve worked fine but used elseif for expandibility
        else if (!strncmp(game.player2Name, "Thali", 5))    // lvl3_thalith
        {
            game.playerMove = lvl3_thalith_1win_2dontLose_3prevent3InARow_4playRandom(player2Mark, player1Mark);
        }
    }

    // 47 dialogues, 45 arbitrary bytes/char max length per dialogue
    char AIDialogues[46][45] = 
                                {
                                // Original dialogues (by me :>)
                                "I think imma play at umm...",              
                                "Hmm my turn huh... ", 
                                "Lets go with umm... ",
                                "I wont let u win that easily :> ", 
                                "Oh noice move!\nLemme play... ", 
                                "The player is here! :)",
                                "Am i losing already?! ",  
                                "I found a good one... :) ", 
                                "Why do u play so good?? ",
                                "Lemme think a little... ", 
                                "You are smart, i will give you that... ", 
                                "Am i losing already?? ",
                                "where do i play? where do i play? ", 
                                "i have a good feeling about this :)",
                                "Argh, my turn again... ", 
                                "i think i figured it out... :] ", 
                                "boi you got some skills! ",
                                "i see wut you are trying to do there :>", 
                                "i guess i haveta play this properly now... ",
                                "deep breaths, O AI! U got this! :)",
                                "wut are u even cooking?? ",
                                "me sense a storm is brewing... ",
                                "nooooooo, i was thinkin of playing that! ",
                                "Are u a grandmaster or smthing?! ",
                                "you know i can teach u how to play this :) ",
                                "Even a blind rat plays better than u",
                                "hahaha, wut an amateur! ",
                                "please go easy on me... :( ",
                                "please spare my honour! :(",
                                "i should have been resting now... ",
                                "y do i have to do this on the weekend... ",
                                
                                // New dialogues (generated by claude.ai on 23/10/25)
                                "ooh this is getting interesting... ",       
                                "wait wait, let me recalculate... ",
                                "you almost got me there! ",
                                "hmm not bad, not bad at all... ",
                                "okay okay, i got a plan :D ",
                                "this one looks promising! ",
                                "ur making this harder than i thought ",
                                "aha! i see an opening... ",
                                "brain.exe is loading... ",
                                "gimme a sec to think this through ",
                                "i sense a trap here... :/ ",
                                "alright, lets try this one! ",
                                "fortune favors the bold right? ",
                                "my superhuman senses are tingling... ",
                                "calculated move incoming... maybe "
                                };
    
    printf("\n");
    int randomIndex = (rand() % 46);    // index 0 till 45
    animateText(AIDialogues[randomIndex], animateTextDelay_63ms);
    wait(2000);

    printf("\n--> Col %d!", game.playerMove + 1);        // game.playerMove contains index (ie columnPosition - 1)
    wait(500);  // wait .5s
}
void updateGameBoard()
{
    game.totalMoves++;
    char playerMark = ((game.activePlayer == 1)? player1Mark : player2Mark);

    for (int row = 0; row < game.rowCount; row++)
    {
        clearScreen();
        printGameBoard();
        if (game.gameBoard[row][game.playerMove] == emptyChar)
        {
            if (row > 0){ game.gameBoard[row - 1][game.playerMove] = emptyChar; }      // sets the previous/above position back to emptyChar
            game.gameBoard[row][game.playerMove] = playerMark;      // sets current position/space to playerMark
            wait(animateGameBoardDelay_ms - (row * 20));      // accelerating :)
        }
        else { break; }
    }

    clearScreen();
    printGameBoard();
}

// checking the gameBoard for win, draw, or nothing & then updating game.gameState accordingly
int checkHorizontally()
{
    /*
        checking horizontally (rows)

        -1: continue game (no complete, horizontal row)
        1 : player 1 won (has a complete, horizontal row)
        2 : player 2 won (has a complete, horizontal row)
    */

    for (int row = 0; row < game.rowCount; row++)
    {
        for (int col = 0; col < (game.colCount - 3); col++)
        {
            if ((game.gameBoard[row][col]     != emptyChar              ) &&
                (game.gameBoard[row][col]     == game.gameBoard[row][col + 1]) && 
                (game.gameBoard[row][col + 1] == game.gameBoard[row][col + 2]) && 
                (game.gameBoard[row][col + 2] == game.gameBoard[row][col + 3])   )
            { 
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row);         // row indices remain constant
                    game.winningIndices[i][1] = (col + i);     // col indices increment by 1 from 0
                }
                if    (game.gameBoard[row][col] == player1Mark) { return 1; }     // player 1 won
                else                                                   { return 2; }     // player 2 won
            }
        }
    }

    return noTerminalState;  // no four in a row horizontally
}
int checkVertically()
{
    /*
        checking vertically (rows)

        -1: continue game (no complete, vertical row)
        1 : player 1 won (has a complete, vertical row)
        2 : player 2 won (has a complete, vertical row)
    */

    for (int col = 0; col < game.colCount; col++)     // left to right
    {
        for (int row = 0; row < (game.rowCount - 3); row++)   // top to bottom
        {
            // printf("\n[row(j) = %d][col(i) = %d] game.gameBoard[j][i]: %c game.gameBoard[j+1][i]: %c game.gameBoard[j + 2][i]: %c game.gameBoard[j + 3][i]: %c\n", i, j, game.gameBoard[j][i], game.gameBoard[j + 1][i], game.gameBoard[j + 2][i], game.gameBoard[j + 3][i]);
            if ((game.gameBoard[row][col]     != emptyChar              ) &&
                (game.gameBoard[row]    [col] == game.gameBoard[row + 1][col]) &&
                (game.gameBoard[row + 1][col] == game.gameBoard[row + 2][col]) &&
                (game.gameBoard[row + 2][col] == game.gameBoard[row + 3][col])   )
            { 
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row + i);     // row indices increment by 1 from 0
                    game.winningIndices[i][1] = (col);         // col indixes remain constant
                }
                if   (game.gameBoard[row][col] == player1Mark) { return 1; }     // player 1 won
                else                                                  { return 2; }     // player 2 won
            }
        }
    }

    return noTerminalState;  // no four in a row vertically
}
int checkPosDiagonals()
{
    /*
        checking the +ve gradient diagonals

        -1: continue game (no complete, +ve diagonal)
        1 : player 1 won (has a complete, +ve diagonal)
        2 : player 2 won (has a complete, +ve diagonal)
    */

    for (int Row = game.rowCount; Row >= 4; Row--)     // reverse loop (from the bottom row to the topmost)
    {
        for (int Col = 1; Col <= (game.colCount - 3); Col++)      // left to right
        {
            int row = (Row - 1), col = (Col - 1);
            if ((game.gameBoard[row][col]         != emptyChar                  ) &&
                (game.gameBoard[row]    [col]     == game.gameBoard[row - 1][col + 1]) && 
                (game.gameBoard[row - 1][col + 1] == game.gameBoard[row - 2][col + 2]) && 
                (game.gameBoard[row - 2][col + 2] == game.gameBoard[row - 3][col + 3])   )
            {
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row - i);     // row indices decrement by 1
                    game.winningIndices[i][1] = (col + i);     // col indices increment by 1
                }        
                if   (game.gameBoard[row][col] == player1Mark) { return 1; }     // player 1 won
                else                                                  { return 2; }     // player 2 won
            }    
        }
    }

    return noTerminalState;      // no complete +ve diagonal
}
int checkNegDiagonals()
{
    /*
        checking the -ve gradient diagonals

        -1: continue game (no complete, -ve diagonal)
        1 : player 1 won (has a complete, -ve diagonal)
        2 : player 2 won (has a complete, -ve diagonal)
    */

    for (int Row = game.rowCount; Row >= 4; Row--)     // reverse loop (from the bottom row to the topmost)
    {
        for (int Col = game.colCount; Col >= 4; Col--)     // from right to left
        {
            int row = (Row - 1), col = (Col - 1);
            if ((game.gameBoard[row][col]         != emptyChar                  ) &&
                (game.gameBoard[row]    [col]     == game.gameBoard[row - 1][col - 1]) && 
                (game.gameBoard[row - 1][col - 1] == game.gameBoard[row - 2][col - 2]) && 
                (game.gameBoard[row - 2][col - 2] == game.gameBoard[row - 3][col - 3])   )
            {
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row - i);     // row indices decrement by 1
                    game.winningIndices[i][1] = (col - i);     // col indices decrement by 1
                }
                if   (game.gameBoard[row][col] == player1Mark) { return 1; }     // player 1 won
                else                                                  { return 2; }     // player 2 won
            }    
        }
    }

    return noTerminalState;      // no complete -ve m diagonal
}
int checkDraw()
{
    /*
        returns 
            > -1: if no draw (ie empty space left)
            >  1: if draw (ie no more empty space left)
    */

    for (int row = 0; row < game.rowCount; row++)
    {
        for (int col = 0; col < game.colCount; col++)
        {
            if (game.gameBoard[row][col] == emptyChar){ return noTerminalState; }
        }
    }
    return 1;
}
void updateGameState()
{    
    if (game.totalMoves >= minMovesToWin)     // the min num of moves required for any player to have won is 7 (ie 4 by the player 1)
    {
        int checkHorizontal = checkHorizontally(), checkVertical = checkVertically(), checkPosDiagonal = checkPosDiagonals(), checkNegDiagonal = checkNegDiagonals(); 
        
        // checking if player1 won
        if ((checkHorizontal == 1) || (checkVertical == 1) || (checkPosDiagonal == 1) || (checkNegDiagonal == 1))
        {
            game.gameState = 1;     // player1 wins
            return;
        }
        // checking if player2 won
        else if ((checkHorizontal == 2) || (checkVertical == 2) || (checkPosDiagonal == 2) || (checkNegDiagonal == 2))
        {
            game.gameState = 2;     // player2 wins
            return;
        }
        // checking if the game was a draw
        else if (checkDraw() == 1)
        {
            game.gameState = 0;     // game draws
            return;
        }   
        // else gamestate remains -1
    }
}

// evaluating/analyzing the value of game.gameState for a terminal state
void evaluateGameBoard()
{
    if (game.gameState != noTerminalState)        // continue the round if gameState == -1; will only work for totalMoves >= 7
    {
        game.endTime = time(NULL);      // stoppin the "stopwatch" when the game reaches a terminal state

        wait(330);     // a little pause before printing results
        switch (game.gameState)
        {
            case 0: 
                animateText("\n\n=====================\n[ >< ]  DRAW!  [ >< ]\n=====================\n", animateTextDelay_33ms);
                break;

            case 1:     // couldve used sprintf
            {   
                clearScreen();
                printGameBoard();
                int numBars = strlen("[* * *] WINNER: ") + strlen(game.player1Name) + strlen("! [* * *]");
                char bars[numBars + 1];     // + 1 for '\0'
                for (int i = 0; i < numBars; i++){ bars[i] = '='; }
                bars[numBars] = '\0';
                animateText("\n\n", animateTextDelay_63ms); animateText(bars, animateTextDelay_63ms); animateText("\n[* * *] WINNER: \033[1;33m", animateTextDelay_63ms); animateText(game.player1Name, animateTextDelay_63ms); animateText("\033[0m! [* * *]\n", animateTextDelay_63ms); animateText(bars, animateTextDelay_63ms); animateText("\n", animateTextDelay_63ms);
                break;
            }

            case 2: 
            {   
                clearScreen();
                printGameBoard();
                int numBars = strlen("[* * *] WINNER: ") + strlen(game.player2Name) + strlen("! [* * *]");
                char bars[numBars + 1];     // + 1 for '\0'
                for (int i = 0; i < numBars; i++){ bars[i] = '='; }
                bars[numBars] = '\0';
                animateText("\n\n", animateTextDelay_63ms); animateText(bars, animateTextDelay_63ms); animateText("\n[* * *] WINNER: \033[1;34m", animateTextDelay_63ms); animateText(game.player2Name, animateTextDelay_63ms); animateText("\033[0m! [* * *]\n", animateTextDelay_63ms); animateText(bars, animateTextDelay_63ms); animateText("\n", animateTextDelay_63ms);
                break;
            }
        }

        game.playGame = false;          // breaks from the inner while loop in main()
    }
}


// gameModes
void PvP()
{

    setGame();          // initialize globals
    if (game.quickMatch)
    {
        // strcpy(game.player1Name, "Player 1");            coulve done this & made this a proper quickMatch but it wont be leaderBoards applicable
        choosePlayer(1);
        // strcpy(game.player2Name, "Player 2");            isliye went with this approach. revert later if ya want
        choosePlayer(2);
        game.rowCount = 6;        // CLassic (7 x 6)
        game.colCount = 7;
    }
    else
    {    
        setPlayers();       // choose players
        setGameBoard();     // choose gameBoard size
    }
    startGame();

    while (game.playGame)
    {
        switchActivePlayer();
        getPlayerMove();
        updateGameBoard();
        updateGameState();
        evaluateGameBoard();
        saveGameBoardDetails();
    }

    saveGameDetails();
    updateLeaderBoards();                    // leaderBoards will only be updated if the mode is not AIvAI (is PvP or PvAI)
    pressEnterToContinue();
}
void PvAI()                                                                             // player vs AI mode
{
    setGame();                                                                          // initializing game. globals
    if (game.quickMatch)                                                                // quickMatch() mode
    {
        choosePlayer(0);                                                                // initially did this:  strcpy(game.player1Name, "Player 1");
        strcpy(game.player2Name, "Thalith");                                            // defaulting ai with thalith
        game.rowCount = 6;                                                              // CLassic (7 x 6) gameBoard
        game.colCount = 7;
    }
    else                                                                                // customMatch() mode
    {    
        setPlayers();                                                                   // choose players
        setGameBoard();                                                                 // choose gameBoard size
    }
    if (!program.randomSeeded){ srand(time(NULL)); program.randomSeeded = true; }       // seeding rand() with time(NULL) once per program
    startGame();                                                                        // printing the gameBoard & a little pause before starting game

    while (game.playGame)                                                               // infinite gameLoop until a terminal state is reached
    {
        switchActivePlayer();                                                           // switches game.activePlayer 
        if  (game.activePlayer == 1) { getPlayerMove(); }                               // gets move based on game.activePlayer 
        else                         { getAIMove();     }                               // getting AI's move when game.activePlayer is even (2 + 2n)
        updateGameBoard();                                                              // updates the gameBoard row by row & calls printGameBoard in between them
        updateGameState();                                                              // updates game.gameState based upon the 5 checking functions
        evaluateGameBoard();                                                            // evaluating game based upon game.gameState
        saveGameBoardDetails();                                                         // updates the gameBoardHistory.txt file with the current playerMove
    }

    saveGameDetails();
    updateLeaderBoards();                                                               // leaderBoards will only be updated if the mode is not AIvAI (is PvP or PvAI)
    pressEnterToContinue();                                                             // waits for user input before clearing the screen & reprinting mainMenu
}
void AIvAI()
{
    setGame();
    if (game.quickMatch)
    {
        strcpy(game.player1Name, "Thalith_theFirst");
        strcpy(game.player2Name, "Thalith_theSecond");
        game.rowCount = 6;        // CLassic (7 x 6)
        game.colCount = 7;
    }
    else
    {    
        setPlayers();       // choose players
        setGameBoard();     // choose gameBoard size
    }
    if (!program.randomSeeded){ srand(time(NULL)); program.randomSeeded = true; }
    startGame();

    while (game.playGame)
    {
        switchActivePlayer();
        getAIMove();
        updateGameBoard();
        updateGameState();
        evaluateGameBoard();
        saveGameBoardDetails();
    }

    saveGameDetails();
    pressEnterToContinue();
}
