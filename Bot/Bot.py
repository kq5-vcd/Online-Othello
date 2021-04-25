import Board

class Bot:
    def __init__(self, turn):
        self.turn = turn


    def get_available_moves(self, board):
        temp_board = board.get_board()
        moves = []

        for row in range(self.board.rows):
            for col in range(self.board.cols):
                if temp_board[row][col] < 0:
                    moves.append([row, col])

        return moves


    def make_move(self, board):
        moves = self.get_available_moves(board)

        return moves[0]