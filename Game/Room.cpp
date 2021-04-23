#include "Room.hpp"

Room::Room(bool notification):
    game(notification){
        numPlayer = 1;
        id++;
}

Game Room::getGame(){
    return this->game;
}

int Room::getNumPlayer(){
    return this->numPlayer;
}

void Room::setNumPlayer(int num){
    this->numPlayer = num;
}

int Room::getId(){
    return this->id;
}