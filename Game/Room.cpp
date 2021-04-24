#include "Room.hpp"

Room::Room(bool notification):
    game(notification){
        this->numPlayer = 1;
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

void Room::setId(int id){
    this->id = id;
}