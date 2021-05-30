#include "MiniMax.hpp"

const int MiniMax::REWARD[8][8] = {{120, -20, 20,  5,  5, 20, -20, 120},
                                   {-20, -40, -5, -5, -5, -5, -40, -20},
                                   { 20,  -5, 15,  3,  3, 15,  -5,  20},
                                   {  5,  -5,  3,  3,  3,  3,  -5,   5},
                                   {  5,  -5,  3,  3,  3,  3,  -5,   5},
                                   { 20,  -5, 15,  3,  3, 15,  -5,  20},
                                   {-20, -40, -5, -5, -5, -5, -40, -20},
                                   {120, -20, 20,  5,  5, 20, -20, 120}};
const int MiniMax::MIN_EVAL = -1776;
const int MiniMax::MAX_EVAL = 1776;

MiniMax::MiniMax(int turn, int depth): turn(turn), depth(depth) {}

int MiniMax::evalBoard(vector<vector<int>> board) {
    int totalReward = 0;

    for(int x = 0; x < board.size(); x++) {
        for(int y = 0; y < board[0].size(); y++) {
            if(board[x][y] == turn) {
                totalReward += REWARD[x][y];
            } else if(board[x][y] == 3 - turn) {
                totalReward -= REWARD[x][y];
            }
        }
    }

    return totalReward;
}

vector<vector<int>> MiniMax::getAvailableMoves(vector<vector<int>> board, int player) {
    Board temp_board = Board(false, board);
    temp_board.setAvailableMoves(turn);

    return Bot::getAvailableMoves(temp_board.getBoard());
}

int MiniMax::minimax(vector<vector<int>> board, int depth, int player) {
    vector<vector<int>> moves = getAvailableMoves(board, player);
    int bestValue;

    if(depth == 0 || moves.size() == 0) {
        return evalBoard(board);
    }

    if(player == turn) {
        bestValue = MIN_EVAL;

        for(int i = 0; i < moves.size(); i++) {
            Board temp_board = Board(false, board);

            temp_board.makeMove(player, moves[i][0], moves[0][1]);
            int val = minimax(temp_board.getBoard(), depth - 1, 3 - player);

            bestValue = val > bestValue ? val : bestValue;
        }
    } else {
        bestValue = MAX_EVAL;

        for(int i = 0; i < moves.size(); i++) {
            Board temp_board = Board(false, board);

            temp_board.makeMove(player, moves[i][0], moves[0][1]);
            int val = minimax(temp_board.getBoard(), depth - 1, 3 - player);

            bestValue = val < bestValue ? val : bestValue;
        }
    }

    return bestValue;
}

vector<int> MiniMax::makeMove(vector<vector<int>> board) {
    vector<vector<int>> moves = getAvailableMoves(board, turn);
    int maxPoints = MIN_EVAL;
    int choice;

    for(int i = 0; i < moves.size(); i++) {
        Board temp_board = Board(false, board);

        temp_board.makeMove(turn, moves[i][0], moves[0][1]);
        int val = minimax(temp_board.getBoard(), depth - 1, 3 - turn);

        if(val > maxPoints) {
            maxPoints = val;
            choice = i;
        }
    }

    //cout << "Hello!" << endl;
    return moves[choice];
}