#include "Bot.hpp"

Bot::Bot(){}

vector<vector<int>> Bot::getAvailableMoves(vector<vector<int>> board) {
    vector<vector<int>> moves;

    for(int x = 0; x < board.size(); x++) {
        for(int y = 0; y < board[0].size(); y++) {
            if(board[x][y] == -1) {
                vector<int> move = {x, y};
                moves.push_back(move);
            }
        }
    }

    return moves;
}

vector<int> Bot::makeMove(vector<vector<int>> board) {
    vector<vector<int>> moves = getAvailableMoves(board);

    srand (time(NULL));
    int choice = rand() % moves.size();

    return moves[choice];
}