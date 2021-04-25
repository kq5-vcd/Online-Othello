class Board:
    DIR = [[0, 1], [1, 1], [1, 0], [1, -1], [0, -1], [-1, -1], [-1, 0], [-1, 1]]

    def __init__(self, rows, cols):
        self.rows = rows
        self.cols = cols
        self.new_board()


    def new_board(self):
        self.board = [[0] * self.cols for x in range(self.rows)]
        self.init_pieces()

    
    def set_board(self, matrix):
        self.board = matrix


    def set_piece(self, piece, x, y):
        self.board[x][y] = piece


    def init_pieces(self):
        start_x = self.rows // 2 - 1
        start_y = self.cols // 2 - 1

        self.set_piece(1, start_x, start_y)
        self.set_piece(1, start_x + 1, start_y + 1)
        self.set_piece(2, start_x + 1, start_y)
        self.set_piece(2, start_x, start_y + 1)

    
    def is_on_board(self, x, y):
        return x >= 0 and x < self.rows and y >= 0 and y < self.cols


    def valid_direction(self, player, x, y, dir_x, dir_y):
        i = x + dir_x
        j = y + dir_y
        gain = 0

        if not self.is_on_board(i, j):
            return False

        while self.board[i][j] == 3 - player:
            gain += 1
            i += dir_x
            j += dir_y

            if not self.is_on_board(i, j):
                return False

        if gain > 0 and self.board[i][j] == player:
            return True
        
        return False

    
    def set_available_moves(self, player):
        for x in range(self.rows):
            for y in range(self.cols):
                if self.board[x][y] == -1:
                    self.board[x][y] = 0

                if self.board[x][y] == 0:
                    for i, j in self.DIR:
                        if self.valid_direction(player, x, y, i, j):
                            self.board[x][y] = -1


    def validate_move(self, player, x, y):
        if not self.is_on_board(x, y):
            print("INPUT OUT OF RANGE")
            return False

        if self.board[x][y] > 0:
            print("SLOT OCCUPIED")
            return False

        if self.board[x][y] == -1:
            return True
        else:
            print("NO PIECE TO TAKE")
            return False

    
    def make_move(self, player, x, y):
        self.set_piece(player, x, y)

        for i, j in self.DIR:
            if self.valid_direction(player, x, y, i, j):
                dir_x = x + i
                dir_y = y + j

                while self.board[dir_x][dir_y] != player:
                    self.set_piece(player, dir_x, dir_y)

                    dir_x += i
                    dir_y += j


    def get_scores(self):
        scores = [0 , 0]

        for row in range(self.rows):
            for col in range(self.cols):
                if self.board[row][col] == 1:
                    scores[0] += 1
                elif self.board[row][col] == 2:
                    scores[1] += 1

        return scores


    def get_board(self):
        return self.board

    
    def playable(self):
        for x in range(self.rows):
            for y in range(self.cols):
                if self.board[x][y] == -1:
                    return True

        return False


    def print_board(self):
        for row in range(self.rows):
            for col in range(self.cols):
                print(self.board[row][col], end="\t")
            print()
        print()