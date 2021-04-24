#include "Game.hpp"

class Room{

private:
    int id;
    int numPlayer;
    Game game;
    
public:
    Room(bool notification);
    Game getGame();
    int getNumPlayer();
    void setNumPlayer(int num);
    int getId();
    void setId(int id);
};