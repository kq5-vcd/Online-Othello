#include "Room.hpp"

Room::Room(bool notification):game(notification){
    this->numPlayer = 1;
}

void Room::newGame(){
    this->game = Game(false);
    
}

Game& Room::getGame(){
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

void Room::addPlayer(int socket, int turn){
    Players *p = new Players();
    p->setSocket(socket);
    p->setTurn(turn);
    this->players.push_back(*p);
}

vector<Players> Room::getPlayers(){
    return this->players;
}