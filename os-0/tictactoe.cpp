#include <stdio.h>
#include <iostream>
#include "tictactoe.hpp"
using namespace std;

int board [3][3] = {};

// const int width = 3;
// const int length = 3;

void setBoard(){
    for (int i = 0; i < width; i++)
    {
        for (int y = 0; y < length; y++)
        {
            board[i][y]=0;
        }
        
    }
    
}

void setup(){
    setBoard();
}

void printBoard(){
    for (int i = 0; i < width; i++)
    {
        for (int y = 0; y < length; y++)
        {
            switch (board[i][y])
            {
            case 0:
                printf("-");
                break;
            case 1:
                printf("X");
                break;
            case 2:
                printf("O");
                break;
            
            default:
                printf("-1");
                break;
            }
        }
        printf("\n");
    }
}

int matchColumn(){
    // check column and return either 0 or the player number
    // check column and return either 0 or the player number

    for (int toCheck = 0; toCheck <= 2; toCheck++)
    {
        // cout << "Checking Column " << toCheck << endl;
        // printf("Board[0][%d]:%d, Board[1][%d]:%d, Board[2][%d]:%d\n", toCheck, board[0][toCheck], toCheck, board[1][toCheck], toCheck, board[2][toCheck]);        
        if (board[0][toCheck] == 1 && board[1][toCheck] == 1 && board[2][toCheck] == 1)
        {
            return 1;
        }
        else if (board[0][toCheck] == 2 && board[1][toCheck] == 2 && board[2][toCheck] == 2)
        {
            return 2;
        }
        // else if (board[toCheck][0] == 0 && board[toCheck][1] == 0 && board[toCheck][2] == 0)
        // {
        //     cout << "Returning 0" << endl;
        //     return 0;
        // }
    }
    // cout <<"End of loop" << endl;
    return 0;
}
int matchRow()
{
    // check column and return either 0 or the player number

    for (int toCheck = 0; toCheck <= 2; toCheck++)
    {
        // cout << "Checking Row " << toCheck << endl;
        // printf("Board[%d][0]:%d, Board[%d][1]:%d, Board[%d][2]:%d\n", toCheck, board[toCheck][0], toCheck, board[toCheck][1], toCheck, board[toCheck][2]);        
        if (board[toCheck][0] == 1 && board[toCheck][1] == 1 && board[toCheck][2] == 1)
        {
            return 1;
        }
        else if (board[toCheck][0] == 2 && board[toCheck][1] == 2 && board[toCheck][2] == 2)
        {
            return 2;
        }
        // else if (board[toCheck][0] == 0 && board[toCheck][1] == 0 && board[toCheck][2] == 0)
        // {
        //     cout << "Returning 0" << endl;
        //     return 0;
        // }
    }
    // cout <<"End of loop" << endl;
    return 0;
}

int matchLeftDiagonal(){
    if (board[0][0] == 1 && board[1][1] == 1 && board[2][2] == 1)
    {
        return 1;
    }
    else if (board[0][0] == 2 && board[1][1] == 2 && board[2][2] == 2)
    {
        return 2;
    }
    return 0;
}

int matchRightDiagonal()
{
    if (board[0][2] == 1 && board[1][1] == 1 && board[2][0] == 1)
    {
        return 1;
    }
    else if (board[0][2] == 2 && board[1][1] == 2 && board[2][0] == 2)
    {
        return 2;
    }
    return 0;
}

int check(){
    int lastCheck = 0;
    lastCheck = matchLeftDiagonal();
    if (lastCheck != 0){
        return lastCheck;
    }
    lastCheck = matchRightDiagonal();
    if (lastCheck != 0)
    {
       return lastCheck;
    }
    lastCheck = matchRow();
    if (lastCheck != 0)
    {
        return lastCheck;
    }
    lastCheck = matchColumn();
    if (lastCheck != 0)
    {
        return lastCheck;
    }
}
void playerChoice(int* player){
    std::string s;
    // parse input
    cout << "Player " << *player << " enter your choice" << endl;
    cout << "Example: 12 denoting: second row, third column" << endl;
    cout << "Input: ";
    cin >> s;
    int x = s[0] - '0';
    int y = s[1] - '0';
    if (board[x][y] != 0)
    {
        cout << "Invalid choice\n##########" << endl;
        playerChoice(player);
    }
    else if (x > 3 || y > 3)
    {
        cout << "Invalid choice\n##########" << endl;
        playerChoice(player);
    }
    
    else
    {
        board[x][y] = *player;
        *player = *player == 1 ? 2 : 1;
    }
    return;
}

void intro(){
    cout << "Welcome to Tic Tac Toe" << endl;
    cout << "Player 1 is X" << endl;
    cout << "Player 2 is O" << endl;
    cout << "Player 1 starts first" << endl;
}

bool fullBoard(){
    for (short x = 0; x < 3; x++)
    {
        for (short y = 0; y < 3; y++)
        {
            if (board[x][y] == 0) return false;
        }   
    }
    return true;
}

void loop(){
    while (true)
    {
        int player = 1;
        playerChoice(&player);
        printBoard();
    
        if (check() != 0){
            cout << "Player " << check() << " wins" << endl;
            break;
        }
        if (fullBoard())
        {
            cout << "Draw" << endl;
            break;
        }
        playerChoice(&player);
        printBoard();
        if (check() != 0){
            cout << "Player " << check() << " wins" << endl;
            break;
        }
        // scan board for full match
        if (fullBoard())
        {
            cout << "Draw" << endl;
            break;
        }
    
    }
}


int testFillLeftDiag(){
    board[0][0] = 1;
    board[1][1] = 1;
    board[2][2] = 1;
    if (matchLeftDiagonal() != 1)
    {
        cout << "LeftDiag failed p1" << endl;
        return 1;
    }
    setBoard();
    board[0][0] = 2;
    board[1][1] = 2;
    board[2][2] = 2;
    if (matchLeftDiagonal() != 2)
    {
        cout << "LeftDiag failed p2" << endl;
        return 1;
    }
    cout << "LeftDiag passed" << endl;
    setBoard();
    return 0;
}
int testFillRightDiag(){
    // left but inverted
    board[0][2] = 1;
    board[1][1] = 1;
    board[2][0] = 1;
    if (matchRightDiagonal() != 1)
    {
        cout << "RightDiag failed p1" << endl;
        return 1;
    }
    setBoard();
    board[0][2] = 2;
    board[1][1] = 2;
    board[2][0] = 2;
    if (matchRightDiagonal() != 2)
    {
        cout << "RightDiag failed p2" << endl;
        return 1;
    }
    cout << "RightDiag passed" << endl;
    setBoard();
    return 0;
}

int testFillRow(){
    // try to fill rows
    for (int i = 0; i < 3; i++)
    {
        setBoard();
        board[0][i] = 1;
        board[1][i] = 1;
        board[2][i] = 1;
        if (matchColumn() != 1)
        {
            cout << "Row failed p1" << endl;
            printBoard();
            // return 1;
        }
        // reset
        setBoard();
        board[0][i] = 2;
        board[1][i] = 2;
        board[2][i] = 2;
        if (matchColumn() != 2)
        {
            cout << "Row failed p2" << endl;
            printBoard();
            // return 1;
        }
        setBoard();
        cout << "\n\n############\nRow passed\n###########" << endl;
        return 0;
    }
}

int testFillColumn(){
    // try to fill rows
    
    for (int i = 0; i < 3; i++)
    {
        setBoard();
        board[i][0] = 1;
        board[i][1] = 1;
        board[i][2] = 1;
        if (matchRow() != 1)
        {
            cout << "Column failed p1" << endl;
            printBoard();
            // return 1;
        }
        setBoard();
        board[i][0] = 2;
        board[i][1] = 2;
        board[i][2] = 2;
        if (matchRow() != 2)
        {
            cout << "Column failed p2" << endl;
            printBoard();
            // return 1;
        }
        setBoard();
        cout << "\n\n############\nColumn passed\n###########" << endl;
    }
    return 0;
}
bool test = false;
int main(){
    setup();
    printBoard();
    if (test){
        // testFillLeftDiag();
        // testFillRightDiag();
        testFillRow();
        testFillColumn();
        return 0;
    }
    intro();
    loop();

    return 0;
}