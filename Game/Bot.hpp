#include <vector>
#include <stdlib.h>    
#include <time.h>   

using namespace std;

class Bot {
private:
    int turn;
    
    vector<vector<int>> getAvailableMoves(vector<vector<int>> board);
    

public:
    Bot(int turn);

    vector<int> makeMove(vector<vector<int>> board);
};