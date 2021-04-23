#include <vector>
#include <string>

using namespace std;
class Board {
    static const int DIR[8][2];

private:
    int rows, cols;
    int scores[2];
    vector<vector<int>> board;
    
    bool isOnBoard(int x, int y);
    bool validDirection(int player, int x, int y, int dir_x, int dir_y);

    void setPiece(int player, int x, int y);
    void initPieces();
    void fillBoard(int val);

public:
    Board(int rows, int cols);

    void newBoard();
    void setAvailableMoves(int player);
    void makeMove(int player, int x, int y);
    void easyWin(int player);

    int* getScores();
    string printBoard(); 

    bool validateMove(int player, int x, int y);
    bool playable();
};
