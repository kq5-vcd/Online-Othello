#include "Players.hpp"

Players::Players(){
    socket = 0;
    turn = 0;
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