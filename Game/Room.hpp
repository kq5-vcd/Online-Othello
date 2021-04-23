#include "Game.hpp"

class Room{

private:
    int numPlayer;
    Game game;

public:
    static int id;
    Room(bool notification);
    Game getGame();
    int getNumPlayer();
    void setNumPlayer(int num);
    int getId();
};