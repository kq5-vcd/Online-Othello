#include "Game.hpp"

class Room{
private:
    int numPlayer;
    Game game;

public:
    Room(bool notification);
    Game getGame();
    int getNumPlayer();
};