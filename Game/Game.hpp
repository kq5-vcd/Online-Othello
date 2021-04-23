#include "Board.hpp"
#include <string>

class Game {
private:
    Board board;
    int turn;
    int endCounter;

    void newTurn();

    bool checkNumber(string str);

public:
    Game(bool notification, int rows = 8, int cols = 8);

    void printStatus();
    void makeMove(int x, int y);
    void checkStatus();
    void declareResult();
    void testing();

    vector<int> getMove();
    vector<int> getScores();
    vector<int> getStatus();

    bool validateInput(int x, int y);
    bool gameOver();
};