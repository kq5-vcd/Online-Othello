#include "Room.hpp"

Room::Room(bool notification):
    game(notification){
        numPlayer = 1;
    
}

Game Room::getGame(){
    return this->game;
}

int Room::getNumPlayer(){
    return this->numPlayer;
}