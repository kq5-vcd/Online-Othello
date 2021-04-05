class Board {
    static const int DIR[8][2];
private:
    int rows = 8; 
    int cols = 8;
    int board[8][8];
public:
    Board();

    void newBoard();
    void setPiece(int player, int x, int y);
};
