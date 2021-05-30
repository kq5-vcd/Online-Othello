#ifndef BOARD
#define BOARD

#include <vector>
#include <iostream>

using namespace std;

class Board {
    static const int DIR[8][2];

private:
    int rows, cols;
    vector<vector<int>> board;
    bool notification;
    
    bool isOnBoard(int x, int y);
    bool validDirection(int player, int x, int y, int dir_x, int dir_y);

    void setPiece(int player, int x, int y);
    void initPieces();
    void fillBoard(int val);

public:
    Board(bool notification, int rows, int cols);
    Board(bool notification, vector<vector<int>> board);

    void newBoard();
    void setAvailableMoves(int player);
    void makeMove(int player, int x, int y);
    void easyWin(int player);

    vector<int> getScores();
    void printBoard(); 

    vector<vector<int>> getBoard();

    bool validateMove(int player, int x, int y);
    bool playable();
};

#endif