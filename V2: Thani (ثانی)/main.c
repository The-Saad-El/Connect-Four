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
    [1]  setup filing/files (save/load game, leaderboard, resume game, achievements)
    [2]  seperate AIs & all the related stuff into its own file
    [3]  miniMax (rabi & khamis)
    [4]  break everything into files (main.c, game.c, ai.c, file_io.c, {files}.txt)
    
*/


// headers ------------------------------------------------------------------------------------------------------------------------------------------

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#ifdef _WIN32
    #include <windows.h>    // for windows
#else
    #include <unistd.h>     // for linux & mac
#endif


// globals ------------------------------------------------------------------------------------------------------------------------------------------

// constants
#define maxRows 9                       // the max num of rows possible
#define maxCols 8                       // the max num of cols possible
#define emptyChar ' '                   // the char used to represent an empty/unfilled position
#define player1Mark 'X'                 // the symbol/mark/token for player1's spaces
#define player2Mark 'O'                 // the symbol/mark/token for player2's spaces
#define arbitrarySize 25                // arbitrary size (of 25 bytes/char) of string pointers used in fgets
#define animateTextDelay_33ms 33        // the arbitrary timeDelay_ms of 33ms used as the argument of the animateText() in printing menus
#define animateTextDelay_63ms 63        // the arbitrary timeDelay_ms of 63ms used as the argument of the animateText() for printing AI's dialogues
#define animateTextDelay_123ms 123      // the arbitrary timeDelay_ms of 123ms for some of the animateText() strings such as animating connectFour at start & animating the round result
#define animateGameBoardDelay_ms 300    // the time, in ms, used for the argument of the wait() function, for animating gameBoard updates

// variable struct
typedef struct 
{    
    // flags
    bool continueProgram;                   // flag used for the main while loop in main()
    bool playGame;                          // flag used for the individual game loops
    bool randomSeeded;                      // flag used to indicate whether rand() has already been seeded in the program (using srand())
    bool mainMenuShown;                     // flag used to indicate whether the mainMenu() ran once (used to terminate the connectFour animation after it ran once)      
    
    // game setup
    int totalMoves;                         // counter for the total number of moves made
    int activePlayer;                       // 0: game not started, 1: player1, 2: player2 (can be real players or AI)
    int playerMove;                         // holds the move (ie the numOfAvailableColumns position of the chosen column) of the current activePlayer
    int gameState;                          // flag for the current gameBoard state; -1: continue, 0: draw, 1: player1 won, 2: player2 won
    int winningIndices[4][2];               // an array containing indices of the winning-row

    // gameBoard setup
    char gameBoard[maxRows][maxCols];       // the gameBoard, initialized to the max possible size
    int rowCount;                           // number of rows of the chosen gameBoard size
    int colCount;                           // number of columns of the chosen gameBoard size

    // player setup         
    char player1Name[arbitrarySize];        // name/title of player1
    char player2Name[arbitrarySize];        // name/title of player2;
    int numOfHumanPlayers;                  // the num of Human (not AI) players

} gameConfig;

gameConfig game;


// helpers ------------------------------------------------------------------------------------------------------------------------------------------

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
    for (int i = 0; i < strlen(strToAnimate); i++)
    {
        printf("%c", strToAnimate[i]);
        wait(timeDelay_ms);
    }
}


// game ---------------------------------------------------------------------------------------------------------------------------------------------

// gameModes
void PvP();
void PvAI();
void AIvAI();

// main() stuff
int mainMenu()
{
    // printing the connect four title
    clearScreen();      // clears the terminal screen
    if   (!game.mainMenuShown)
    { 
        animateText("==================\n=> Connect Four <=\n==================\n", animateTextDelay_123ms);   // keep at animateTextDelay_123ms
        wait(500);    // wait .5s
    }
    else { printf("==================\n=> Connect Four <=\n==================\n"); }
    game.mainMenuShown = true;

    // the main menu
    animateText("\n\n[Main Menu]\n  [1] Play\n  [2] Exit", animateTextDelay_33ms);
    char userChoice[arbitrarySize];
    while (true)
    {
        printf("\nEnter your choice [1/2]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || (userChoice[0] == '2')))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break;
        }
        else
        {
            if (userChoice[strlen(userChoice) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
            printf("> [!] Enter either 1 or 2");
            continue;
        }

    }

    // returning value based on userChoice
    switch(userChoice[0])
    {
        case '1': return 1;
        case '2': return 2;
    }
}
void playGame()
{
    /*
        > choose gameMoade
        > validates input
        > clears buffer
        > calls the respective functions: PvP(), PvAI(), or AIvAI()
    */

    clearScreen();
    printf("==================\n=> Connect Four <=\n==================\n\n\n");

    // choosing the gameMode
    animateText("[Choose GameMode]\n  [1] PvP: Player vs Player\n  [2] PvAI: Player vs AI\n  [3] AIvAI: AI vs AI\n  [4] Go Back", animateTextDelay_33ms);
    char userChoice[arbitrarySize];
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
            game.numOfHumanPlayers = 2;
            PvP();
            break;
        case '2':
            game.numOfHumanPlayers = 1;
            PvAI();
            break;
        case '3':
            game.numOfHumanPlayers = 0;
            AIvAI();
            break;
        case '4':
            // mainMenu();      will result in infinite recursive loop
            //break;
            return;
    }
}
void exitGame()
{
    // exiting animation mainly
    clearScreen();
    printf("==================\n=> Connect Four <=\n==================\n");
    printf("\n\n");

    for (int i = 5; i >= 1; i--)
    {
        printf("\rExiting in %d", i);
        wait(1000);
    }
    //printf("\rExiting now. <^-^>\n");

    animateText("\r            \rMade by Saad, bi-idhni'Allah Ta'ala  <^-^>", animateTextDelay_33ms);
    wait(2000);
    animateText("\r                                          ", animateTextDelay_33ms);
}

// initializing game
void setGame()
{
    // game setup 
    game.playGame = true;
    game.totalMoves = 0;
    game.activePlayer = 0;
    game.playerMove = 0;
    game.gameState = -1;
    for (int i = 0; i < 4; i++){ game.winningIndices[i][0] = -1; game.winningIndices[i][1] = -1; }
}
void setPvPPlayers()
{
    // player1 info
    animateText("\n> \033[1;33mPlayer 1\033[0m\n", animateTextDelay_33ms);
    printf("      Enter your name: ");
    fgets(game.player1Name, sizeof(game.player1Name), stdin);
    if   ((strlen(game.player1Name) == 1) || (!strcmp(game.player1Name, "Player 2\n"))){     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
        strcpy(game.player1Name, "Player 1"); 
    }
    else if (game.player1Name[strlen(game.player1Name) - 1] != '\n'){        // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
        emptyBuffer();
        strcpy(game.player1Name, "Player 1"); 
    } 
    else{                   
        game.player1Name[strlen(game.player1Name) - 1] = '\0';        // setting the trailing newline char to the null terminator char
    }   

    // player2 info
    animateText("> \033[1;34mPlayer 2\033[0m\n", animateTextDelay_33ms);
    printf("      Enter your name: ");
    fgets(game.player2Name, sizeof(game.player2Name), stdin);
    if   ((strlen(game.player2Name) == 1) || (!strcmp(game.player2Name, "Player 1\n"))){     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
        strcpy(game.player2Name, "Player 2"); 
    }
    else if (game.player2Name[strlen(game.player2Name) - 1 ] != '\n'){        // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
        emptyBuffer();
        strcpy(game.player2Name, "Player 2"); 
    } 
    else{                   
        game.player2Name[strlen(game.player2Name) - 1] = '\0';        // setting the trailing newline char to the null terminator char
    }   
}
void setPvAIPlayers()
{
    // player info
    animateText("\n> \033[1;33mPlayer 1\033[0m\n", animateTextDelay_33ms);
    printf("      Enter your name: ");
    fgets(game.player1Name, sizeof(game.player1Name), stdin);
    if   ((strlen(game.player1Name) == 1) || (!strcmp(game.player1Name, "Player 2\n"))){     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
        strcpy(game.player1Name, "Player 1"); 
    }
    else if (game.player1Name[strlen(game.player1Name) - 1] != '\n'){        // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
        emptyBuffer();
        strcpy(game.player1Name, "Player 1"); 
    } 
    else{                   
        game.player1Name[strlen(game.player1Name) - 1] = '\0';        // setting the trailing newline char to the null terminator char
    }    

    // ai info
    animateText("> \033[1;34mChoose AI\033[0m\n      [1] Awwal (Easy)\n      [2] Thani (Medium)\n      [3] Thalith (Hard)", animateTextDelay_33ms);
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

    switch (userChoice[0])
    {
        case '1': strcpy(game.player2Name, "Awwal");   break;
        case '2': strcpy(game.player2Name, "Thani");   break;
        case '3': strcpy(game.player2Name, "Thalith"); break;
    }
}
void setAIvAIPlayers()
{
    char userChoice[arbitrarySize];

    // ai1 info
    animateText("\n> \033[1;33mChoose AI 1\033[0m\n      [1] Awwal (Easy)\n      [2] Thani (Medium)\n      [3] Thalith (Hard)", animateTextDelay_33ms);
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

    switch (userChoice[0])
    {
        case '1': strcpy(game.player1Name, "Awwal");   break;
        case '2': strcpy(game.player1Name, "Thani");   break;
        case '3': strcpy(game.player1Name, "Thalith"); break;
    }

    // ai2 info
    animateText("> \033[1;34mChoose AI 2\033[0m\n      [1] Awwal (Easy)\n      [2] Thani (Medium)\n      [3] Thalith (Hard)", animateTextDelay_33ms);
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

    switch (userChoice[0])
    {
        case '1': strcpy(game.player2Name, "Awwal");   break;
        case '2': strcpy(game.player2Name, "Thani");   break;
        case '3': strcpy(game.player2Name, "Thalith"); break;
    }
}
void setPlayers()
{
    // players setup

    clearScreen();
    printf("==================\n=> Connect Four <=\n==================\n\n\n");

    animateText("[Players' Information]", animateTextDelay_33ms);

    switch(game.numOfHumanPlayers)
    {
        case 0:
        {
            // AIvsAI

            setAIvAIPlayers();
            break;
        }

        case 1:
        {
            // PvsAI
            
            setPvAIPlayers();
            break;
        }

        case 2:
        {
            // PvsP

            setPvPPlayers();
            break;
        }
    }

}
void setGameBoard()
{
    // gameBoard setup

    clearScreen();
    printf("==================\n=> Connect Four <=\n==================\n\n\n");
    
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
            printf("> [!] Enter either 1, 2, or 3");
            continue;
        }
    }

    // setting globals to selected size's settings
    switch (userChoice[0])
    {
        case '1':               // Mini (5 x 4)
            game.rowCount = 5;
            game.colCount = 4;
            break;
        case '2':               // Blitz (6 x 5)
            game.rowCount = 6;
            game.colCount = 5;
            break;
        case '3':               // CLassic (7 x 6)
            game.rowCount = 7;
            game.colCount = 6;
            break;
        case '4':               // Grand (8 x 7)
            game.rowCount = 8;
            game.colCount = 7;
            break;
        case '5':               // Titan (9 x 8)
            game.rowCount = 9;
            game.colCount = 8;
            break;    
        case '6':               // go back to setPlayers()
            setPlayers();
            return;             // breaks from the whole function
    }

    // initializing the spaces in the gameBoard to emptyChar
    for (int i = 0; i < game.rowCount; i++)
    {
        for (int j = 0; j < game.colCount; j++){
            game.gameBoard[i][j] = emptyChar;
        }
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
                if   (game.gameBoard[i][j] == player1Mark){ printf("\033[1;4;33;40m%c\033[0m", game.gameBoard[i][j]); }     // bold yellow color with grey highlighting for player1 (winner)
                else                                             { printf("\033[1;4;34;40m%c\033[0m", game.gameBoard[i][j]); }     // bold blue color with grey highlighting for player2 (winner)
            }
            else
            {
                if      (game.gameBoard[i][j] == player1Mark){ printf("\033[1;33m%c\033[0m", game.gameBoard[i][j]); }     // bold yellow color for player1
                else if (game.gameBoard[i][j] == player2Mark){ printf("\033[1;34m%c\033[0m", game.gameBoard[i][j]); }     // bold blue color for player2
                else                                                { printf("%c",                  game.gameBoard[i][j]); }     // no color formatting if empty char
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
void showGameBoard()
{
    // didnt wanna write all this in main so trashed it here :D
    
    clearScreen();
    printf("\nThis is the gameBoard:");
    printGameBoard();
    wait(2000);    // wait 2s
    clearScreen();
    printGameBoard();
}

// gamePlay stuff
void switchActivePlayer()
{
    switch (game.activePlayer)
    {
        case 0: game.activePlayer = 1; break;
        case 1: game.activePlayer = 2; break;
        case 2: game.activePlayer = 1; break;
    }
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

// checking the gameBoard for win, draw, or nothing
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

    return -1;  // no four in a row horizontally
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

    return -1;  // no four in a row vertically
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

    return -1;      // no complete +ve diagonal
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

    return -1;      // no complete -ve m diagonal
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
            if (game.gameBoard[row][col] == emptyChar){ return -1; }
        }
    }
    return 1;
}
void checkGameBoard()
{    
    if (game.totalMoves >= 7)     // the min num of moves required for any player to have won is 7 (ie 4 by the player 1)
    {
        int checkHorizontal = checkHorizontally(), checkVertical = checkVertically(), checkPosDiagonal = checkPosDiagonals(), checkNegDiagonal = checkNegDiagonals(); 
        
        // checking if player1 won
        if ((checkHorizontal == 1) || (checkVertical == 1) || (checkPosDiagonal == 1) || (checkNegDiagonal == 1))
        {
            game.gameState = 1;     // player1 wins
        }
        // checking if player2 won
        else if ((checkHorizontal == 2) || (checkVertical == 2) || (checkPosDiagonal == 2) || (checkNegDiagonal == 2))
        {
            game.gameState = 2;     // player2 wins
        }
        // checking if the game was a draw
        else if (checkDraw() == 1)
        {
            game.gameState = 0;     // game draws
        }   
        // else gamestate remains -1
    }
}

// outcome checking
void evaluateGameBoard()
{
    if (game.gameState != -1)        // continue the round if gameState == -1; will only work for totalMoves >= 7
    {
        wait(330);     // a little pause before printing results
        switch (game.gameState)
        {
            case 0: 
                animateText("\n\n=====================\n[ >< ]  DRAW!  [ >< ]\n=====================\n", animateTextDelay_33ms);
                break;

            case 1: 
            {   
                clearScreen();
                printGameBoard();
                int numBars = strlen("[* * *] WINNER: ") + strlen(game.player1Name) + strlen("! [* * *]");
                char bars[numBars + 1];     // + 1 for '\0'
                for (int i = 0; i < numBars; i++){ bars[i] = '='; }
                bars[numBars] = '\0';
                animateText("\n\n", animateTextDelay_123ms); animateText(bars, animateTextDelay_123ms); animateText("\n[* * *] WINNER: ", animateTextDelay_123ms); animateText(game.player1Name, animateTextDelay_123ms); animateText("! [* * *]\n", animateTextDelay_123ms); animateText(bars, animateTextDelay_123ms); animateText("\n", animateTextDelay_123ms);
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
                animateText("\n\n", animateTextDelay_123ms); animateText(bars, animateTextDelay_123ms); animateText("\n[* * *] WINNER: ", animateTextDelay_123ms); animateText(game.player2Name, animateTextDelay_123ms); animateText("! [* * *]\n", animateTextDelay_123ms); animateText(bars, animateTextDelay_123ms); animateText("\n", animateTextDelay_123ms);
                break;
            }
        }

        game.playGame = false;      // breaks from the inner while loop in main()
    }
}
void finishRound()
{
    wait(1500);    // wait 1.5s
    printf("\n\nPress Enter to continue: ");
    char uselessStr[arbitrarySize];    // size 40 so the user can enter anything, even a faltoo long string, without the program breaking
    fgets(uselessStr, sizeof(uselessStr), stdin);    // used %s instead of %c and getchar() so that the program wont break with any possible input given by the user
    if (uselessStr[strlen(uselessStr) - 1] != '\n'){ emptyBuffer(); }   // empties the buffer if the user entered an input whose length is greater than 25 bytes (max string size)
}


// ai stuff ------------------------------------------------------------------------------------------------------------------------------------------

// global variables;
char gameBoardCopy[maxRows][maxCols];   //  a copy of the game.gameBoard
int availableColumns[maxCols];          //  an array containing the indices of the empty/available/playable columns in gameBoardCopy
int numOfAvailableColumns;              //  counter containing the num of elements in emptyColumnIndices

// helper functions
void makeGameBoardCopy()
{
    for (int row = 0; row < game.rowCount; row++)
    {
        strcpy(gameBoardCopy[row], game.gameBoard[row]);  
    }
}
void findAvailableColumns()
{
    numOfAvailableColumns = 0;
    for (int col = 0; col < game.colCount; col++)
    {
        if (game.gameBoard[0][col] == emptyChar)
        {
            availableColumns[numOfAvailableColumns] = col;
            numOfAvailableColumns++;
        }
    }
}
int positionNotFloating(int row, int col)
{
    if      (row == (game.rowCount - 1))                    { return 1; }       // is the very last row
    else if (gameBoardCopy[row + 1][col] != emptyChar)      { return 1; }       // is not the last row, & the row after it is filled
    else                                                    { return 0; }       // is not the last row, & the row after it is empty
}

// checking functions
int simpleUpdateGameBoard(char tokenToDrop, int columnToUpdate)
{
    for (int row = 0; row < game.rowCount; row++)
    {
        if (gameBoardCopy[row][columnToUpdate] == emptyChar)
        {
            gameBoardCopy[row][columnToUpdate] = tokenToDrop;
            if (row > 0){ gameBoardCopy[row - 1][columnToUpdate] = emptyChar; }
            if (row == (game.rowCount - 1)){ return row; }    // the token reached the last row
        }
        else { return (row - 1); }        // returns the row num at which the token was dropped
    }
}
int simpleCheckHorizontally(char myToken)
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
            if ((gameBoardCopy[row][col]     != emptyChar             ) &&
                (gameBoardCopy[row][col]     == gameBoardCopy[row][col + 1]) && 
                (gameBoardCopy[row][col + 1] == gameBoardCopy[row][col + 2]) && 
                (gameBoardCopy[row][col + 2] == gameBoardCopy[row][col + 3])   )
            { 
                if    (gameBoardCopy[row][col] == myToken) { return 1; }     // I (ai) won
                else                                       { return 2; }     // opponent won
            }
        }
    }

    return -1;  // no four in a row horizontally
}
int simpleCheckVertically(char myToken)
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
            if ((gameBoardCopy[row]    [col] != emptyChar             ) &&
                (gameBoardCopy[row]    [col] == gameBoardCopy[row + 1][col]) &&
                (gameBoardCopy[row + 1][col] == gameBoardCopy[row + 2][col]) &&
                (gameBoardCopy[row + 2][col] == gameBoardCopy[row + 3][col])   )
            { 
                if   (gameBoardCopy[row][col] == myToken) { return 1; }     // I (ai) won
                else                                      { return 2; }     // opponent won
            }
        }
    }

    return -1;  // no four in a row vertically
}
int simpleCheckPosDiagonals(char myToken)
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
            if ((gameBoardCopy[row]    [col]     != emptyChar                 ) &&
                (gameBoardCopy[row]    [col]     == gameBoardCopy[row - 1][col + 1]) && 
                (gameBoardCopy[row - 1][col + 1] == gameBoardCopy[row - 2][col + 2]) && 
                (gameBoardCopy[row - 2][col + 2] == gameBoardCopy[row - 3][col + 3])   )
            {      
                if   (gameBoardCopy[row][col] == myToken) { return 1; }     // I (ai) won
                else                                      { return 2; }     // opponent won
            }    
        }
    }

    return -1;      // no complete +ve diagonal
}
int simpleCheckNegDiagonals(char myToken)
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
            if ((gameBoardCopy[row]    [col]     != emptyChar                 ) &&
                (gameBoardCopy[row]    [col]     == gameBoardCopy[row - 1][col - 1]) && 
                (gameBoardCopy[row - 1][col - 1] == gameBoardCopy[row - 2][col - 2]) && 
                (gameBoardCopy[row - 2][col - 2] == gameBoardCopy[row - 3][col - 3])   )
            {
                if   (gameBoardCopy[row][col] == myToken) { return 1; }     // I (ai) won
                else                                      { return 2; }     // opponent won
            }    
        }
    }

    return -1;      // no complete -ve m diagonal
}
int simpleCheckDraw()
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
            if (gameBoardCopy[row][col] == emptyChar){ return -1; }
        }
    }

    return 1;   // no emptyChar, so draw
}
int simpleCheckGameBoard(char myToken)
{    
    /*
        if (game.totalMoves >= 7)     // the min num of moves required for any player to have won is 7 (ie 4 by the player 1)
        implement the above in the indiviudal simulation AI modules
        make it smthing like this: (game.totalMoves >= (7 - [aiType: 2 simulation, 4 simulation etc]))
    */
    
    if (simpleCheckDraw() == 1)
    { return 0; }            // game draws

    else
    {
        int simpleCheckHorizontal = simpleCheckHorizontally(myToken), simpleCheckVertical = simpleCheckVertically(myToken), simpleCheckPosDiagonal = simpleCheckPosDiagonals(myToken), simpleCheckNegDiagonal = simpleCheckNegDiagonals(myToken);

        if ((simpleCheckHorizontal == 1) || (simpleCheckVertical == 1) || (simpleCheckPosDiagonal == 1) || (simpleCheckNegDiagonal == 1))
        { return 1; }       // player1 wins

        else if ((simpleCheckHorizontal == 2) || (simpleCheckVertical == 2) || (simpleCheckPosDiagonal == 2) || (simpleCheckNegDiagonal == 2))
        { return 2; }       // player2 wins

        else 
        { return -1; }      // game continues as is
    }
}

// ai sub-modules
int checkAIWin(char myToken)
{
    // checking for ai's instant win

    int move;
    findAvailableColumns();

    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard(myToken, move);
        if ((simpleCheckGameBoard(myToken) == 0) || (simpleCheckGameBoard(myToken) == 1)){ return move; }    //  if ai wins or the game draws with the ai's move, return that move
    }

    return -1;      // no instant win
}
int blockOpponentWin(char opponentToken)
{
    // blocking opponent's win

    int move;
    findAvailableColumns();

    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard(opponentToken, move);
        if ((simpleCheckGameBoard(opponentToken) == 1)){ return move; }    //  if the opponent can win in a move, return that move
    }

    return -1;      // no opponent's instant win
}
int blockOpponent3InARow(char opponentToken)
{
    findAvailableColumns();

    int row, col;
    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        col = availableColumns[i];
        row = simpleUpdateGameBoard(opponentToken, col);
        int filled, empty;

        // checking horizontally (rows)
        for (int j = 0; j < (game.colCount - 3); j++)
        {
            filled = 0, empty = 0;
            for (int k = 0; k < 4; k++)
            {
                if      ((gameBoardCopy[row][j + k] == emptyChar) && (positionNotFloating(row, j + k))) { empty++;  }
                else if (gameBoardCopy[row][j + k] == opponentToken)                                    { filled++; }
            }
            if ((empty == 1) && (filled == 3)) { return col; }
        }

        // checking vertically (columns)
        if (row < game.rowCount - 2)
        {
            if ((gameBoardCopy[row + 1][col] == opponentToken) && (gameBoardCopy[row + 2][col] == opponentToken)){ return col; }
        }

        // checking pos diagonals
        for (int j = 0; j < (game.colCount - 3); j++)   // columns: left to right
        {
            for (int k = (game.rowCount - 1); k > 2; k--)   // rows: bottom to top
            {
                filled = 0, empty = 0;
                for (int l = 0; l < 4; l++)     // groups
                {
                    if      ((gameBoardCopy[k - l][j + l] == emptyChar) && (positionNotFloating(k - l, j + l))) { empty++;  }
                    else if (gameBoardCopy[k - l][j + l] == opponentToken)                                      { filled++; }
                }
                if ((empty == 1) && (filled == 3)) { return col; }
            }
        }

        // checking neg diagonals
        for (int j = (game.colCount - 1); j > 2; j--)   // columns: right to left
        {
            for (int k = (game.rowCount - 1); k > 2; k--)   // rows: bottom to top
            {
                filled = 0, empty = 0;
                for (int l = 0; l < 4; l++)     // groups
                {
                    if      ((gameBoardCopy[k - l][j - l] == emptyChar) && (positionNotFloating(k - l, j - l))) { empty++;  }
                    else if (gameBoardCopy[k - l][j - l] == opponentToken)                                      { filled++; }
                }
                if ((empty == 1) && (filled == 3)) { return col; }
            }
        }
    }

    return -1;      // no opponent's 3-in-a-row
}
int playRandomMove()
{
    // playing a random move

    findAvailableColumns();

    int randomIndex = (rand() % numOfAvailableColumns);
    int randomCol = availableColumns[randomIndex];
    return randomCol;
}

// AIs
int lvl1_awwal_1win_2playRandom(char myToken)
{
    /*
        "if i can win now, i will win; else i will play a random position"

        > simulates one move of his across all available columns
        > if he wins by playing any of em or the game draws with his move, returns the numOfAvailableColumns of that column
        > else plays a random available column and return its numOfAvailableColumns
    */

    // checking for instant win
    if (checkAIWin(myToken) != -1){ return checkAIWin(myToken); }

    // playing a random move
    return playRandomMove();
}
int lvl2_thani_1win_2dontLose_3playRandom(char myToken, char opponentToken)
{
    /*
        "if i can win now, i will win; else if the opponent can win, i will block; else i will play a random position"

        > simulates one move of his across all available columns
        > if he wins by playing any of em or the game draws with his move, returns the numOfAvailableColumns of that column
        > else if the opponent can win next turn by playing a move, blocks that
        > else plays a random available column and return its numOfAvailableColumns
    */

    // checking for ai's instant win
    int checkAIWin_result = checkAIWin(myToken);
    if (checkAIWin_result != -1){ return checkAIWin_result; }

    // blocking opponent's win
    int blockOpponentWin_result = blockOpponentWin(opponentToken);
    if (blockOpponentWin_result != -1){ return blockOpponentWin_result; }

    // playing a random move
    return playRandomMove();
}
int lvl3_thalith_1win_2dontLose_3prevent3InARow_4playRandom(char myToken, char opponentToken)  //
{
    /*
        "win; block opponent's win; block opponent's potential 3-in-a-row; else play a random move"

        > if i can win, i will win
        > else if the opponent can win, i will block
        > else if the opponent can make 3 in a row, i will block
        > else i will play a random position 
    */

    // checking for ai's instant win
    int checkAIWin_result = checkAIWin(myToken);
    if (checkAIWin_result != -1){ return checkAIWin_result; }

    // blocking opponent's win
    int blockOpponentWin_result = blockOpponentWin(opponentToken);
    if (blockOpponentWin_result != -1){ return blockOpponentWin_result; }

    // preventing the opponent's potential 3-in-a-row
    int blockOpponent3InARow_result = blockOpponent3InARow(opponentToken);
    if (blockOpponent3InARow_result != -1){ return blockOpponent3InARow_result; }

    // playing a random move
    return playRandomMove();
}

int lvl4_rabi_simulates5Moves();       // 1st ai, 2nd opponent, 3rd ai
int lvl5_khamis_simulates7Moves();     // 1st ai, 2nd opponent, 3rd ai, 4th opponent, 5th ai


// Getting PLayers Moves ---------------------------------------------------------------------------------------------------------------------------- 

void getPlayerMove()
{
    // sets the column number of a player's move to game.playerMove

    if  (game.activePlayer == 1){ printf("\n\n[\033[1;33m%s\033[0m]", game.player1Name); }      // printing the playerName in color
    else                        { printf("\n\n[\033[1;34m%s\033[0m]", game.player2Name); }

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
    if  (game.activePlayer == 1)
    { 
        printf("\n\n[\033[1;33m%s\033[0m]", game.player1Name);      // printing the playerName in color
        if (!strcmp(game.player1Name, "Awwal"))
        {
            game.playerMove = lvl1_awwal_1win_2playRandom(player1Mark);
        }
        else if (!strcmp(game.player1Name, "Thani"))
        {
            game.playerMove = lvl2_thani_1win_2dontLose_3playRandom(player1Mark, player2Mark);
        }
        else if (!strcmp(game.player1Name, "Thalith"))      // else wouldve worked fine but used elseif for expandibility
        {
            game.playerMove = lvl3_thalith_1win_2dontLose_3prevent3InARow_4playRandom(player1Mark, player2Mark);
        }
    }
    else                        
    { 
        printf("\n\n[\033[1;34m%s\033[0m]", game.player2Name); 
        if (!strcmp(game.player2Name, "Awwal"))
        {
            game.playerMove = lvl1_awwal_1win_2playRandom(player2Mark);
        }
        else if (!strcmp(game.player2Name, "Thani"))
        {
            game.playerMove = lvl2_thani_1win_2dontLose_3playRandom(player2Mark, player1Mark);
        }
        else if (!strcmp(game.player2Name, "Thalith"))
        {
            game.playerMove = lvl3_thalith_1win_2dontLose_3prevent3InARow_4playRandom(
        player2Mark, player1Mark);
        }
    }

    char AIDialogues[20][45] = {        // 19 dialogues, 45 arbitrary bytes/char max length per dialogue
                                "I think imma play at umm...", "Hmm my turn huh... ", "Lets go with umm... ", 
                                "I wont let u win that easily :> ", "Oh noice move!\nLemme play... ", "The player is here! :)", 
                                "Am i losing already?! ",  "I found a good one... :) ", "Why do u play so good?? ", 
                                "Lemme think a little... ", "You are smart, i will give you that... ", "Am i losing already?? ",
                                "My mom told me to play at... ", "where do i play? where do i play? ", "i have a good feeling about this :)",
                                "Argh, my turn again... ", "i think i figured it out... :] ", "boi you got some skills! ", 
                                "i see wut you are trying to do there :>", "i guess i haveta play this properly now... "
                                };
    
    printf("\n");
    int randomIndex = (rand() % 19) + 1;
    animateText(AIDialogues[randomIndex], animateTextDelay_63ms);
    wait(2000);

    printf("\n--> Col %d!", game.playerMove + 1);        // game.playerMove contains index (ie columnPosition - 1)
    wait(500);  // wait .5s
}


//  gameModes ---------------------------------------------------------------------------------------------------------------------------------------

void PvP()
{

    setGame();          // initialize globals
    setPlayers();       // choose players
    setGameBoard();     // choose gameBoard size
    showGameBoard();

    while (game.playGame)
    {
        switchActivePlayer();
        getPlayerMove();
        updateGameBoard();
        checkGameBoard();
        evaluateGameBoard();
    }

    finishRound();
}
void PvAI()
{
    setGame();
    setPlayers();
    setGameBoard();
    showGameBoard();
    if (!game.randomSeeded){ srand(time(NULL)); game.randomSeeded = true; }

    while (game.playGame)
    {
        switchActivePlayer();
        if  (game.activePlayer == 1) { getPlayerMove(); }
        else                         { getAIMove();     }
        updateGameBoard();
        checkGameBoard();
        evaluateGameBoard();
    }

    finishRound();
}
void AIvAI()
{
    setGame();
    setPlayers();
    setGameBoard();
    showGameBoard();
    if (!game.randomSeeded){ srand(time(NULL)); game.randomSeeded = true; }

    while (game.playGame)
    {
        switchActivePlayer();
        getAIMove();
        updateGameBoard();
        checkGameBoard();
        evaluateGameBoard();
    }

    finishRound();
}


// ------------------------------------------------------------------------------------------------------------------------------------------------


int main()
{
    game.continueProgram = true;
    game.mainMenuShown = false;
    game.randomSeeded = false;

    while (game.continueProgram)
    {
        switch (mainMenu())
        {
            case 1:
                playGame();
                break;

            case 2:
                game.continueProgram = false;
                exitGame();
                break;
        }
    } 

    return 0;
}
