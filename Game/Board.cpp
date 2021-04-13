#include <iostream>
#include "Board.hpp"

using namespace std;

const int Board::DIR[8][2] = {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

Board::Board(int rows = 8, int cols = 8): rows(rows), cols(cols) {
    Board::newBoard();
}

void Board::setPiece(int player, int x, int y) {
    board[x][y] = player;
}

void Board::initPieces() {
    int start_x = rows / 2 - 1;
    int start_y = cols / 2 - 1;

    Board::setPiece(1, start_x, start_y);
    Board::setPiece(1, start_x + 1, start_y + 1);
    Board::setPiece(2, start_x + 1, start_y);
    Board::setPiece(2, start_x, start_y + 1);
}

void Board::newBoard() {
    board.resize(rows, vector<int>(cols));
    for (auto &i : board) fill(i.begin(), i.end(), 0);

    Board::initPieces();
}

void Board::setAvailableMoves(int player) {
    for(int x = 0; x < rows; x++) {
        for(int y = 0; y < cols; y++) {
            if(board[x][y] == -1) {
                board[x][y] = 0;
            }

            if(board[x][y] == 0) {
                for(int dir = 0; dir < 8; dir++) {
                    int i = Board::DIR[dir][0];
                    int j = Board::DIR[dir][1];

                    if(validDirection(player, x, y, i, j)) {
                        board[x][y] = -1;
                    }
                }
            }
        }
    }
}

void Board::makeMove(int player, int x, int y) {
    setPiece(player, x, y);

    for(int dir = 0; dir < 8; dir++) {
        int i = Board::DIR[dir][0];
        int j = Board::DIR[dir][1];

        if(validDirection(player, x, y, i, j)) {
            int dir_x = x + i;
            int dir_y = y + j;

            while(board[dir_x][dir_y] != player) {
                setPiece(player, dir_x, dir_y);
                
                dir_x += i;
                dir_y += j;
            } 
        }
    }
}

void Board::printBoard() {
    for(int x = 0; x < rows; x++) {
        for(int y = 0; y < cols; y++) {
            if(board[x][y] != -1) cout << " ";
            cout << board[x][y] << " ";
        }
        cout << endl;
    }

    cout << endl;
}

int* Board::getScores() {
    int scores[2] = {0, 0};

    for(int x = 0; x < rows; x++) {
        for(int y = 0; y < cols; y++) {
            if(board[x][y] == 1) scores[0]++;
            else if(board[x][y] == 2) scores[1]++;
        }
    }

    return scores;
}

bool Board::isOnBoard(int x, int y) {
    return x >= 0 and x < rows and y >= 0 and y < cols;
}

bool Board::validDirection(int player, int x, int y, int dir_x, int dir_y) {
    int i = x + dir_x;
    int j = y + dir_y;
    int gain = 0;

    if(!Board::isOnBoard(i, j)) return false;

    while(board[i][j] == 3 - player) {
        gain++;
        i += dir_x;
        j += dir_y;

        if(!Board::isOnBoard(i, j)) return false;
    }

    if(gain > 0 and board[i][j] == player) {
        return true;
    }

    return false;
}

bool Board::validateMove(int player, int x, int y) {
    if(!Board::isOnBoard(x, y)) {
        cout << "INPUT OUT OF RANGE\n";
        return false;
    }

    if(board[x][y] > 0) {
        cout << "SLOT OCCUPIED\n";
        return false;
    }

    if(board[x][y] == -1) return true;

    cout << "NO PIECE TO TAKE\n";
    return false;
}

bool Board::playable() {
    for(int x = 0; x < rows; x++) {
        for(int y = 0; y < cols; y++) {
            if(board[x][y] == -1) return true;
        }
    }

    return false;
}