#include "Game.hpp"
#include "Bot.hpp"

int main() {
    Game game = Game(true);
    Bot randy = Bot();

    //game.testing();

    bool isOn = !game.gameOver();

    while(isOn) {
        int turn = game.getStatus()[66];

        vector<int> move;

        if(turn == 1) {
            game.printStatus();
            move = game.getMove();
        } else if(turn == 2) {
            move = randy.makeMove(game.getBoard());
        }

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