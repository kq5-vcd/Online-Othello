#include "Game.hpp"
#include "MiniMax.hpp"

void play(Bot& bot) {
    Game game = Game(true);

    //game.testing();

    bool isOn = !game.gameOver();

    while(isOn) {
        int turn = game.getStatus()[66];

        vector<int> move;

        if(turn == 1) {
            game.printStatus();
            move = game.getMove();
        } else if(turn == 2) {
            //move = randy.makeMove(game.getBoard());
            move = bot.makeMove(game.getBoard());
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
}

int main() {
    int difficulty = 3;
    
    switch(difficulty) {
        case 1: {
            Bot bot = Bot();
            play(bot);
            break;
            }
        case 2: {
            MiniMax bot = MiniMax(2, 1);
            play(bot);
            break;
            }
        case 3: {
            MiniMax bot = MiniMax(2, 3);
            play(bot);
            break;
            }
        case 4: {
            MiniMax bot = MiniMax(2, 3);
            play(bot);
            break;
            }
    }
    
    return 0;
}