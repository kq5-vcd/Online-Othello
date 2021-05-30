#include "Players.hpp"

Players::Players(bool notification, int rows, int cols):
    game(notification, rows, cols){
    socket = 0;
    turn = 0;
    bot = Bot();
}

void Players::setSocket(int socket){
    this->socket = socket;
}

int Players::getSocket(){
    return this->socket;
}

void Players::setTurn(int turn){
    this->turn = turn;
}

int Players::getTurn(){
    return this->turn;
}

void Players::setName(string name){
    this->name = name;
}

string Players::getName(){
    return this->name;
}

void Players::setRank(int rank){
    this->rank = rank;
}

int Players::getRank(){
    return this->rank;
}

Game& Players::getGame(){
    return this->game;
}

Bot& Players::getBot(){
    return this->bot;
}

void Players::setBot(Bot bot){
    this->bot = bot;
}