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

}

void Board::makeMove(int player, int x, int y) {

}

void Board::printBoard() {

}

int* Board::getScores() {

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
        
    }
}

bool Board::validateMove(int player, int x, int y) {

}

bool Board::playable() {

}