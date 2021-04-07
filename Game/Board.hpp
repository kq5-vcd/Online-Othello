class Board {
    static const int DIR[8][2];
private:
    int rows = 8; 
    int cols = 8;
    int board[8][8];
    
    bool isOnBoard();
    bool validDirection(int player, int x, int y, int dir_x, int dir_y);

    void setPiece(int player, int x, int y);
    void initPieces();

public:
    Board();

    void newBoard();
    void setAvailableMoves(int player);
    void makeMove(int player, int x, int y);

    int* getScores();
    void printBoard(); 

    bool validateMove(int player, int x, int y);
    bool playable();
};
