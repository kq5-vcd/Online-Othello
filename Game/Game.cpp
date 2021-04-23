#include <iostream>
#include <string>
#include "Game.hpp"


Game::Game(int rows = 8, int cols = 8): board(rows, cols) { //board = Board(rows, cols);
   turn = 2;
   endCounter = 0;

   newTurn();
}

void Game::newTurn() {
   turn = 3 - turn;
   board.setAvailableMoves(turn);
}

string Game::getStatus() {
   string output;
   output.append("Current player: ").append(to_string(turn)).append("\n"); 

   int* scores = board.getScores();
   output.append("x: ").append(to_string(scores[0])).append("\ty: ").append(to_string(scores[1])).append("\n");
   

   output.append(board.printBoard());
   return output;
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
   int* scores = board.getScores();
   int x = *scores;
   int y = *(scores + 1);

   getStatus();

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

int* Game::getMove() {
   int x, y;
   string inputX = "";
   string inputY = "";

   cout << "Select row: ";
   cin >> inputX;

   x = checkNumber(inputX) ? stoi(inputX) : -1;

   cout << "Select column: ";
   cin >> inputY;

   y = checkNumber(inputY) ? stoi(inputY) : -1;

   currentMove[0] = x;
   currentMove[1] = y;

   return currentMove;
}

bool Game::checkNumber(string str) {
   for (int i = 0; i < int(str.length()); i++) if (isdigit(str[i]) == false) return false;
   return true;
}

bool Game::validateInput(int x, int y) {
   return board.validateMove(turn, x, y);
}

bool Game::gameOver() {
   return endCounter > 1;
}
