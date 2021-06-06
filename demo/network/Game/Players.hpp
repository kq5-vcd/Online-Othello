#include <string>
#include "Game.hpp"
#include "MiniMax.hpp"
using namespace std;
class Players{
private:
    int socket;
    int turn;
    int rank;
    string name;
    Game game;
    Bot bot;


public:
    Players(bool notification, int rows = 8, int cols = 8);
    int getSocket();
    int getTurn();
    int getRank();
    string getName();
    Game& getGame();
    Bot& getBot();
    void setSocket(int socket);
    void setTurn(int turn);
    void setName(string name);
    void setRank(int rank);
    void setBot(Bot& bot);
    void newGame();
};