import Bot
import random

class Randy(Bot):
    def __init__(self, turn):
        super().__init__(turn)


    def make_move(self, board):
        moves = self.get_available_moves(board)

        return moves[random.randint(0, len(moves) - 1)]