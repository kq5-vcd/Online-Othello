#include "Game.hpp"
#include "Players.hpp"

class Room{

private:
    int id;
    int numPlayer;
    vector<Players> players;
    Game game;
    
public:
    Room(bool notification);
    Game& getGame();
    int getNumPlayer();
    int getId();
    void setNumPlayer(int num);
    void setId(int id);
    void addPlayer(int socket, int turn, string name);
    void removePlayer(int index);
    void newGame();
    void setTurn(int index, int turn);
    vector<Players> getPlayers();
};