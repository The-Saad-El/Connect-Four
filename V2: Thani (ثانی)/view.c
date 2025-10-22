// view() (filing) --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// declarations
#define winPoints 200
#define drawPoints 50
#define defeatPoints 10

// headers
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "globals.h"


// leaderBoards
void createTempLeaderBoards_PvP()
{
    // creating a temp file with all leaderBoard records (new & old)
    
    FILE *mainFile = fopen("gameFiles/leaderBoards.txt", "a+");         // used / instead of (\\);  [a+] mode will allow reading, appending, & creation of file (important when the program runs for the 1st time)
    FILE *tempFile = fopen("gameFiles/temp.txt", "w");                  // will append all unique records to temp file
    
    if      (mainFile == NULL) { printf("[!] ERROR: Couldn't access file 'gameFiles/leaderBoards.txt, a+'"); }
    else if (tempFile == NULL) { printf("[!] ERROR: Couldn't access file 'gameFiles/temp.txt, w'");          }
    else
    {
        char playerName[arbitrarySize];
        int gamesPlayed, wins, draws, defeats, score;
        bool player1ExistsInLeaderBoards = false, player2ExistsInLeaderBoards = false;

        fseek(mainFile, 0, SEEK_SET);       // in a+ mode, file pointer is at very end of file (for appending stuff). so fscanf(...) will always return 0 as it wont have anything to read at eof. therefore fseek(...) causes file pointer to move at the very start of the text file.      fseek(mainFile, 0, SEEK_SET); is same as rewind(mainFile);
        while (true)
        {
            int numOfScans = fscanf(mainFile, "Score: [%d] | Player Name: [%[^]]] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", 
                                                &score, playerName, &gamesPlayed, &wins, &draws, &defeats);
            if (numOfScans != 6){ break; }      // max line reached
            else
            {
                if (!strcmp(game.player1Name, playerName))           // p1 already exists in the leaderboards; saves his updated info in a tempFile
                {
                    gamesPlayed++;
                    (game.gameState == 1)? wins++ : (game.gameState == 0)? draws++ : defeats++;
                    score = (wins*winPoints + draws*drawPoints + defeats*defeatPoints);       // random scoring formula/criteria;  5 draws > 1 win & 21 defeats > 1 win due to the greater volume of games played of the formers
                    fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, playerName, gamesPlayed, wins, draws, defeats);
                    player1ExistsInLeaderBoards = true;
                }
                else if (!strcmp(game.player2Name, playerName))     // p2 already exists in the leaderboards
                {
                    gamesPlayed++;
                    (game.gameState == 2)? wins++ : (game.gameState == 0)? draws++ : defeats++;
                    score = (wins*winPoints + draws*drawPoints + defeats*defeatPoints);       // random scoring formula/criteria;  5 draws > 1 win & 21 defeats > 1 win due to the greater volume of games played of the formers
                    fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, playerName, gamesPlayed, wins, draws, defeats);       // playerName could have been replaced with player2Name
                    player2ExistsInLeaderBoards = true;
                }
                else        // player != player1; copies the same playerData to temp file
                {
                    fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, playerName, gamesPlayed, wins, draws, defeats);
                }
            }
        }

        if (!player1ExistsInLeaderBoards)        // creates record of p1 in temp file if it doesnt exist in leaderboards
        { 
            wins = 0, draws = 0, defeats = 0;
            (game.gameState == 1)? wins++ : (game.gameState == 0)? draws++ : defeats++;
            score = (wins*winPoints + draws*drawPoints + defeats*defeatPoints); 
            fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [1] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, game.player1Name, wins, draws, defeats); 
        }
        
        if (!player2ExistsInLeaderBoards)        // if no record/leaderBoard-position occurs of p2, creates it and then appends in the temp file
        { 
            wins = 0, draws = 0, defeats = 0;
            (game.gameState == 2)? wins++ : (game.gameState == 0)? draws++ : defeats++;
            score = (wins*winPoints + draws*drawPoints + defeats*defeatPoints);
            fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [1] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, game.player2Name, wins, draws, defeats); 
        }
    }

    //  closing both files 
    if (mainFile != NULL) { fclose(mainFile); }         // intentionally didnt close these files inside the else block above
    if (tempFile != NULL) { fclose(tempFile); }         // if one file opened and the other didnt, i will print the error msg but 1 file will remain opened. isliye brought these outside the if block
}
void createTempLeaderBoards_PvAI()
{
    FILE *mainFile = fopen("gameFiles/leaderBoards.txt", "a+");         // used / instead of (\\);  [a+] mode will allow reading, appending, & creation of file (important when the program runs for the 1st time)
    FILE *tempFile = fopen("gameFiles/temp.txt", "w");                  // will append all unique records to temp file

    if      (mainFile == NULL) { printf("[!] ERROR: Couldn't access file 'gameFiles/leaderBoards.txt, a+'"); }
    else if (tempFile == NULL) { printf("[!] ERROR: Couldn't access file 'gameFiles/temp.txt, w'");         }
    else
    {
        bool playerExistsInLeaderBoards;
        char playerName[arbitrarySize];
        int gamesPlayed, wins, draws, defeats, score;

        // for player1 (applicable in both PvP() & PvAI())
        playerExistsInLeaderBoards = false;
        fseek(mainFile, 0, SEEK_SET);       // in a+ mode, file pointer is at very end of file (for appending stuff). so fscanf(...) will always return 0 as it wont have anything to read at eof. therefore fseek(...) causes file pointer to move at the very start of the text file.      fseek(mainFile, 0, SEEK_SET); is same as rewind(mainFile);
        while (true)
        {
            int numOfScans = fscanf(mainFile, "Score: [%d] | Player Name: [%[^]]] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", 
                                                &score, playerName, &gamesPlayed, &wins, &draws, &defeats);
            if (numOfScans != 6){ break; }      // max line reached
            else
            {
                if (!strcmp(game.player1Name, playerName))     // p1 already exists in the leaderboards; saves his updated info in a tempFile
                {
                    gamesPlayed++;
                    (game.gameState == 1)? wins++ : (game.gameState == 0)? draws++ : defeats++;
                    score = (wins*winPoints + draws*drawPoints + defeats*defeatPoints);       // random scoring formula/criteria;  5 draws > 1 win & 21 defeats > 1 win due to the greater volume of games played of the formers
                    fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, playerName, gamesPlayed, wins, draws, defeats);
                    playerExistsInLeaderBoards = true;
                }
                else        // player != player1; copies the same playerData to temp file
                {
                    fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, playerName, gamesPlayed, wins, draws, defeats);
                }
            }
        }
        if (!playerExistsInLeaderBoards)        // creates record of p1 in temp file if it doesnt exist in leaderboards
        { 
            wins = 0, draws = 0, defeats = 0;
            (game.gameState == 1)? wins++ : (game.gameState == 0)? draws++ : defeats++;
            score = (wins*winPoints + draws*drawPoints + defeats*defeatPoints); 
            fprintf(tempFile, "Score: [%d] | Player Name: [%s] | Games Played: [1] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", score, game.player1Name, wins, draws, defeats); 
        }
    }

    //  closing both files 
    if (mainFile != NULL) { fclose(mainFile); }         // intentionally didnt close these files inside the else block above
    if (tempFile != NULL) { fclose(tempFile); }         // if one file opened and the other didnt, i will print the error msg but 1 file will remain opened. isliye brought these outside the if block
}
void updateMainLeaderBoards()
{
    // reads records from gameFiles/temp.txt, sorts em & then writes em to the mainFile

    FILE *mainFile = fopen("gameFiles/leaderBoards.txt", "w");          // reopening both files but in different modes than last time
    FILE *tempFile = fopen("gameFiles/temp.txt", "r");                  

    if      (mainFile == NULL) { printf("[!] ERROR: Couldn't access file 'gameFiles/leaderBoards.txt, w'"); }
    else if (tempFile == NULL) { printf("[!] ERROR: Couldn't access file 'gameFiles/temp.txt, r'");         }
    else
    {
        typedef struct          // alhamdulillah aala idea teh yeh struct istemaal karnay ka :>
        {
            int score;          
            char record[199];   // a veryyy large string 
        } tempStruct;

        int index = 0, numOfScans;
        tempStruct tempFileData[arbitrarySize];     // 35 records ought to be more than enough

        // storing data from tempFile to array tempFileData (an array of length 35 & datatype tempStruct (a struct))
        while (true)
        {
            numOfScans = fscanf(tempFile, "Score: [%d]", &tempFileData[index].score);       // file pointer is moved after [%d] so fgets only gets the partial line/record (ie after Score: [%d])
            if (numOfScans != 1){ break; }
            
            fgets(tempFileData[index].record, sizeof(tempFileData[index].record), tempFile);        // no need to check for NULL since the above check suffices
            index++;
        }

        // sorting records in array tempFileData based on descending scores
        for (int i = 0; i < index; i++)
        {
            for (int j = 0; j < (index - i - 1); j++)       // bubble sort
            {
                if (tempFileData[j].score < tempFileData[j + 1].score)      // descending order (highest to smallest)
                {
                    tempStruct temp = tempFileData[j];
                    tempFileData[j] = tempFileData[j + 1];
                    tempFileData[j + 1] = temp;
                }
            }
        }

        // updating main file
        for (int line = 0; line < index; line++)
        {
            /*
            couldve used sprintf...
            char record[199]; 
            sprintf(record, "Score: [%d]%s", tempFileData[line].score, tempFileData[line].record);
            fputs(record, mainFile);

            orrr...
            sprintf(tempFileData[line].record, "Score: [%d]%s", tempFileData[line].score, tempFileData[line].record);
            fputs(tempFileData[line].record, mainFile);
            */

            fprintf(mainFile, "Score: [%d]", tempFileData[line].score);      
            fputs(tempFileData[line].record, mainFile); // tempFileData[line].record has the partial record after Score: [%d]
        }
    }

    //  closing both files 
    if (mainFile != NULL) { fclose(mainFile); }         // intentionally didnt close these files inside the else block above
    if (tempFile != NULL) { fclose(tempFile); }         // if one file opened and the other didnt, i will print the error msg but 1 file will remain opened. isliye brought these outside the if block
    remove("gameFiles/temp.txt");                       // deletes the temp file after finishing the processing & updating of leaderboards
}
void updateLeaderBoards()
{   
    // creating a temp file with all leaderBoard records (new + old)
    if      (!strcmp(game.gameMode, "PvP"))  { createTempLeaderBoards_PvP();  }     // for PvP
    else if (!strcmp(game.gameMode, "PvAI")) { createTempLeaderBoards_PvAI(); }     // for PvAI   (couldve used else and it wouldve worked the same inshaAllah)

    // updating gameFiles/leaderBoards.txt
    updateMainLeaderBoards();
}
void displayLeaderBoards()
{
    printConnectFourTitle();

    FILE *fPtr = fopen("gameFiles/leaderBoards.txt", "a+");        // used a+ (read and append; creates file if not existing) instead of r so that when the user opens the program for the 1st time and opens history, he wont see an error msg due to the gameFiles/gameHistory.txt file not exisitng; rather the file will be created 
    if (fPtr == NULL)
    { 
        printf("[!] ERROR: Couldn't access file 'gameFiles/leaderBoards.txt'"); 
    }
    else
    {
        animateText("-------------------------------------------------------------------------------------------------------------\n", animateTextDelay_13ms);
        printf("| %7s | %-25s | %15s | %9s | %9s | %9s | %13s |\n", "Rank", "Player Name", "Games Played", "Wins", "Draw", "Defeats", "Score");
        animateText("-------------------------------------------------------------------------------------------------------------\n", animateTextDelay_13ms);
        
        
        typedef struct
        {
            char playerName[arbitrarySize];
            char score[10];         // initially declared this as an int, but later used string due to sprintf
        } tempStruct;
        tempStruct top3Players[3] = {{"", ""}, {"", ""}, {"", ""}};          // for top3Players podium;  initializing all 3 positions

        char playerName[arbitrarySize];
        int rank = 0, gamesPlayed, wins, draws, defeats, score;
        while (true)
        {
            int numOfScans = fscanf(fPtr, "Score: [%d] | Player Name: [%[^]]] | Games Played: [%d] | Wins: [%d] | Draws: [%d] | Defeats: [%d]\n", 
                                            &score, playerName, &gamesPlayed, &wins, &draws, &defeats);
            if (numOfScans != 6){ break; }
            else
            {
                printf("| %7d | %-25s | %15d | %9d | %9d | %9d | %13d |\n", rank + 1, playerName, gamesPlayed, wins, draws, defeats, score);
            }
            if (rank < 3)
            {
                // old comment (when was using sprintf in printing podium part below): adding () around playerScores;   couldve used sprintf during saving data in the struct above (score wouldve been a string)
                strcpy(top3Players[rank].playerName, playerName);           // using rank as an index (0-2)
                sprintf(top3Players[rank].score, "(%d)", score);            // op idea :D alhamdulillah
            }
            rank++;
        }
        if (rank == 0){ printf("%63s\n", "(No Available Data)"); }
        animateText("-------------------------------------------------------------------------------------------------------------\n", animateTextDelay_13ms);

        // making podiums for top3Players
        // this took long...
        printf("\n\n\n");
        if (rank == 0)
        { 
            wait(2000);                 // a little pause
            animateText("Is no one playing my game?!\n", animateTextDelay_63ms);            // :)
        }     

        printf("%27s%9s\n", " ", top3Players[0].playerName);
        printf("%27s%10s\n", " ", top3Players[0].score);
        printf("%27s---------------\n", " ");          // 15 dashes
        printf("%27s|%6s\033[1;33m1\033[0m%6s|\n", " ", " ", " ");
        printf("%27s|%13s|\n", " ", " ");
        printf("%27s|%13s|\n", " ", " ");

        printf("%25s  |%13s|\n", top3Players[1].playerName, " ");
        printf("%25s  |%13s|\n", top3Players[1].score, " ");
        printf("%13s--------------|%13s|\n", " ", " ");
        printf("%13s|%6s\033[1;33m2\033[0m%6s|%13s|\n", " ", " ", " ", " ");
        printf("%13s|%13s|%13s|\n", " ", " ", " ");

        printf("%13s|%13s|%13s|  %s\n", " ", " ", " ", top3Players[2].playerName);
        printf("%13s|%13s|%13s|  %s\n", " ", " ", " ", top3Players[2].score);
        printf("%13s|%13s|%13s|--------------\n", " ", " ", " ");
        printf("%13s|%13s|%13s|%6s\033[1;33m3\033[0m%6s|\n", " ", " ", " ", " ", " ");
        printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
        printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
        printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
        printf("%13s|%13s|%13s|%13s|\n", " ", " ", " ", " ");
        //printf("%13s-------------------------------------------", " ");
        printf("\n\n");


        fclose(fPtr);
    }

    wait(2000);         // wait some sec
    pressEnterToContinue();
}

// gameHistory
void saveGameHistory()                  // runs only at the end of a game/round
{   
    /*
        stores the following information about each game
            > gameNum
            > dateTime
            > match (quick or custom)
            > gameMode (pvp, pvai, or aivai)
            > player1
            > player2
            > gameBoard (size)
            > totalMoves
            > game duration (in mins)
            > result (win/draw)

        > is called at just the end of a game
        > (wont store the game details if the game didnt reach a conclusion (ie didnt end))
    */
    
    FILE *gameHistory = fopen("gameFiles/gameHistory.txt", "a");       // / instead of \\ (the former works on all platforms while the latter is windows-dependent)
    if (gameHistory == NULL)
    { 
        printf("[!] ERROR: Couldn't access file 'gameFiles/gameHistory.txt'"); 
    }
    else
    {
        char match[7];
        strcpy(match, (game.quickMatch)? "Quick" : "Custom");

        char gameBoard[8];
        switch (game.colCount)
        {
            case 5: strcpy(gameBoard, "Mini");    break;
            case 6: strcpy(gameBoard, "Blitz");   break;
            case 7: strcpy(gameBoard, "Classic"); break;
            case 8: strcpy(gameBoard, "Grand");   break;
            case 9: strcpy(gameBoard, "Titan");   break;
        }

        char result[30];
        switch(game.gameState)
        {
            case 0: 
                strcpy(result, "Draw");
                break;
            case 1: 
                strcpy(result, game.player1Name);
                strcat(result, " Won");
                break;
            case 2: 
                strcpy(result, game.player2Name);
                strcat(result, " Won");
                break;
        }

        // char dateTime[] = ctime(&game.startTime);
        char dateTime[25];
        strcpy(dateTime, ctime(&game.startTime));
        dateTime[strlen(dateTime) - 1] = '\0';

        fprintf(gameHistory, "Date: [%s] | Match: [%s] | GameMode: [%s] | Player 1: [%s] | Player 2: [%s] | GameBoard: [%s] | TotalMoves: [%d] | Duration: [%.1f min] | Result: [%s]\n", 
                        dateTime, match, game.gameMode, game.player1Name, game.player2Name, gameBoard, game.totalMoves, difftime(game.endTime, game.startTime)/60.0, result);
        
        fclose(gameHistory);
    }
}
void saveGameBoardHistory()             // runs after every playerMove
{
    /*
        stores the following
            >   player names
            >   gameBoard row, cols
            >   gameMoves in order
            >   gameBoard at end (terminal state)
            >   winning indices
            
        is called at the end of every playerMove
    */

    FILE *gameBoardHistory = fopen("gameFiles/gameBoardHistory.txt", "a");
    if (gameBoardHistory == NULL){ printf("[!] ERROR: Couldn't access file 'gameBoardHistory.txt'"); }
    else
    {
        if (game.totalMoves == 1)           // writes the following after the first move has been made
        {    
            fprintf(gameBoardHistory, "Player1: [%s] | Player2: [%s] | rowCount: [%d] | colCount: [%d] | gameMoves: [",
                                        game.player1Name, game.player2Name, game.rowCount, game.colCount);
        }

        fprintf(gameBoardHistory, "%d", game.playerMove);     // writes the player's move (index of chosen column) in the file after gameMoves: [
        
        if (game.gameState != -1)           // a terminal state has been reached (draw/win/loss)
        {    
            int index = 0;
            char winningIndices[9];
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    winningIndices[index] = '0' + game.winningIndices[i][j];      // int to char; chars are actually just numbers under the hood (but their value starts from 48 ('0' = 48))
                    index++;
                }
            }
            winningIndices[8] = '\0';

            index = 0;
            char gameBoard[game.rowCount*game.colCount + 1];            // +1 for '\0'
            for (int i = 0; i < game.rowCount; i++)
            {
                for (int j = 0; j < game.colCount; j++)
                {
                    gameBoard[index] = game.gameBoard[i][j];
                    index++;
                }
            }
            gameBoard[game.rowCount*game.colCount] = '\0';          // couldve used index as index

            fprintf(gameBoardHistory, "] | gameBoard: [%s] | winningIndices: [%s]\n", 
                                        gameBoard, winningIndices);
        }

        fclose(gameBoardHistory);
    }
}
void checkForUnfinishedGame()
{
    /*
        > will run at the start of every gameRound/match
        > counts numOfRecords in the gameHistory.txt file
        > if no unfinished games, count_gameHistory == count_gameBoardHistory
        > adds all records from the gameBoardHistory.txt file to temp.txt
            (until count_gameHistory != count_gameBoardHistory)
        > adds extra record (unfinished game) to resumeGame.txt
        > deletes gameBoardHistory.txt
        > renames temp.txt to gameBoardHistory.txt
        > if count_gameHistory == count_gameBoardHistory
            temp.txt & the deleted gameBoardHistory.txt would be the same
        > so, therefore, gameBoardHistory will contain all finished/completed games
            & resumeGame.txt will have all of the unfinished games
    */

    int count_gameHistory = 0;
    char line[350];                                 // arbitrarily-sized buffer

    // gameHistory.txt
    FILE *gameHistory = fopen("gameFiles/gameHistory.txt", "a+");        // will create file if it no exist
    if (gameHistory == NULL)
    { 
        printf("[!] ERROR: Couldn't access file 'gameFiles/gameHistory.txt'"); 
        pressEnterToContinue();
        return;
    }
    else
    {
        // counting the numOfRecords in the gameHistory.txt file
        fseek(gameHistory, 0, SEEK_SET);            // rewind/move-back file pointer to start of file

        while (true)
        {
            if (fgets(line, sizeof(line), gameHistory) == NULL){ break; }           // reached EOF
            else                                               { count_gameHistory++; }
        }

        fclose(gameHistory);
    }

    
    // moving every record in the gameBoardHistory.txt file into a temp file & then renaming it
    // moving any extra record (pending/unfinished game) into the resumeGame.txt file

    // gameBoardHistory.txt
    FILE *gameBoardHistory = fopen("gameFiles/gameBoardHistory.txt", "a+");        // will create file if it no exist
    if (gameBoardHistory == NULL)
    { 
        printf("[!] ERROR: Couldn't access file 'gameFiles/gameBoardHistory.txt'"); 
        pressEnterToContinue();
        return;
    }
    // temp.txt file
    FILE *temp = fopen("gameFiles/temp.txt", "w");        
    if (temp == NULL)
    { 
        printf("[!] ERROR: Couldn't access file 'gameFiles/temp.txt'");
        fclose(gameBoardHistory);           // closing gameBoardHistory.txt which wouldve already opened successfully if the code reached here
        pressEnterToContinue();
        return;
    }
    // resumeGame.txt
    FILE *resumeGame = fopen("gameFiles/resumeGame.txt", "a+");        // will create file if it no exist
    if (resumeGame == NULL)
    { 
        printf("[!] ERROR: Couldn't access file 'gameFiles/resumeGame.txt'"); 
        fclose(gameBoardHistory);           // closing gameBoardHistory.txt which wouldve already opened successfully if the code reached here
        fclose(temp);               
        pressEnterToContinue();
        return;
    }


    // will only reach here if the above 3 files opened successfully

    fseek(gameBoardHistory, 0, SEEK_SET);            // rewind/move-back file pointer to start of the gameBoardHistory.txt file

    int count_gameBoardHistory = 0;
    while (true)
    {
        if (fgets(line, sizeof(line), gameBoardHistory) == NULL){ break; }                   // reached EOF
        else                                                    
        { 
            count_gameBoardHistory++; 
            if (count_gameBoardHistory > count_gameHistory)
            {
                fputs(line, resumeGame);            // puts the extra, unfinished gameRecord into the resumeGame.txt file
                break;              // breaks from the while loop
            }
            else { fputs(line, temp); }         // puts the read record/line from the gameBoardHistory.txt to the temp.txt
        }
    }

    // temp.txt will now have all the records of the finished games;  resumeGame.txt will have 1 more or none of the unfinished game records

    fclose(gameBoardHistory);                                           // closing gameBoardHistory.txt
    remove("gameFiles/gameBoardHistory.txt");                           // deleting file as no use now
    fclose(temp);                                                       // closing temp.txt
    rename("gameFiles/temp.txt", "gameFiles/gameBoardHistory.txt");     // renaming the temp.txt to gameBoardHistory.txt (mwahahahaha)
    fclose(resumeGame);                                                 // closing resumeGame.txt
    
    pressEnterToContinue();
}
void resumeGame();
void _saveGameHistory()
{
    // might have to seperate the following 2 funcs


    // for gameBoard layout/state/condition at end
    saveGameBoardHistory();

    // for games details: time, gameMode, matchType, players, serial number, winner, elapsed time
    if (game.gameState != -1) { saveGameDetails(); }        // will only run at the end of a game
}

int displayGameDetails()
{
    // make this return a value; either gameNum or 0 to exit and then call respective functions

    printConnectFourTitle();

    // FILE *fPtr = fopen("gameFiles/gameHistory.txt", "r");
    FILE *gameHistory = fopen("gameFiles/gameHistory.txt", "a+");        // used a+ (read and append; creates file if not existing) instead of r so that when the user opens the program for the 1st time and opens history, he wont see an error msg due to the gameFiles/gameHistory.txt file not exisitng; rather the file will be created 
    if (gameHistory == NULL)
    { 
        printf("[!] ERROR: Couldn't access file 'gameFiles/gameHistory.txt'"); 
        pressEnterToContinue();
        return 0;
    }
    else        // gameHistory.txt is accessible
    {
        char dateTime[arbitrarySize], match[7], gameMode[6], player1Name[arbitrarySize], player2Name[arbitrarySize], gameBoard[8], result[arbitrarySize + 5];    // result max size = arbitrarySize (25 bytes) + 4 bytes (" Won") + 1 null terminater 
        int numOfScans, totalMoves, count = 0;
        float duration;

        animateText("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n", animateTextDelay_13ms);
        printf("| %5s | %-27s | %10s | %10s | %-25s | %-25s | %10s | %11s | %10s | %-29s |\n", "Game", "dateTime", "Match", "gameMode", "player1Name", "player2Name", "gameBoard", "totalMoves", "Duration", "Result");
        animateText("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------", animateTextDelay_13ms);
        while (true)
        {
            numOfScans = fscanf(gameHistory, "Date: [%[^]]] | Match: [%[^]]] | GameMode: [%[^]]] | Player 1: [%[^]]] | Player 2: [%[^]]] | GameBoard: [%[^]]] | TotalMoves: [%d] | Duration: [%f min] | Result: [%[^]]]\n",        // used a scanset: %[^]] (read everything until the 1st instance of ])
                                        dateTime, match, gameMode, player1Name, player2Name, gameBoard, &totalMoves, &duration, result);
            if (numOfScans != 9) { break; }    // no items read (ie max num of lines (EOF) reached)
            else
            {
                printf("\n\n| %5d | %-27s | %10s | %10s | %-25s | %-25s | %10s | %11d | %5.1f mins | %-29s |", ++count, dateTime, match, gameMode, player1Name, player2Name, gameBoard, totalMoves, duration, result);              // intentionally am skipping a line (table looks better like this it)
            }
        }
        if (count == 0){ printf("%140s\n", "(No Available Data)"); }
        else           { printf("\n\n"); }        // formatting kay keeray
        animateText("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n", animateTextDelay_13ms);

        fclose(gameHistory);

        printf("\n\n\n");
        if (count == 0)     // gameHistory are empty so no gameBoard to view as well
        {
            animateText("Is no one playing my game?!\n", animateTextDelay_63ms);                  // :>
            wait(1000);         // a little pause
            pressEnterToContinue();
            return 0;
        }
        else        // atleast one entry in the gameHistory.txt (& so in gameBoardHistory.txt)
        {
            wait(2000);         // a little pause before printing menu
            animateText("[Game History]\n   > Enter Game Number to view its gameBoard\n   > Enter 0 to return to the Main Menu", animateTextDelay_33ms);
        
            char userInput[arbitrarySize];
            while (true)
            {
                printf("\nEnter your choice [0-%d]: ", count);
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
                            printf("> [!] Enter a number!");
                            isNum = false;
                        }
                    }
                    
                    if (isNum)              // the userInput contains only numbers
                    {
                        int numUserInput;
                        sscanf(userInput, "%d", &numUserInput);             // the string userInput in extracted to numUserInput

                        if (numUserInput == 0){ return 0; }                               // user entered 0;  returning to mainMenu()
                        else if ((numUserInput < 0) || (numUserInput > count))            // userInput was not in range [1, count]
                        {
                            printf("> [!] Enter a number in range [1, %d]", count);
                            continue;
                        }
                        else
                        { return numUserInput; }
                    }
                }
            }
        }
    }
}
void displayGameBoardHistory(int gameNum, int mode)
{
    printConnectFourTitle();

    FILE *gameBoardHistory = fopen("gameFiles/gameBoardHistory.txt", "r");          // no need to open in a+ mode since the function itself will only be called if there is atleast 1 record in gameHistory.txt so gameBoardHistory.txt would also be existent
    if (gameBoardHistory == NULL){ printf("[!] ERROR: Couldn't access file 'gameBoardHistory.txt'"); }
    else
    {
        int count = 0, rowCount, colCount;
        char player1[arbitrarySize], player2[arbitrarySize], fGameMoves[maxRows*maxCols + 1], fGameBoard[maxRows*maxCols + 1], fWinningIndices[9];       // f for file

        while (true)            // parsing through records in the gameBoardHistory.txt file until we find the record for the required game (gameNum)
        {
            count++;
            fscanf(gameBoardHistory, "Player1: [%[^]]] | Player2: [%[^]]] | rowCount: [%d] | colCount: [%d] | gameMoves: [[^]]] | gameBoard: [%[^]]] | winningIndices: [%[^]]]\n",         // no need to check return value
                                        player1, player2, &rowCount, &colCount, fGameMoves, fGameBoard, fWinningIndices);
            if (count == gameNum)       // if the record for the required gameNum was reached
            {
                char playerDetails[123];            // 35 + 35 (max player string size) + 22 + 22 (other chars around %s)  = 114    --->   taking 123 cuz me like that num
                sprintf(playerDetails,"%s: \033[1;33mX\033[0m\n%s: \033[1;34mO\033[0m\n", player1, player2);
                animateText(playerDetails, animateTextDelay_13ms);

                // recreating 2d gameboard array
                char gameBoard[rowCount][colCount];
                int index = 0;
                for (int row = 0; row < rowCount; row++)
                {
                    for (int col = 0; col < colCount; col++)
                    {
                        gameBoard[row][col] = fGameBoard[index];
                        index++;
                    }
                }

                // recreating winningIndices
                int winningIndices[4][2] = {{-1,-1}, {-1,-1}, {-1,-1}, {-1,-1}};        // initializing with -1s
                index = 0;                                                              // reusing index from above
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 2; j++)
                    {
                        winningIndices[i][j] = fWinningIndices[index] - '0';            // reversal of the process in saveGameBoardLayout()
                        index++;
                    }
                }

                // printing the gameBoard...
                int numOfDashes = (6 * colCount) + 1;    // calculating the num of dashes required for rows
                char rowDashes[numOfDashes + 1];
                for (int i = 0; i <  numOfDashes; i++){ rowDashes[i] = '-'; }
                rowDashes[numOfDashes] = '\0';

                printf("\n  %s\n", rowDashes);    // topmost row of dashes
                for (int i = 0; i < rowCount; i++)
                {
                    for (int j = 0; j < colCount; j++)
                    {
                        bool isWinningIndex = false;
                        if (winningIndices[0][0] != -1)     // goes inside if the game wasnt draw (if it was, winningIndices would still have all -1)  
                        {
                            for (int k = 0; k < 4; k++)
                            {
                                if ((winningIndices[k][0] == i) && (winningIndices[k][1] == j)){   // [k][0] has the i value of the winning position's numOfAvailableColumns & [k][1] the j value
                                    isWinningIndex = true;
                                    break;
                                }
                            }
                        }

                        printf("  |  ");
                        if (isWinningIndex){
                            if   (gameBoard[i][j] == player1Mark){ printf("\033[1;3;4;33;40m%c\033[0m", gameBoard[i][j]); }     // bold, italic yellow color with grey highlighting for player1 (winner)
                        else                                     { printf("\033[1;3;4;34;40m%c\033[0m", gameBoard[i][j]); }     // bold, italic blue color with grey highlighting for player2 (winner)
                        }
                        else
                        {
                            if      (gameBoard[i][j] == player1Mark){ printf("\033[1;33m%c\033[0m", gameBoard[i][j]); }     // bold yellow color for player1
                            else if (gameBoard[i][j] == player2Mark){ printf("\033[1;34m%c\033[0m", gameBoard[i][j]); }     // bold blue color for player2
                            else                                    { printf("%c",                  gameBoard[i][j]); }     // no color formatting if empty char
                        }
                    }
                    printf("  |\n  %s\n", rowDashes);   // a row of dashes after every completed row
                }
                
                // for printing column numbers
                printf("  ");    // empty space of width 2 (is fixed)
                for (int i = 0; i < colCount; i++)
                {
                    printf("  [%d] ", i+1);
                }

                printf("\n");

                break;
            }
        }

        fclose(gameBoardHistory);
    }

    pressEnterToContinue();
}
void displayGameHistory()
{
    int returnVal;
    while (true)                                                    // did this to prevent a infinite recursive loop
    {
        returnVal = displayGameDetails();   
        if   (returnVal == 0){ return; }                            // breaks from the whole function and goes back to mainMenu()
        else                 { displayGameBoardHistory(returnVal , 1); }
    }
}

// help
void displayHelp()
{
    printConnectFourTitle();

    animateText("[Help]\n  [1] Brief\n  [2] Detailed", animateTextDelay_33ms);
    char userChoice[arbitrarySize];
    while (true) 
    {
        printf("\nEnter your choice [1/2]: ");
        fgets(userChoice, sizeof(userChoice), stdin);
        if ((strlen(userChoice) == 2) && ((userChoice[0] == '1') || userChoice[0] == '2'))
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

    printConnectFourTitle();

    switch(userChoice[0])
    {
        case '1':       // brief help
            FILE *briefHelp = fopen("gameFiles/briefHelp.txt", "r");
            if (briefHelp == NULL){ printf("[!] ERROR: Couldn't access file 'briefHelp.txt'"); }
            else
            {
                char line[199];
                while (true)
                {
                    if (fgets(line, sizeof(line), briefHelp) == NULL){ break; }     // eof reached
                    animateText(line, animateTextDelay_13ms);       // prints/animates the line of help on screen
                }

                fclose(briefHelp);
            }
            break;

        case '2':       // detailed help  
            FILE *detailedHelp = fopen("gameFiles/detailedHelp.txt", "r");
            if (detailedHelp == NULL){ printf("[!] ERROR: Couldn't access file 'detailedHelp.txt'"); }
            else
            {
                char line[199];
                while (true)
                {
                    if (fgets(line, sizeof(line), detailedHelp) == NULL){ break; }     // eof reached
                    animateText(line, animateTextDelay_13ms);       // prints/animates the line of help on screen
                }

                fclose(detailedHelp);
            }
            break;
    }

    pressEnterToContinue();
}
