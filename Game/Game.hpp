#include "Board.hpp"
#include <string>

class Game {
private:
    Board board;
    int turn;
    int endCounter;
    int currentMove[2];
    void newTurn();

    bool checkNumber(string str);

public:
    Game(int rows, int cols);

    string getStatus();
    void makeMove(int x, int y);
    void checkStatus();
    void declareResult();
    void testing();

    int* getMove();

    bool validateInput(int x, int y);
    bool gameOver();
};