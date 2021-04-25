import Bot

class Gordon(Bot):
    VAL = [[120, -20, 20,  5,  5, 20, -20, 120],
           [-20, -40, -5, -5, -5, -5, -40, -20],
           [ 20,  -5, 15,  3,  3, 15,  -5,  20],
           [  5,  -5,  3,  3,  3,  3,  -5,   5],
           [  5,  -5,  3,  3,  3,  3,  -5,   5],
           [ 20,  -5, 15,  3,  3, 15,  -5,  20],
           [-20, -40, -5, -5, -5, -5, -40, -20],
           [120, -20, 20,  5,  5, 20, -20, 120]]


    def __init__(self, turn):
        super().__init__(turn)


    def make_move(self, board):
        moves = self.get_available_moves(board)

        reward = [self.VAL[move[0]][move[1]] for move in moves]

        return moves[reward.index(max(reward))]