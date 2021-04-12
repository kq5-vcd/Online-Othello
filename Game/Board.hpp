#include <vector>

class Board {
    static const int DIR[8][2];

private:
    int rows, cols;
    std::vector<std::vector<int>> board;
    
    bool isOnBoard(int x, int y);
    bool validDirection(int player, int x, int y, int dir_x, int dir_y);

    void setPiece(int player, int x, int y);
    void initPieces();

public:
    Board(int rows, int cols);

    void newBoard();
    void setAvailableMoves(int player);
    void makeMove(int player, int x, int y);

    int* getScores();
    void printBoard(); 

    bool validateMove(int player, int x, int y);
    bool playable();
};
