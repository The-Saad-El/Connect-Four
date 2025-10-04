/*
    n x n gameBoard
    replayable
    menu
    animations

    boardSizes
        1) 6 x 5
        2) 7 x 6
        3) 8 x 7

    started: 8ish 4/10/25
*/

#include <stdio.h>
#include <windows.h>
#include <stdbool.h>


typedef struct 
{
    // perhaps add a seperate config function or initialize all elements properly at start of initializeGame
    // add a menu function; move the whole connect four title printing to menu
    
    char gameBoard[8][7];   // the gameBoard, initialized to the max possible size
    int rowCount;           // number of rows of the gameBoard
    int colCount;           // number of columns of the gameBoard
    char _player1Name_[25];       // name of player1
    char _player1Symbol_;     // symbol for player1's spaces
    char _player2Name_[25];       // name of player2
    char _player2Symbol_;     // symbol for player2's spaces
    char emptyChar;         // the char used to represent an empty position
    int _totalMoves_;         // counter for the total number of moves made
    int activePlayer;       // [CURRENTLY] odd: player1, even: player2; also serves as a counter for total moves   [IDEALLY] 1:player1, 2:player2 
    bool playGame;          // flag used for the main while loop main()
    int sleepTime;          // the time, in ms, used for the argument of the Sleep() function, for animating gameBoard updates

} gameConfig;

gameConfig game;


void mainMenu()
{
    system("cls");      // clears the terminal screen

    printf("\n==================");
    printf("\n=> Connect Four <=");
    printf("\n==================\n\n");

    printf("[Main Menu]\n  [1] Play\n  [2] Exit");
    int userChoice;
    while (true)
    {
        printf("\nEnter your choice [1/2]: ");
        scanf("%d", &userChoice);

        if (userChoice == 1){ break; }
        else if (userChoice == 2)
        { 
            printf("\n");
            for (int i = 5; i >= 1; i--)
            {
                printf("\rExiting in %d...", i);
                Sleep(1000);
            }
            printf("\rExiting now.  ");
            exit(0);
        }
        else    // ie ((userChoice != 1) && (userChoice != 2))
        {
            printf("> [!] Enter either 1 or 2");
            continue;
        }
    }
}

void initializeGame()
{
    game.activePlayer = 0;
    game.playGame = true;
    game.sleepTime = 400;
    game.emptyChar = ' ';

    char userChoice;
    while (1)
    {
        printf("\nChoose your gameBoard size\n  1) Blitz: 6 x 5\n  2) Classic: 7 x 6\n  3) Large: 8 x 7\nEnter [1-3]: ");
        scanf(" %c", &userChoice);
        if (userChoice == '1' || userChoice == '2' || userChoice == '3')
        {
            printf("> Accepted\n");
            break;
        }
        printf("> Enter between 1 and 3\n");
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

void printGameBoard()
{
    int numOfDashes = (6 * game.colCount);    // calculating the num of dashes required for rows
    char rowDashes[numOfDashes + 1];
    for (int i = 0; i <= numOfDashes; i++){ rowDashes[i] = '-'; }
    rowDashes[numOfDashes + 1] = '\0';

    printf("\n  %s\n", rowDashes);    // topmost row of dashes
    for (int i = 0; i < game.rowCount; i++)
    {
        for (int j = 0; j < game.colCount; j++)
        {
            printf("  |  %c", game.gameBoard[i][j]);
        }
        printf("  |\n  %s\n", rowDashes);
    }
    
    // for printing column numbers
    printf("  ");    // empty space of width 2 (is fixed)
    for (int i = 0; i < game.colCount; i++)
    {
        printf("  [%d] ", i+1);
    }

    printf("\n");
}

int getPlayerMove()
{
    // returns the column number of a player's move

    printf("\n\n[%s]", (((game.activePlayer % 2) == 1))? "Player 1" : "Player 2");
    int userMove;
    while (1)
    {
        printf("\nEnter your move (column number): ");
        scanf("%d", &userMove);

        if ((userMove < 1) || (userMove > game.colCount)){
            printf("> That column doesn't exist!");
            continue;
        }
        else if (game.gameBoard[0][userMove - 1] != game.emptyChar){       // checks if the very top row is full or not
            printf("> That column is already filled!");
            continue;
        }
        else { break; }
    }

    return userMove;
}

void updateGameBoard(int columnToUpdate)
{
    char playerMark = (((game.activePlayer % 2) == 1)? '1' : '2');
    columnToUpdate--;

    for (int i = 0; i <= game.rowCount; i++)    // why <= and not just < ???
    {
        system("cls");
        printGameBoard();
        if (game.gameBoard[i][columnToUpdate] == game.emptyChar)
        {
            if (i > 0){ game.gameBoard[i - 1][columnToUpdate] = game.emptyChar; }
            game.gameBoard[i][columnToUpdate] = playerMark;
            Sleep(game.sleepTime);
        }
        else { break; }
    }
}

int checkHorizontally()
{
    /*
        checking horizontally (rows)

        -1: continue game (no complete, horizontal row)
        1 : player 1 won (has a complete, horizontal row)
        2 : player 2 won (has a complete, horizontal row)
    */

    for (int i = 0; i < game.rowCount; i++)
    {
        for (int j = 0; j < (game.colCount - 3); j++)
        {
            if (((game.gameBoard[i][j])     == (game.gameBoard[i][j + 1])) && 
                ((game.gameBoard[i][j + 1]) == (game.gameBoard[i][j + 2])) && 
                ((game.gameBoard[i][j + 2]) == (game.gameBoard[i][j + 3]))    )
            { 
                if      (game.gameBoard[i][j] == '1') { return 1; }     // player 1 won
                else if (game.gameBoard[i][j] == '2') { return 2; }     // player 2 won
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

    for (int i = 0; i < game.colCount; i++)
    {
        for (int j = 0; j < (game.rowCount - 3); j++)
        {
            if (((game.gameBoard[i][j])     == (game.gameBoard[i][j + 1])) && 
                ((game.gameBoard[i][j + 1]) == (game.gameBoard[i][j + 2])) && 
                ((game.gameBoard[i][j + 2]) == (game.gameBoard[i][j + 3]))    )
            { 
                if      (game.gameBoard[i][j] == '1') { return 1; }     // player 1 won
                else if (game.gameBoard[i][j] == '2') { return 2; }     // player 2 won
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

        this took longgg...
    */

    for (int i = game.rowCount; i >= 4; i--)     // reverse loop (from the bottom row to the topmost)
    {
        for (int j = 1; j <= (game.colCount - 3); j++)
        {
            int row = i-1, column = j-1;
            if (((game.gameBoard[row][column])     == (game.gameBoard[row-1][column+1])) && 
                ((game.gameBoard[row-1][column+1]) == (game.gameBoard[row-2][column+2])) && 
                ((game.gameBoard[row-2][column+2]) == (game.gameBoard[row-3][column+3])))
                {
                    if      (game.gameBoard[i][j] == '1') { return 1; }     // player 1 won
                    else if (game.gameBoard[i][j] == '2') { return 2; }     // player 2 won
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

        this took longgg...
    */

    for (int i = game.rowCount; i >= 4; i--)     // reverse loop (from the bottom row to the topmost)
    {
        for (int j = game.colCount; j >= 4; j--)     // from right to left
        {
            int row = (i - 1), column = (j - 1);
            if (((game.gameBoard[row][column])     == (game.gameBoard[row-1][column-1])) && 
                ((game.gameBoard[row-1][column-1]) == (game.gameBoard[row-2][column-2])) && 
                ((game.gameBoard[row-2][column-2]) == (game.gameBoard[row-3][column-3])))
                {
                    if      (game.gameBoard[i][j] == '1') { return 1; }     // player 1 won
                    else if (game.gameBoard[i][j] == '2') { return 2; }     // player 2 won
                }    
        }
    }

    return -1;      // no complete -ve m diagonal
}
int checkDraw()
{
    /*
        returns 
            > 0: if no draw (ie empty space left)
            > 1: if draw (ie no more empty space left)
    */

    for (int i = 0; i < game.rowCount; i++)
    {
        for (int j = 0; j < game.colCount; j++)
        {
            if (game.gameBoard[i][j] == game.emptyChar){ return 0; }
        }
    }
    return 1;
}
int checkGameBoard()
{
    if (game.activePlayer >= 7)     // the min num of moves required for any player to have won is 7 (ie 4 by the player 1)
    {
        // checking horizontally (rows)
        switch (checkHorizontally())
        {
            case 1: return 1;
            case 2: return 2;
        }
        
        // checking vertically (columns)
        switch (checkVertically())
        {
            case 1: return 1;
            case 2: return 2;
        }

        // checking +ve diagonals (diagonals with a +ve gradient)
        switch (checkPosDiagonals())
        {
            case 1: return 1;
            case 2: return 2;
        }

        // checking -ve diagonals (diagonals with a -ve gradient)
        switch (checkNegDiagonals())
        {
            case 1: return 1;
            case 2: return 2;
        }

        // checking for draw
        if (checkDraw() == 1) { return 0; }
    }

    return -1;     // else the game continues
}


int main()
{
    mainMenu();
    initializeGame();
    printf("\n This is the gameBoard:");
    printGameBoard();
    Sleep(3000);    // wait 3s

    while (game.playGame)
    {
        game.activePlayer++;
        int playerMove = getPlayerMove();
        updateGameBoard(playerMove);
        int gameStatus = checkGameBoard();

        switch (gameStatus)
        {
            case -1: continue;   // game continues

            case  0:  
                printf("\n==================\n[ >< ]  DRAW!  [ >< ]\n================");
                game.playGame = false;      // break from the top while loop
                break;
            case  1:
                printf("\n==========================\n[* * *] WINNER: Player 1!\n==========================");
                game.playGame = false;      // break from the top while loop
                break;
            case  2:
                printf("\n==========================\n[* * *] WINNER: Player 2!\n==========================");
                game.playGame = false;      // break from the top while loop
                break;
        }
    }

    return 0;
}