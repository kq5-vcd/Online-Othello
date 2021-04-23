#include "Game.hpp"

class Room{
private:
    static int id = 0;
    int numPlayer;
    Game game;

public:
    Room(bool notification);
    Game getGame();
    int getNumPlayer();
    void setNumPlayer(int num);
    int getId();
};