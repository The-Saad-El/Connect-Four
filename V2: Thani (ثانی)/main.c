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

#define maxRows 8
#define maxCols 7

typedef struct 
{    
    // game setup
    bool continueProgram;                   // flag used for the main while loop in main()
    bool playGame;                          // flag used for the individual game loops
    int totalMoves;                         // counter for the total number of moves made
    int activePlayer;                       // 0: game not started, 1: player1, 2: player2 (can be real players or AI)
    int playerMove;                         // holds the move (ie the numOfAvailableColumns position of the chosen column) of the current activePlayer
    int gameState;                          // flag for the current gameBoard state; -1: continue, 0: draw, 1: player1 won, 2: player2 won
    int sleepTime;                          // the time, in ms, used for the argument of the wait() function, for animating gameBoard updates
    int winningIndices[4][2];               // an array containing indices of the winning-row

    // gameBoard setup
    char gameBoard[maxRows][maxCols];       // the gameBoard, initialized to the max possible size
    int rowCount;                           // number of rows of the chosen gameBoard size
    int colCount;                           // number of columns of the chosen gameBoard size
    char emptyChar;                         // the char used to represent an empty/unfilled position

    // player setup         
    char player1Name[25];                   // name/title of player1
    char player1Symbol;                     // the symbol/mark/token for player1's spaces
    char player2Name[25];                   // name/title of player2;
    char player2Symbol;                     // the symbol/mark/token for player2's spaces

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
    int tempChar;
    while (true)
    {
        tempChar = getchar();
        if ((tempChar == '\n') || (tempChar == EOF)){   // continue while tempChar is not the newline char or the eof is not reach
            break;
        }
        else{
            // do nothing, just consume characters
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
void animateText(char strToAnimate[], int timeDelayms)
{
    for (int i = 0; i < strlen(strToAnimate); i++)
    {
        printf("%c", strToAnimate[i]);
        wait(timeDelayms);
    }
}


// game ---------------------------------------------------------------------------------------------------------------------------------------------

void PvP();
void PvAI();
void AIvAI();

// main() stuff
int mainMenu()
{
    // printing the connect four title
    clearScreen();      // clears the terminal screen
    animateText("==================\n=> Connect Four <=\n==================", 123);   // keep at 123
    wait(500);    // wait .5s

    // the main menu
    printf("\n\n\n[Main Menu]\n  [1] Play\n  [2] Exit");
    char userChoice[25];
    while (true)
    {
        emptyBuffer();      // will be relevant if there is a 2nd repetition
        printf("\nEnter your choice [1/2]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && (userChoice[1] == '1') || (userChoice[1] == '2'))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break;
        }
        else
        {
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

    // choosing the gameMode
    printf("\n\n[Choose GameMode]\n  [1] PvP: Player vs Player\n  [2] PvAI: Player vs AI\n  [3] AIvAI: AI vs AI");
    char userChoice[25];
    while (true)
    {
        emptyBuffer();
        printf("\nEnter your choice [1-3]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && (userChoice[1] == '1') || (userChoice[1] == '2') || (userChoice[1] == '3'))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break; 
        }
        else
        { 
            printf("> [!] Enter either 1, 2, or 3");
            continue;
        }
    }

    // calling gameMode function based on userChoice
    switch(userChoice[0])
    {
        case '1':
            PvP();
            break;
        case '2':
            PvAI();
            break;
        case '3':
            AIvAI();
            break;
    }
}
void exitGame()
{
    // exiting animation mainly

    printf("\n\n");
    for (int i = 5; i >= 1; i--)
    {
        printf("\rExiting in %d", i);
        wait(1000);
    }
    printf("\rExiting now. <^-^>");
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
    game.player1Symbol = 'X';       // hard coding these symbols in this version
    game.player2Symbol = 'O';
    game.emptyChar = ' ';
    game.sleepTime = 300;
}
void setPlayers(int numOfHumanPlayers)
{
    // players setup
    printf("\n\n[Players' Information]");

    switch(numOfHumanPlayers)
    {
        case 0:
        {
            // AIvsAI

            char userChoice[25];

            // ai1 info
            printf("\n> \033[1;33mChoose AI 1\033[0m\n      [1] Awwal (Easy)\n      [2] Thani (Medium)\n      [3] Thalith (Hard)\n");
            while (true)
            {
                emptyBuffer();      // will be relevant if there is a 2nd repetition
                printf("\nEnter your choice [1-3]: ");
                fgets(userChoice, sizeof(userChoice), stdin);
                if ((strlen(userChoice) == 2) && (userChoice[1] == '1') || (userChoice[1] == '2') || (userChoice[1] == '3'))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
                {
                    printf("> Accepted\n\n");
                    break;
                }
                else
                {
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

            // ai2 info
            printf("\n> \033[1;34mChoose AI 2\033[0m\n      [1] Awwal (Easy)\n      [2] Thani (Medium)\n      [3] Thalith (Hard)\n");
            while (true)
            {
                emptyBuffer();      // will be relevant if there is a 2nd repetition
                printf("\nEnter your choice [1-3]: ");
                fgets(userChoice, sizeof(userChoice), stdin);
                if ((strlen(userChoice) == 2) && (userChoice[1] == '1') || (userChoice[1] == '2') || (userChoice[1] == '3'))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
                {
                    printf("> Accepted\n\n");
                    break;
                }
                else
                {
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

            break;
        }

        case 1:
        {
            // PvsAI
            
            // player info
            emptyBuffer();
            printf("\n> \033[1;33mPlayer 1\033[0m\n");
            printf("      Enter your name: ");
            fgets(game.player1Name, sizeof(game.player1Name), stdin);
            if   ((strlen(game.player1Name) == 1) || !(strcmp(game.player1Name, "Player 2\n"))) { strcpy(game.player1Name, "Player 1"); }     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
            else                                                                                { game.player1Name[strlen(game.player1Name) - 1] = '\0';}      // setting the trailing newline char to the null terminator char

            // ai info
            printf("\n> \033[1;34mChoose AI\033[0m\n      [1] Awwal (Easy)\n      [2] Thani (Medium)\n      [3] Thalith (Hard)\n");
            char userChoice[25];
            while (true)
            {
                emptyBuffer();      // will be relevant if there is a 2nd repetition
                printf("\nEnter your choice [1-3]: ");
                fgets(userChoice, sizeof(userChoice), stdin);
                if ((strlen(userChoice) == 2) && (userChoice[1] == '1') || (userChoice[1] == '2') || (userChoice[1] == '3'))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
                {
                    printf("> Accepted\n\n");
                    break;
                }
                else
                {
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

            break;
        }

        case 2:
        {
            // PvsP

            // player1 info
            emptyBuffer();
            printf("\n> \033[1;33mPlayer 1\033[0m\n");
            printf("      Enter your name: ");
            fgets(game.player1Name, sizeof(game.player1Name), stdin);
            if   ((strlen(game.player1Name) == 1) || !(strcmp(game.player1Name, "Player 2\n"))) { strcpy(game.player1Name, "Player 1"); }     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
            else                                                                                { game.player1Name[strlen(game.player1Name) - 1] = '\0';}      // setting the trailing newline char to the null terminator char

            // player2 info
            emptyBuffer();
            printf("> \033[1;34mPlayer 2\033[0m\n");
            printf("      Enter your name: ");
            fgets(game.player2Name, sizeof(game.player2Name), stdin);
            if   ((strlen(game.player2Name) == 1) || !(strcmp(game.player2Name, "Player 1\n"))) { strcpy(game.player2Name, "Player 2"); }        // sets player2Name to {Player 2} if length is 1, or the user entered {Player 1}
            else
            { 
                game.player2Name[strlen(game.player2Name) - 1] = '\0';
                if (!(strcmp(game.player1Name, game.player2Name))){ strcpy(game.player2Name, "Player 2"); }     // if player1name is same as player2name so sets player2name as "Player 2"
            }

            break;
        }
    }

}
void setGameBoard()
{
    // gameBoard setup
    
    // choosing the gameBoard's size
    printf("\n\n[GameBoard Size]\n  1) Blitz: 6 x 5\n  2) Classic: 7 x 6\n  3) Grand: 8 x 7");      // Mini, Titan
    char userChoice[25];
    while (true)
    {
        emptyBuffer();
        printf("\nChoose your gameBoard [1-3]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && (userChoice[1] == '1') || (userChoice[1] == '2') || (userChoice[1] == '3'))        // if userChoice has only 2 characters (1: userInput 2nd: '\n') & the first char is a valid choice
        {
            printf("> Accepted\n\n");
            break;
        }
        else
        {
            printf("> [!] Enter either 1, 2, or 3");
            continue;
        }
    }

    // setting globals to selected size's settings
    switch (userChoice[1])
    {
        case '1':   // 6 x 5
            game.rowCount = 6;
            game.colCount = 5;
            break;
        case '2':   // 7 x 6
            game.rowCount = 7;
            game.colCount = 6;
            break;
        case '3':   // 8 x 7
            game.rowCount = 8;
            game.colCount = 7;
            break;
    }

    // initializing the spaces in the gameBoard to emptyChar
    for (int i = 0; i < game.rowCount; i++)
    {
        for (int j = 0; j < game.colCount; j++){
            game.gameBoard[i][j] = game.emptyChar;
        }
    }
}

// printing gameBoard
void printGameBoard()
{
    int numOfDashes = (6 * game.colCount);    // calculating the num of dashes required for rows
    char rowDashes[numOfDashes + 1];
    for (int i = 0; i <=  numOfDashes; i++){ rowDashes[i] = '-'; }
    rowDashes[numOfDashes + 1] = '\0';

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
                if   (game.gameBoard[i][j] == game.player1Symbol){ printf("\033[1;4;33;40m%c\033[0m", game.gameBoard[i][j]); }     // bold yellow color with grey highlighting for player1 (winner)
                else                                             { printf("\033[1;4;34;40m%c\033[0m", game.gameBoard[i][j]); }     // bold blue color with grey highlighting for player2 (winner)
            }
            else
            {
                if      (game.gameBoard[i][j] == game.player1Symbol){ printf("\033[1;33m%c\033[0m", game.gameBoard[i][j]); }     // bold yellow color for player1
                else if (game.gameBoard[i][j] == game.player2Symbol){ printf("\033[1;34m%c\033[0m", game.gameBoard[i][j]); }     // bold blue color for player2
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
    char playerMark = ((game.activePlayer == 1)? game.player1Symbol : game.player2Symbol);

    for (int row = 0; row <= game.rowCount; row++)    // why <= and not just < ???
    {
        clearScreen();
        printGameBoard();
        if (game.gameBoard[row][game.playerMove] == game.emptyChar)
        {
            if (row > 0){ game.gameBoard[row - 1][game.playerMove] = game.emptyChar; }
            game.gameBoard[row][game.playerMove] = playerMark;
            wait(game.sleepTime);
        }
        else { break; }
    }
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
            if ((game.gameBoard[row][col]     != game.emptyChar              ) &&
                (game.gameBoard[row][col]     == game.gameBoard[row][col + 1]) && 
                (game.gameBoard[row][col + 1] == game.gameBoard[row][col + 2]) && 
                (game.gameBoard[row][col + 2] == game.gameBoard[row][col + 3])   )
            { 
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row);         // row indices remain constant
                    game.winningIndices[i][1] = (col + i);     // col indices increment by 1 from 0
                }
                if    (game.gameBoard[row][col] == game.player1Symbol) { return 1; }     // player 1 won
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
            if ((game.gameBoard[row][col]     != game.emptyChar              ) &&
                (game.gameBoard[row]    [col] == game.gameBoard[row + 1][col]) &&
                (game.gameBoard[row + 1][col] == game.gameBoard[row + 2][col]) &&
                (game.gameBoard[row + 2][col] == game.gameBoard[row + 3][col])   )
            { 
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row + i);     // row indices increment by 1 from 0
                    game.winningIndices[i][1] = (col);         // col indixes remain constant
                }
                if   (game.gameBoard[row][col] == game.player1Symbol) { return 1; }     // player 1 won
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
            if ((game.gameBoard[row][col]         != game.emptyChar                  ) &&
                (game.gameBoard[row]    [col]     == game.gameBoard[row - 1][col + 1]) && 
                (game.gameBoard[row - 1][col + 1] == game.gameBoard[row - 2][col + 2]) && 
                (game.gameBoard[row - 2][col + 2] == game.gameBoard[row - 3][col + 3])   )
            {
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row - i);     // row indices decrement by 1
                    game.winningIndices[i][1] = (col + i);     // col indices increment by 1
                }        
                if   (game.gameBoard[row][col] == game.player1Symbol) { return 1; }     // player 1 won
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
            if ((game.gameBoard[row][col]         != game.emptyChar                  ) &&
                (game.gameBoard[row]    [col]     == game.gameBoard[row - 1][col - 1]) && 
                (game.gameBoard[row - 1][col - 1] == game.gameBoard[row - 2][col - 2]) && 
                (game.gameBoard[row - 2][col - 2] == game.gameBoard[row - 3][col - 3])   )
            {
                for (int i = 0; i < 4; i++)
                {
                    game.winningIndices[i][0] = (row - i);     // row indices decrement by 1
                    game.winningIndices[i][1] = (col - i);     // col indices decrement by 1
                }
                if   (game.gameBoard[row][col] == game.player1Symbol) { return 1; }     // player 1 won
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
            if (game.gameBoard[row][col] == game.emptyChar){ return -1; }
        }
    }
    return 1;
}
void checkGameBoard()
{    
    if (game.totalMoves >= 7)     // the min num of moves required for any player to have won is 7 (ie 4 by the player 1)
    {
        // checking if player1 won
        if ((checkHorizontally() == 1) || (checkVertically() == 1) || (checkPosDiagonals() == 1) || (checkNegDiagonals() == 1))
        {
            game.gameState = 1;     // player1 wins
        }
        // checking if player2 won
        else if ((checkHorizontally() == 2) || (checkVertically() == 2) || (checkPosDiagonals() == 2) || (checkNegDiagonals() == 2))
        {
            game.gameState = 2;     // player2 wins
        }
        // checking if the game was a draw
        else if (checkDraw() == 1)
        {
            game.gameState = 0;     // game draws
        }   // else gamestate remains -1
    }
}

// for debugging the check functions
void shaykh_Debugger()
{
    printf("\n\n[Shaykh Debugger]\n\t[1] checkHorizontally(): %d\n\t[2] checkVertically():   %d\n\t[3] checkPosDiagonals(): %d\n\t[4] checkNegDiagonals(): %d\n\t[5] checkDraw():         %d\n\t[6] game.gameState:      %d\n\t[7] game.winningIndices: ", checkHorizontally(), checkVertically(), checkPosDiagonals(), checkNegDiagonals(), checkDraw(), game.gameState);
    for (int i = 0; i < 4; i++){ printf("[%d, %d] ", game.winningIndices[i][1], game.winningIndices[i][2]); }
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
                animateText("\n\n=====================\n[ >< ]  DRAW!  [ >< ]\n=====================", 123);
                break;

            case 1: 
            {   
                clearScreen();
                printGameBoard();
                int numBars = strlen("[* * *] WINNER: ") + strlen(game.player1Name) + strlen("! [* * *]");
                char bars[numBars + 1];     // + 1 for '\0'
                for (int i = 0; i < numBars; i++){ bars[i] = '='; }
                bars[numBars] = '\0';
                animateText("\n\n", 123); animateText(bars, 123); animateText("\n[* * *] WINNER: ", 123); animateText(game.player1Name, 123); animateText("! [* * *]\n", 123); animateText(bars, 123); animateText("\n", 123);
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
                animateText("\n\n", 123); animateText(bars, 123); animateText("\n[* * *] WINNER: ", 123); animateText(game.player2Name, 123); animateText("! [* * *]\n", 123); animateText(bars, 123); animateText("\n", 123);
                break;
            }
        }

        game.playGame = false;      // breaks from the inner while loop in main()
    }
}
void finishRound()
{
    wait(1500);    // wait 1.5s
    emptyBuffer();      // clearing the '\n' from the buffer
    printf("\n\nPress Enter to continue: ");
    char uselessStr[25];    // size 40 so the user can enter anything, even a faltoo long string, without the program breaking
    fgets(uselessStr, sizeof(uselessStr), stdin);    // used %s instead of %c and getchar() so that the program wont break with any possible input given by the user
}


// ai stuff ------------------------------------------------------------------------------------------------------------------------------------------

// global variables;
char gameBoardCopy[maxRows][maxCols];   //  a copy of the game.gameBoard
int availableColumns[maxCols];          //  an array containing the indices of the empty/available/playable columns in gameBoardCopy
int numOfAvailableColumns;              //  counter containing the num of elements in emptyColumnIndices

// helper functions
void makeGameBoardCopy()
{
    for (int row = 0; row < maxRows; row++)
    {
        strcpy(gameBoardCopy[row], game.gameBoard[row]);  
    }
}
void findAvailableColumns()
{
    numOfAvailableColumns = 0;
    for (int col = 0; col < game.colCount; col++)
    {
        if (game.gameBoard[0][col] == game.emptyChar)
        {
            availableColumns[numOfAvailableColumns] = col;
            numOfAvailableColumns++;
        }
    }
}
int positionNotFloating(int row, int col)
{
    if      (row == (game.rowCount - 1))                    { return 1; }       // is the very last row
    else if (gameBoardCopy[row + 1][col] != game.emptyChar) { return 1; }       // is not the last row, & the row after it is filled
    else                                                    { return 0; }       // is not the last row, & the row after it is empty
}

// checking functions
int simpleUpdateGameBoard(char tokenToDrop, int columnToUpdate)
{
    for (int row = 0; row < game.rowCount; row++)
    {
        if (gameBoardCopy[row][columnToUpdate] == game.emptyChar)
        {
            gameBoardCopy[row][columnToUpdate] = tokenToDrop;
            if (row > 0){ gameBoardCopy[row - 1][columnToUpdate] = game.emptyChar; }
        }
        else { return row; }        // returns the row num at which the token was dropped
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
            if ((gameBoardCopy[row][col]     != game.emptyChar             ) &&
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
            if ((gameBoardCopy[row]    [col] != game.emptyChar             ) &&
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
            if ((gameBoardCopy[row]    [col]     != game.emptyChar                 ) &&
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
            if ((gameBoardCopy[row]    [col]     != game.emptyChar                 ) &&
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
            if (gameBoardCopy[row][col] == game.emptyChar){ return -1; }
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
    { return 0; }       // game draws

    else if ((simpleCheckHorizontally(myToken) == 1) || (simpleCheckVertically(myToken) == 1) || (simpleCheckPosDiagonals(myToken) == 1) || (simpleCheckNegDiagonals(myToken) == 1))
    { return 1; }       // player1 wins

    else if ((simpleCheckHorizontally(myToken) == 2) || (simpleCheckVertically(myToken) == 2) || (simpleCheckPosDiagonals(myToken) == 2) || (simpleCheckNegDiagonals(myToken) == 2))
    { return 2; }       // player2 wins

    else 
    { return -1; }      // game continues as is
}

// debugging stuff
void _shaykh_Debugger()
{
    // also put numOfAvailableColumns, gameboardcopy, and emptycolumnindices
    // printf("\n\n[Shaykh Debugger]\n\t[1] simpleCheckHorizontally(): %d\n\t[2] simpleCheckVertically():   %d\n\t[3] simpleCheckPosDiagonals(): %d\n\t[4] simpleCheckNegDiagonals(): %d\n\t[5] simpleCheckDraw():         %d\n", simpleCheckHorizontally(), simpleCheckVertically(), simpleCheckPosDiagonals(), simpleCheckNegDiagonals(), simpleCheckDraw());
}

// AIs
int lvl1awwal_win_playRandom(char myToken)
{
    /*
        "if i can win now, i will win; else i will play a random position"

        > simulates one move of his across all available columns
        > if he wins by playing any of em or the game draws with his move, returns the numOfAvailableColumns of that column
        > else plays a random available column and return its numOfAvailableColumns
    */

    int move;
    findAvailableColumns();

    // checking for instant win
    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard('a', move);
        if ((simpleCheckGameBoard(myToken) == 0) || (simpleCheckGameBoard(myToken) == 1)){ return move; }    //  if ai wins or the game draws with the ai's move, return that move
    }

    // playing a random move
    srand(time(NULL));
    int randomIndex = (rand() % numOfAvailableColumns);
    move = availableColumns[randomIndex];
    return move;
}
int lvl2thani_win_dontLose_playRandom(char myToken, char opponentToken)
{
    /*
        "if i can win now, i will win; else if the opponent can win, i will block; else i will play a random position"

        > simulates one move of his across all available columns
        > if he wins by playing any of em or the game draws with his move, returns the numOfAvailableColumns of that column
        > else if the opponent can win next turn by playing a move, blocks that
        > else plays a random available column and return its numOfAvailableColumns
    */

    int move;
    findAvailableColumns();

    // checking for ai's instant win
    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard(myToken, move);
        if ((simpleCheckGameBoard(myToken) == 0) || (simpleCheckGameBoard(myToken) == 1)){ return move; }    //  if ai wins or the game draws with the ai's move, return that move
    }

    // blocking opponent's win
    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard(opponentToken, move);
        if ((simpleCheckGameBoard(myToken) == 2)){ return move; }    //  if the opponent can win in a move, return that move
    }

    // playing a random move
    srand(time(NULL));
    int randomIndex = (rand() % numOfAvailableColumns);
    move = availableColumns[randomIndex];
    return move;
}
int lvl3thalith_win_dontLose_prevent3InARow_playRandom(char myToken, char opponentToken)
{
    /*
        "win; block opponent's win; block opponent's potential 3-in-a-row; else play a random move"

        > if i can win, i will win
        > else if the opponent can win, i will block
        > else if the opponent can make 3 in a row, i will block
        > else i will play a random position 
    */

    int move;
    findAvailableColumns();

    // checking for ai's instant win
    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard(myToken, move);
        if ((simpleCheckGameBoard(myToken) == 0) || (simpleCheckGameBoard(myToken) == 1)){ return move; }    //  if ai wins or the game draws with the ai's move, return that move
    }

    // blocking opponent's win
    for (int i = 0; i < numOfAvailableColumns; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard(opponentToken, move);
        if ((simpleCheckGameBoard(myToken) == 2)){ return move; }    //  if the opponent can win in a move, return that move
    }

    // preventing the opponent's potential 3-in-a-row
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
                if      ((gameBoardCopy[row][j + k] == game.emptyChar) && (positionNotFloating(row, j + k))) { empty++;  }
                else if (gameBoardCopy[row][j + k] == opponentToken)                                         { filled++; }
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
                    if      ((gameBoardCopy[k - l][j + l] == game.emptyChar) && (positionNotFloating(k - l, j + l))) { empty++;  }
                    else if (gameBoardCopy[k - l][j + l] == opponentToken)                                           { filled++; }
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
                    if      ((gameBoardCopy[k - l][j - l] == game.emptyChar) && (positionNotFloating(k - l, j - l))) { empty++;  }
                    else if (gameBoardCopy[k - l][j - l] == opponentToken)                                           { filled++; }
                }
                if ((empty == 1) && (filled == 3)) { return col; }
            }
        }
    }

    // playing a random move
    srand(time(NULL));
    int randomIndex = (rand() % numOfAvailableColumns);
    move = availableColumns[randomIndex];
    return move;
}

int lvl4_simulates3Moves();     // 1st ai, 2nd opponent, 3rd ai
int lvl5_simulates5Moves();     // 1st ai, 2nd opponent, 3rd ai, 4th opponent, 5th ai


// Getting PLayers Moves ---------------------------------------------------------------------------------------------------------------------------- 
void getPlayerMove()
{
    // sets the column number of a player's move to game.playerMove

    if  (game.activePlayer == 1){ printf("\n\n[\033[1;33m%s\033[0m]", game.player1Name); }      // printing the playerName in color
    else                        { printf("\n\n[\033[1;34m%s\033[0m]", game.player2Name); }
    
    int userMove;
    while (true)
    {
        printf("\nEnter your move (Column 1-%d): ", game.colCount);
        scanf("%d", &userMove);

        if ((userMove < 1) || (userMove > game.colCount)){
            printf("> That column doesn't exist!");
            continue;
        }
        else if (game.gameBoard[0][userMove - 1] != game.emptyChar){       // checks if the very top row/position of the chosen column is full or not
            printf("> That column is already filled!");
            continue;
        }
        else{ 
            game.playerMove = --userMove;
            break; 
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
            game.playerMove = lvl1awwal_win_playRandom(game.player1Symbol);
        }
        else if (!strcmp(game.player1Name, "Thani"))
        {
            game.playerMove = lvl2thani_win_dontLose_playRandom(game.player1Symbol, game.player2Symbol);
        }
        else if (!strcmp(game.player1Name, "Thalith"))      // else wouldve worked fine but used elseif for expandibility
        {
            game.playerMove = lvl3thalith_win_dontLose_prevent3InARow_playRandom(game.player1Symbol, game.player2Symbol);
        }
    }
    else                        
    { 
        printf("\n\n[\033[1;34m%s\033[0m]", game.player2Name); 
        if (!strcmp(game.player2Name, "Awwal"))
        {
            game.playerMove = lvl1awwal_win_playRandom(game.player2Symbol);
        }
        else if (!strcmp(game.player2Name, "Thani"))
        {
            game.playerMove = lvl2thani_win_dontLose_playRandom(game.player2Symbol, game.player1Symbol);
        }
        else if (!strcmp(game.player2Name, "Thalith"))
        {
            game.playerMove = lvl3thalith_win_dontLose_prevent3InARow_playRandom(game.player2Symbol, game.player1Symbol);
        }
    }

    srand(time(NULL));
    int randomPrompt = (rand() % 10) + 1;

    printf("\n");
    switch (randomPrompt)
    {
        case 1:
            animateText("I think imma play at umm... ", 123);
            wait(2000);
            break;

        case 2:
            printf("Hmm my turn huh... ");
            wait(2000);
            break;

        case 3:
            animateText("Lets go with umm... ", 123);
            wait(2000);
            break;
        
        case 4:
            animateText("Oh noice move!\nLemme play... ", 123);
            wait(2000);
            break;

        case 5:
            animateText("I found a good one... :) ", 123);
            wait(2000);
            break;

        case 6:
            animateText("Why do u play so good?? ", 123);
            wait(2000);
            break;

        case 7:
            animateText("Am i losing already? ", 123);
            wait(2000);
            break;

        case 8:
            animateText("Lemme think a little... ", 123);
            wait(2000);
            break;    
        
        case 9:
            animateText("You are smart, i will give you that... ", 123);
            wait(2000);
            break; 
        
        case 10:
            animateText("My mom says to play... ", 123);
            wait(2000);
            break; 
    }

    printf("\n--> %d", game.playerMove - 1);
}


//  gameModes ---------------------------------------------------------------------------------------------------------------------------------------

void PvP()
{

    setGame();
    setGameBoard();
    setPlayers(2);
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
    setGameBoard();
    setPlayers(1);
    showGameBoard();

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
    setGameBoard();
    setPlayers(0);
    showGameBoard();

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
    do {
        switch (mainMenu())
        {
            case 1:
                game.continueProgram = true;
                playGame();
                break;

            case 2:
                game.continueProgram = false;
                exitGame();
                break;
        }
    } while (game.continueProgram);

    return 0;
}
