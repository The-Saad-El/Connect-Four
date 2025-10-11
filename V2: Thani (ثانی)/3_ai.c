#include "2_game.h"
#include <time.h>       // time()
#include <stdio.h>
#include <stdlib.h>     // rand, srand
#include <stdbool.h>
#include "3_ai.h"

/*
    AI.h Declarations
    ->  int lvl1_firstAvailable();
    ->  int lvl2_randomPosition();
    ->  int lvl3_checksWin();       // iterates through each column and calls the checkGameBoard() to check for win
    ->  int lvl4_checksLoss();
    ->  int lvl5_simulates2Turns();
    ->  int lvl6_simulates3Turns();
*/


// global variables;
int gameBoardCopy[maxRows][maxCols];    //  a copy of the game.gameBoard
int availableColumns[maxCols];          //  an array containing the indices of the empty/available/playable columns in gameBoardCopy
int index;                              //  counter containing the num of elements in emptyColumnIndices
int gameStatus;                         //  stores the result of simpleCheckGameBoard()
int moveScore;                          //  stores the result of findMoveScore()


void makeGameBoardCopy()
{
    strcpy(gameBoardCopy, game.gameBoard);
}
void findAvailableColumns()
{
    index = 0;
    for (int col = 0; col < game.colCount; col++)
    {
        if (game.gameBoard[0][col] == game.emptyChar)
        {
            availableColumns[index] = col;
            index++;
        }
    }
}
int playRandomMove()
{
    srand(time(NULL));
    int randomIndex = (rand() % index);
    return availableColumns[randomIndex];
}

// checking functions
void simpleUpdateGameBoard(int columnToUpdate)
{
    for (int row = 0; row < game.rowCount; row++)
    {
        if (gameBoardCopy[row][columnToUpdate] == game.emptyChar)
        {
            gameBoardCopy[row][columnToUpdate] = game.aiSymbol;
            if (row > 0){ gameBoardCopy[row - 1][columnToUpdate] = game.emptyChar; }
        }
        else { break; }
    }
}
int simpleCheckHorizontally()
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
                if    (gameBoardCopy[row][col] == game.player1Symbol) { return 1; }     // player 1 won
                else                                                  { return 2; }     // player 2 won
            }
        }
    }

    return -1;  // no four in a row horizontally
}
int simpleCheckVertically()
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
                if   (gameBoardCopy[row][col] == game.player1Symbol) { return 1; }     // player 1 won
                else                                                 { return 2; }     // player 2 won
            }
        }
    }

    return -1;  // no four in a row vertically
}
int simpleCheckPosDiagonals()
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
                if   (gameBoardCopy[row][col] == game.player1Symbol) { return 1; }     // player 1 won
                else                                                 { return 2; }     // player 2 won
            }    
        }
    }

    return -1;      // no complete +ve diagonal
}
int simpleCheckNegDiagonals()
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
                if   (gameBoardCopy[row][col] == game.player1Symbol) { return 1; }     // player 1 won
                else                                                 { return 2; }     // player 2 won
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
int simpleCheckGameBoard()
{    
    /*
        if (game.totalMoves >= 7)     // the min num of moves required for any player to have won is 7 (ie 4 by the player 1)
        implement the above in the indiviudal simulation AI modules
        make it smthing like this: (game.totalMoves >= (7 - [aiType: 2 simulation, 4 simulation etc]))
    */
    
    if (simpleCheckDraw() == 1)
    { return 0; }       // game draws

    else if ((simpleCheckHorizontally() == 1) || (simpleCheckVertically() == 1) || (simpleCheckPosDiagonals() == 1) || (simpleCheckNegDiagonals() == 1))
    { return 1; }       // player1 wins

    else if ((simpleCheckHorizontally() == 2) || (simpleCheckVertically() == 2) || (simpleCheckPosDiagonals() == 2) || (simpleCheckNegDiagonals() == 2))
    { return 2; }       // player2 wins

    else 
    { return -1; }      // game continues as is
}
int findMoveScore()
{
    switch (gameStatus)
    {
        case  0:                   // draws
            { return   5; }
        case  1:                   // player wins
            { return -10; }
        case  2:                   // ai wins
            { return  10; }
        case -1:                   // continues
            { return   0; }
    }
}

void shaykh_Debugger()
{
    // also put index, gameboardcopy, and emptycolumnindices
    printf("\n\n[Shaykh Debugger]\n\t[1] simpleCheckHorizontally(): %d\n\t[2] simpleCheckVertically():   %d\n\t[3] simpleCheckPosDiagonals(): %d\n\t[4] simpleCheckNegDiagonals(): %d\n\t[5] simpleCheckDraw():         %d\n", simpleCheckHorizontally(), simpleCheckVertically(), simpleCheckPosDiagonals(), simpleCheckNegDiagonals(), simpleCheckDraw());
}


// AIs
int lvl1_simulates1Move()
{
    /*
        "if i can win now, i will win; else i will play a random position"

        > simulates one move of his across all available columns
        > if he wins by playing any of em, returns the index of that column
        > else plays a random available column and return its index
    */

    findAvailableColumns();

    int move;
    for (int i = 0; i < index; i++)
    {
        makeGameBoardCopy();
        move = availableColumns[i];
        simpleUpdateGameBoard(move);
        if (simpleCheckGameBoard() == 2){ return move; }    // if ai wins, return that move
    }

    return playRandomMove();
}

int lvl2_simulates2Moves()   // the min num of simulations is 2
{
    int emptyColumnIndices[game.colCount][3];   // 1st for index, 2nd for outcome if played move at that index/column 3rd for 2nd outcome
    int index = 0;

    for (int i = 0; i < game.colCount; i++)
    {
        if (game.gameBoard[0][i] == game.emptyChar)
        {
            emptyColumnIndices[index][0] = i;
            index++;
        }
    }

    char gameBoardCopy[8][7];   // size of .gameBoard
    for (int i = 0; i < index; i++)
    {
        strcpy(gameBoardCopy, game.gameBoard);
        int indexToUpdate = emptyColumnIndices[i][0];
        simpleUpdateGameBoard(indexToUpdate);
        int result = simpleCheckGameBoard();
        emptyColumnIndices[i][1] = result;
    }

}
int lvl3_simulates3Moves();
int lvl4_simulates4Moves();
int lvl5_simulates5Moves();
