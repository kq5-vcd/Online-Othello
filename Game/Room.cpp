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

void Room::addPlayer(int socket, int turn, string name){
    Players *p = new Players();
    p->setSocket(socket);
    p->setTurn(turn);
    p->setName(name);
    this->players.push_back(*p);
}

void Room::removePlayer(int index){
    if(index == 1){
        this->players.erase(this->players.begin());
    }
    else this->players.pop_back();
}

void Room::setTurn(int index, int turn){
    this->players[index].setTurn(turn);
}

vector<Players> Room::getPlayers(){
    return this->players;
}

