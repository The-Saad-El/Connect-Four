// ai stuff -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// headers
#include <string.h>
#include <stdlib.h>
#include "globals.h"

// global variables
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
        if ((row < game.rowCount - 2) && (row != 0))       // there are atleast 2 rows below the row where the token dropped to and the row isnt the first (topmost) row (will be pointless to block by placing on the tompost row since the opponent cant ever make a 4-in-a-row with that)
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
