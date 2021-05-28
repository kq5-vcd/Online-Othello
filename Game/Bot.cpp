#include "Bot.hpp"

Bot::Bot(int turn): turn(turn){}

vector<vector<int>> Bot::getAvailableMoves(Board board) {
    vector<vector<int>> temp_board = board.getBoard();
    vector<vector<int>> moves;

    for(int x = 0; x < temp_board.size(); x++) {
        for(int y = 0; y < temp_board[0].size(); y++) {
            if(temp_board[x][y] == -1) {
                vector<int> move = {x, y};
                moves.push_back(move);
            }
        }
    }

    return moves;
}

vector<int> Bot::makeMove(Board board) {
    vector<vector<int>> moves = getAvailableMoves(board);
    int choice = rand() % moves.size();

    return moves[choice];
}