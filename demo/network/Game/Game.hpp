#include "Board.hpp"
#include <string>

class Game {
private:
    int turn;
    int endCounter;
    Board board;
    void newTurn();
    bool checkNumber(string str);
    

public:
    Game(bool notification, int rows = 8, int cols = 8);

    void printStatus();
    void makeMove(int x, int y);
    void checkStatus();
    void declareResult();
    void newGame();
    void testing();

    vector<vector<int>> getBoard();
    vector<int> getMove();
    vector<int> getScores();
    vector<int> getStatus();

    bool validateInput(int x, int y);
    bool gameOver();
};