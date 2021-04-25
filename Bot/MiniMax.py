import Gordon
import random

class MiniMax(Gordon):
    min_eval_board = -1776
    max_eval_board = 1776

    def __init__(self, turn, depth):
        super().__init__(turn)
        self.depth = depth


    def eval_board(self, board, turn):
        temp_board = board.get_board()
        total_reward = 0

        for row in range(board.rows):
            for col in range(board.cols):
                if temp_board[row][col] == self.turn:
                    total_reward += self.VAL[row][col]
                if temp_board[row][col] == 3 - self.turn:
                    total_reward -= self.VAL[row][col]

        return total_reward


    def sort_nodes(self, moves):
        sorted_nodes = [[move, self.VAL[move[0]][move[1]]] for move in moves]

        sorted_nodes = sorted(sorted_nodes, key = lambda node: node[1], reverse = True)
        
        return [node[0] for node in sorted_nodes]


    def get_available_moves(self, board, player):
        board.set_available_moves(player)

        return super().get_available_moves(board)

    
    def minimax_alpha_beta(self, board, depth, player, Alpha, Beta, turn):
        valid_moves = self.get_available_moves(board, player)
        best_value = 0

        if depth == 0 or len(valid_moves) == 0:
            return self.eval_board(board, turn)

        if player == turn:
            tiles = sort_nodes(valid_moves)
            best_value = self.min_eval_board

            for piece in tiles:
                board_temp = copy.deepcopy(board)

                board_temp.make_move(player, piece[0], piece[1]) 
                val = self.minimax_alpha_beta(board_temp, depth - 1, 3 - player, Alpha, Beta, turn)
                
                #print(val, "Alpha_Beta_sorted() val maximize_player")

                if val > best_value:
                    best_value = val

                if val > Alpha:
                    Alpha = val

                if Alpha >= Beta:
                    break #beta cut-off
                
            #print(Alpha, "Alpha_Beta_sorted() Alpha maximize_player")
        else: # minimizingPlayer
            tiles = sort_nodes(valid_moves)
            best_value = self.max_eval_board

            for piece in tiles:
                board_temp = copy.deepcopy(board)

                board_temp.make_move(player, piece[0], piece[1])  
                val = self.minimax_alpha_beta(board_temp, depth - 1, 3 - player, Alpha, Beta, turn)
                
                #print(val, "Alpha_Beta_sorted() val minimize_player")

                if val < best_value:
                    best_value = val
                
                if val < Beta:
                    Beta = val

                if Alpha >= Beta:
                    break #alpha cut-off

            #print(Beta, "Alpha_Beta_sorted() Beta minimize_player")
        
        #print(best_value, "Alpha_Beta_sorted()", depth)
        return best_value


    def get_move(self, board):
        moves = self.get_available_moves(board, self.turn)
        sorted_tiles = sort_nodes(moves)

        max_points = self.min_eval_board
        best_move = []

        for move in sorted_tiles:
            board_temp = copy.deepcopy(board)
            board_temp.make_move(self.turn, move[0], move[1]) 

            points = self.minimax_alpha_beta(board_temp, self.depth, 3 - self.turn, max_points, self.max_eval_board, self.turn)
                    
            if points > max_points:
                max_points = points
                best_move = move
            elif points == max_points:
                if random.randint(0, 1):
                    best_move = move

        #print(best_move, "alpha_beta_sorted_move()")
        return best_move