#include "Game.hpp"

int main() {
    Game game = Game(8, 8);

    //game.testing();

    bool isOn = !game.gameOver();

    while(isOn) {
        game.getStatus();
        int* move = game.getMove();

        int x = *move;
        int y = *(move + 1);

        if(game.validateInput(x, y)) {
            game.makeMove(x, y);
            game.checkStatus();
        }

        isOn = !game.gameOver();
    }

    game.declareResult();

    return 0;
}