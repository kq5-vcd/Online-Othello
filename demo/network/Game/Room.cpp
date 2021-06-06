#include "Room.hpp"

Room::Room(bool notification):game(notification){
    this->numPlayer = 1;
    this->state = 1;
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

int Room::getState(){
    return this->state;
}

void Room::setState(int state){
    this->state = state;
}

void Room::addPlayer(int socket, int turn, string name){
    Players *p = new Players(false);
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

void Room::addSpectator(int socket, string name){
    Players *p = new Players(false);
    p->setSocket(socket);
    p->setName(name);
    this->spectators.push_back(*p);
}

void Room::removeSpectator(int socket){
    vector<Players>::iterator it;
    for(it = spectators.begin(); it != spectators.end(); it++){
        if(it->getSocket() == socket){
            this->spectators.erase(it);
            break;
        }
    }
}

void Room::removeAllSpectators(){
    this->spectators.clear();
}

void Room::setTurn(int index, int turn){
    this->players[index].setTurn(turn);
}

void Room::swapTurn(){
    int tmp = this->players[0].getTurn();
    this->players[0].setTurn(this->players[1].getTurn());
    this->players[1].setTurn(tmp);
}

vector<Players> Room::getPlayers(){
    return this->players;
}

vector<Players> Room::getSpectators(){
    return this->spectators;
}

bool Room::findSpectator(int socket){
    for(int i =0; i<this->spectators.size(); i++){
        if(this->spectators[i].getSocket() == socket){
            return true;
        }
    }
    return false;
}

