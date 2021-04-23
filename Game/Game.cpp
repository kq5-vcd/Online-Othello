#include <iostream>
#include <string>
#include "Game.hpp"

using namespace std;

Game::Game(bool notification, int rows, int cols): 
   board(notification, rows, cols) { //
   turn = 2;
   endCounter = 0;

   newTurn();
}

void Game::newTurn() {
   turn = 3 - turn;
   board.setAvailableMoves(turn);
}

void Game::printStatus() {
   cout << "Current player: " << turn << endl;

   vector<int>scores = board.getScores();
   cout << "x: " << scores[0] << "\ty: " << scores[1] << endl;

   board.printBoard();
}

void Game::makeMove(int x, int y) {
   board.makeMove(turn, x, y);
   newTurn();
}

void Game::checkStatus() {
   if(board.playable()) {
      endCounter = 0;
   } else {
      endCounter++;
      newTurn();
   }
}

void Game::declareResult() {
   vector<int> scores = board.getScores();
   int x = scores[0];
   int y = scores[1];

   printStatus();

   if(x > y) {
      cout << "Player 1 is the winner!";
   } else if(x < y) {
      cout << "Player 1 is the winner!";
   } else {
      cout << "It's a tie!";
   }

   cout << endl;
}

void Game::testing() {
   board.easyWin(turn);
   endCounter = 2;
}

vector<int> Game::getMove() {
   int x, y;
   string input = "";

   cout << "Select row: ";
   cin >> input;

   x = checkNumber(input) ? stoi(input) : -1;

   cout << "Select column: ";
   cin >> input;

   y = checkNumber(input) ? stoi(input) : -1;

   vector<int> currentMove = {x, y};

   return currentMove;
}

vector<int> Game::getScores() {
   return board.getScores();
}

vector<int> Game::getStatus() {
   vector<int> status;

   vector<vector<int>> tempBoard = board.getBoard();
   for (auto &i : tempBoard) status.insert(status.end(), i.begin(), i.end());

   vector<int> scores = board.getScores();
   status.insert(status.end(), scores.begin(), scores.end());

   status.push_back(turn);

   return status;
}

bool Game::checkNumber(string str) {
   for (int i = 0; i < int(str.length()); i++) if (isdigit(str[i]) == false) return false;
   return true;
}

bool Game::validateInput(int x, int y) {
   return board.validateMove(turn, x, y);
}

bool Game::gameOver() {
   if(endCounter > 1) {
      turn = -1;
      return true;
   }

   return false;
}