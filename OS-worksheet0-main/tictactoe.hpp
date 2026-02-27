#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP

#include <iostream>

using namespace std;

const int width = 3;
const int length = 3;

class TicTacToe {
public:
    TicTacToe();
    void setup();
    void printBoard();
    int matchColumn();
    int matchRow();
    int matchLeftDiagonal();
    int matchRightDiagonal();
    int check();
    void playerChoice(int* player);
    void intro();
    void loop();

private:
    int board[width][length];
};

#endif // TICTACTOE_HPP