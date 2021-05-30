#include <vector>
#include <stdlib.h>    
#include <time.h>   

using namespace std;

class Bot {
protected:
    vector<vector<int>> getAvailableMoves(vector<vector<int>> board);
    

public:
    Bot();

    vector<int> makeMove(vector<vector<int>> board);
};