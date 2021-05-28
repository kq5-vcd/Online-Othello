#include "Board.hpp"

class Bot {
private:
    int turn;
    
    vector<vector<int>> getAvailableMoves(Board board);
    

public:
    Bot(int turn);

    vector<int> makeMove(Board board);
};