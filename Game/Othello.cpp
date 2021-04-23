#include "Game.hpp"

int main() {
    Game game = Game(true);

    //game.testing();

    bool isOn = !game.gameOver();

    while(isOn) {
        game.printStatus();
        vector<int> move = game.getMove();

        int x = move[0];
        int y = move[1];

        if(game.validateInput(x, y)) {
            game.makeMove(x, y);
            game.checkStatus();
        }

        for(int i: game.getStatus()) {
            cout << i << " ";
        }
        cout << endl;

        isOn = !game.gameOver();
    }

    game.declareResult();

    return 0;
}