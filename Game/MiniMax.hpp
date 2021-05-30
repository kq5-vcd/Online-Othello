#include "Bot.hpp"
#include "Board.hpp"

class MiniMax: public Bot {
    static const int REWARD[8][8];
    static const int MIN_EVAL;
    static const int MAX_EVAL;

private:
    int turn;
    int depth;

    int evalBoard(vector<vector<int>> board);
    vector<vector<int>> getAvailableMoves(vector<vector<int>> board, int player);
    int minimax(vector<vector<int>> board, int depth, int player);

public:
    MiniMax(int turn, int depth);

    vector<int> makeMove(vector<vector<int>> board) override;
};