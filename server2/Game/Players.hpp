#include <string>
using namespace std;
class Players{
private:
    int socket;
    int turn;
    int rank;
    string name;


public:
    Players();
    int getSocket();
    int getTurn();
    int getRank();
    string getName();
    void setSocket(int socket);
    void setTurn(int turn);
    void setName(string name);
    void setRank(int rank);
};