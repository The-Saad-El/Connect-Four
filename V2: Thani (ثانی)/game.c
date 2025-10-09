
#include "game.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>


gameConfig game;


void animateText(char strToAnimate[], int timeDelayms)
{
    for (int i = 0; i < strlen(strToAnimate); i++)
    {
        printf("%c", strToAnimate[i]);
        Sleep(timeDelayms);
    }
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
    game.sleepTime = 300;
    game.winningIndices[0][0];
}
void setPlayers()
{
    // players setup

    printf("\n\n[Players' Information]");

    getchar();      // to get rid of the newline char in the buffer (from mainMenu())
    printf("\n> \033[1;33mPlayer 1\033[0m\n");
    printf("      Enter your name: ");
    fgets(game.player1Name, sizeof(game.player1Name), stdin);
    if   ((strlen(game.player1Name) == 1) || !(strcmp(game.player1Name, "Player 2\n"))) { strcpy(game.player1Name, "Player 1"); }     // if the user didnt enter anything or entered {player 2}, sets p1name to {Player 1}
    else                                                                                { game.player1Name[strlen(game.player1Name) - 1] = '\0';}      // setting the trailing newline char to the null terminator char
    printf("      Enter your symbol: ");
    scanf(" %c", &game.player1Symbol);
    if ((game.player1Symbol == ' ') || (game.player1Symbol == '\n') || (game.player1Symbol == '2')){ game.player1Symbol = '1'; }

    getchar();
    printf("> \033[1;34mPlayer 2\033[0m\n");
    printf("      Enter your name: ");
    fgets(game.player2Name, sizeof(game.player2Name), stdin);
    if   ((strlen(game.player2Name) == 1) || !(strcmp(game.player2Name, "Player 1\n"))) { strcpy(game.player2Name, "Player 2"); }        // sets player2Name to {Player 2} if length is 1, or the user entered {Player 1}
    else{ 
        game.player2Name[strlen(game.player2Name) - 1] = '\0';
        if (!(strcmp(game.player1Name, game.player2Name))){ strcpy(game.player2Name, "Player 2"); }     // if player1name is same as player2name so sets player2name as "Player 2"
    }
    printf("      Enter your symbol: ");
    scanf(" %c", &game.player2Symbol);
    if ((game.player2Symbol == ' ') || (game.player2Symbol == '\n') || (game.player2Symbol == '1') || (game.player1Symbol == game.player2Symbol)){ game.player2Symbol = '2'; }
}
void setGameBoard()
{
    // gameBoard setup
    
    game.emptyChar = ' ';

    getchar();
    printf("\n\n[GameBoard Size]\n  1) Blitz: 6 x 5\n  2) Classic: 7 x 6\n  3) Grand: 8 x 7");      // Mini, Titan
    char userChoice;
    while (true)
    {
        printf("\nChoose your gameBoard [1-3]: ");
        scanf(" %c", &userChoice);
        if (userChoice == '1' || userChoice == '2' || userChoice == '3')
        {
            printf("> Accepted\n\n");
            break;
        }
        printf("> Enter between 1 and 3");
    }

    switch (userChoice)
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

    for (int i = 0; i < game.rowCount; i++)
    {
        for (int j = 0; j < game.colCount; j++){
            game.gameBoard[i][j] = game.emptyChar;
        }
    }
}
void initializeGame()
{
    setGame();
    setPlayers();
    setGameBoard();
}

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
                    if ((game.winningIndices[k][0] == i) && (game.winningIndices[k][1] == j)){   // [k][0] has the i value of the winning position's index & [k][1] the j value
                        isWinningIndex = true;
                        break;
                    }
                }
            }

            printf("  |  ");
            if (isWinningIndex){
                if   (game.gameBoard[i][j] == game.player1Symbol){ printf("\033[1;33;40m%c\033[0m", game.gameBoard[i][j]); }     // bold yellow color with grey highlighting for player1 (winner)
                else                                             { printf("\033[1;34;40m%c\033[0m", game.gameBoard[i][j]); }     // bold blue color with grey highlighting for player2 (winner)
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
    Sleep(2000);    // wait 2s
    system("cls");
    printGameBoard();
}

int getPlayerMove()
{
    // sets the column number of a player's move to game.playerMove

    switch (game.activePlayer)
    {
        case 0: game.activePlayer = 1; break;
        case 1: game.activePlayer = 2; break;
        case 2: game.activePlayer = 1; break;
    }

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
            game.playerMove = userMove;
            break; 
        }
    }
}

void updateGameBoard()
{
    game.totalMoves++;
    char playerMark = ((game.activePlayer == 1)? game.player1Symbol : game.player2Symbol);
    int columnToUpdate = --game.playerMove;

    for (int row = 0; row <= game.rowCount; row++)    // why <= and not just < ???
    {
        system("cls");
        printGameBoard();
        if (game.gameBoard[row][columnToUpdate] == game.emptyChar)
        {
            if (row > 0){ game.gameBoard[row - 1][columnToUpdate] = game.emptyChar; }
            game.gameBoard[row][columnToUpdate] = playerMark;
            Sleep(game.sleepTime);
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

void evaluateGameBoard()
{
    if (game.gameState != -1)        // continue the round if gameState == -1; will only work for totalMoves >= 7
    {
        Sleep(330);     // a little pause before printing results
        switch (game.gameState)
        {
            case 0: 
                animateText("\n\n=====================\n[ >< ]  DRAW!  [ >< ]\n=====================", 123);
                break;

            case 1: 
            {   
                system("cls");
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
                system("cls");
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
        Sleep(1500);    // wait 1.5s
        getchar();      // clearing the '\n' from the buffer
        printf("\n\nPress Enter to continue: ");
        char uselessStr[40];    // size 40 so the user can enter anything, even a faltoo long string, without the program breaking
        fgets(uselessStr, sizeof(uselessStr), stdin);    // used %s instead of %c and getchar() so that the program wont break with any possible input given by the user
    }
}
