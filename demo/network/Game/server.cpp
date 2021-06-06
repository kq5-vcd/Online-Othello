#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <map>
#include <iterator>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <cctype>
#include "Room.hpp"

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 8910 /*port*/
#define LISTENQ 20      /*maximum number of client connections*/

using namespace std;

//int id = 0;
int num_threads = 0;

pthread_mutex_t mut;

vector<Room> rooms ;
vector<Players> players;

//string map_to_string(map<int,Room>  &m);

vector<string> split(const string& str, const char& delimiter);
bool findStringIC(const string & strHaystack, const string & strNeedle);
void *connection_handler(void *client_socket);
void replaceAll(string& str, const string& from, const string& to);

int main(int argc, char **argv)
{
  //map<int,Room> rooms;
  
  
  int listenfd, connfd, n;
  pid_t childpid;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr, servaddr;
  //string m;

  //Create a socket for the soclet
  //If sockfd<0 there was an error in the creation of the socket
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Problem in creating the socket");
    exit(1);
  }

  //preparation of the socket address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  //bind the socket
  if(bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) !=0){
    cout<<"socket bind failed...\n"; 
    exit(0);
  }

  //listen to the socket by creating a connection queue, then wait for clients
  if(listen(listenfd, LISTENQ) !=0){
    cout<<"Listen failed...\n";
    exit(0);
  }

  cout<<"Server running...waiting for connections."<<endl;
  
  
  pthread_t threads[LISTENQ];

  if (pthread_mutex_init(&mut, NULL) != 0) {                                    
    perror("mutex_lock");                                                       
    exit(1);                                                                    
  }

  while (true){
		printf("Listening...\n");
		int client_socket = accept(listenfd, NULL, NULL);
    cout<<client_socket<<endl;
    if(num_threads == LISTENQ){
      send(client_socket,"0",1,0);
    }
    else{
      puts("Connection accepted");
      if( pthread_create( &threads[num_threads], NULL ,  connection_handler , &client_socket) < 0){
        perror("Could not create thread");
        return 1;
      }
      if (client_socket < 0) { 
        printf("server acccept failed...\n"); 
        exit(0); 
        } 
      else
        printf("Server acccept the client...\n");
      puts("Handler assigned");
      num_threads++;
    }
	}
	int k=0;
  for (k=0;k<LISTENQ;k++){
		pthread_join(threads[k],NULL);
	}

  close(listenfd);

  return 0;
}

// string map_to_string(map<int,Room> &m) {
//   string output = "";
//   string convrt = "";
//   string result = "";

// 	for (map<int,Room>::iterator it = m.begin(); it != m.end(); it++) {
//     if(it->second.getNumPlayer()>0){
//       convrt = to_string(it->first);
//       output.append(convrt).append(" ").append((to_string(it->second.getGame().getScores()[0]))).append(" ").append((to_string(it->second.getGame().getScores()[1]))).append(",");
//     }
//   }
	
// 	result = output.substr(0, output.size() - 1 );
	
//   return result;
// }

vector<string> split(const string& str, const char &delimiter)
{
    vector<string> cont;
    stringstream ss(str);
    string token;
    size_t pos = 0;
    
    while (getline(ss, token, delimiter)) {
        cont.push_back(token);
    }
    return cont;
}

void *connection_handler(void *client_socket){
	int socket = *(int*) client_socket;
  
	int n;
  string m = "";
	char buf[MAXLINE];
  Players *p = new Players(false);
  p->setName("");
  p->setSocket(socket);
  players.push_back(*p);

  while ((n = recv(socket, buf, MAXLINE, 0)) > 0)
  {
    if (pthread_mutex_lock(&mut) != 0) {                                          
      perror("mutex_lock");                                                       
      exit(2);                                                                    
    }
    
    cout<< "Socket:" << socket << endl;
    char b[n];
    for(int c = 0; c<=n; c++){
      b[c] = buf[c];
    }
    string receive(b);

    switch (receive[0]) {
      case '0': {
        cout << "In message 0" << endl;
        if(receive.length() == 1){
          vector<Players>::iterator i;
          for(i = players.begin(); i != players.end(); i++){
            if(i->getSocket() == socket){
              cout<<"Name: "<<i->getName()<<endl;
              if(i->getName().length() == 0){
                send(socket,"null",4,0);
              }
              else{
                send(socket,i->getName().c_str(),i->getName().length(),0);
              }
              break;
            }
          }
          
        }
        else if(split(receive,' ').size() > 1){
          string name = receive.substr(2,receive.size()-2);
        
          vector<Players>::iterator i;
          for(i = players.begin(); i != players.end(); i++){
            if(i->getName().compare(name) == 0 && i->getSocket() != socket){
              send(socket,"0",1,0);
              break;
            }
            if(i->getSocket() == socket){
              i->setName(name);
              send(socket,"1",1,0);
              break;
            }
          }
          if(i == players.end()){
            Players* p = new Players(false);
            p->setName(name);
            p->setSocket(socket);
            players.push_back(*p);
            send(socket,"1",1,0);
          }
        }
        break;
      }
      case '1': {
        cout << "In message 1" << endl;
        m.clear();

        if (!rooms.empty()) {
          if(receive.length() == 1){
            vector<Room>::iterator it;

            for (it = rooms.begin(); it != rooms.end(); it++) {
              if (it->getNumPlayer() > 0 && !it->getGame().gameOver()) {
                
                if (it->getNumPlayer() == 1) {
                  m.append(to_string(it->getId())).append(" ")
                  .append(it->getPlayers()[0].getName()).append(" ")
                  .append(to_string(it->getGame().getScores()[0])).append(" # ")
                  .append(to_string(it->getGame().getScores()[1]));
                } else {
                  m.append(to_string(it->getId())).append(" ")
                  .append(it->getPlayers()[0].getName()).append(" ")
                  .append(to_string(it->getGame().getScores()[0])).append(" ")
                  .append(it->getPlayers()[1].getName()).append(" ")
                  .append(to_string(it->getGame().getScores()[1]));
                }

                m.append(",");
                cout << "Send to client: " << m << endl;
              }
            }
            
            cout << "Send to client 2: " << m << endl;
            if (m.length() == 0) {
              send(socket, "empty", 5, 0);
            } else {
              send(socket, m.c_str(), m.length()-1, 0);
            }
          }
          else{
            string name = receive.substr(2,receive.size()-2);
            vector<Room>::iterator it;

            for (it = rooms.begin(); it != rooms.end(); it++) {
              if (it->getNumPlayer() > 0 && !it->getGame().gameOver()) {
                if(findStringIC(it->getPlayers()[0].getName(),name)){
                  if (it->getNumPlayer() == 1) {
                    m.append(to_string(it->getId())).append(" ")
                    .append(it->getPlayers()[0].getName()).append(" ")
                    .append(to_string(it->getGame().getScores()[0])).append(" # ")
                    .append(to_string(it->getGame().getScores()[1]));
                  } else {
                    m.append(to_string(it->getId())).append(" ")
                    .append(it->getPlayers()[0].getName()).append(" ")
                    .append(to_string(it->getGame().getScores()[0])).append(" ")
                    .append(it->getPlayers()[1].getName()).append(" ")
                    .append(to_string(it->getGame().getScores()[1]));
                  }
                  m.append(",");
                }
              }
            }
            cout << "Send to client: " << m << endl;
            if (m.length() == 0) {
              send(socket, "empty", 5, 0);
            } else {
              send(socket, m.c_str(), m.length()-1, 0);
            }
          }
          
        } else {
          send(socket, "empty", 5, 0);
        }
        break;
      } // refresh
        
      
      case '2': {
        // create new game
        cout << "In message 2" << endl;
        Room *room = new Room(false);
        string player1 = split(receive, ' ')[1];
        room->setNumPlayer(1);
        room->addPlayer(socket, 1, player1);
        
        if (rooms.empty()) {
          room->setId(1);
        } else {
          vector<Room>::iterator it = rooms.end() - 1;
          room->setId(it->getId() + 1);
        }
        rooms.push_back(*room);
        m.clear();

        vector<int> status = room->getGame().getStatus();
        stringstream result;
        copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
        m = result.str();
        m = m.substr(0,m.size()-6);
        replaceAll(m,"-1","0");
        m.append(to_string(room->getId()));
        m.append(" 1");
        cout << "Send to server: " << m << endl;
        send(socket, m.c_str(),m.length(),0);
        
        cout << "Player 1 inside room" << endl;
        break;
      }
      case '3': {
        // join room
        cout << "In message 3" << endl;
        vector<string> join = split(receive, ' ');
        string roomId = join[1];
        string player2 = join[2];
        string host = join[3];
        string m;
        join.clear();

        vector<Room>::iterator it;

        for (it = rooms.begin(); it != rooms.end(); it++) {
          if (it->getId() == stoi(roomId)) {
            if (it->getNumPlayer() == 1) {

              it->addPlayer(socket, 2, player2);

              vector<int> status = it->getGame().getStatus();
              stringstream result;
              copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
              m = result.str();
              m = m.substr(0,m.size()-6);
              replaceAll(m,"-1","0");
              m.append("2");
              
              // Send to player 2
              send(it->getPlayers()[1].getSocket(), m.c_str(), m.length(), 0);
              send(it->getPlayers()[0].getSocket(), player2.c_str(), player2.length(), 0);
              it->setNumPlayer(2);
              cout << "Send to client 1: " << player2 << endl;
              cout << "Send to client 2: " << m << endl;
              cout << "Player 2 joined room" << endl;
            }
            else if(it->getNumPlayer() == 2){
              it->addSpectator(socket,player2);
              cout<<it->getSpectators()[it->getSpectators().size()-1].getName()<<" watching the game\n";
              vector<int> status = it->getGame().getStatus();
              stringstream result;
              copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
              join = split(result.str(),' ');
              string score1 = join[64];
              string score2 = join[65];
              string turn = join[66];
              for(int i = 0; i<64; i++){
                m.append(join[i]).append(" ");
              }
              replaceAll(m,"-1","0");
              m.append(turn).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName()).append(" ").append(score1).append(" ").append(score2);
              for(int i = 0; i < it->getSpectators().size(); i++){
                send(it->getSpectators()[i].getSocket(),m.c_str(),m.length(),0);
              }
            }
          }
        } 
        break;
      }
      case '4': {
        // Start the game
        cout << "In message 4" << endl;
        vector<string> tmp = split(receive, ' ');
        string roomId = tmp[1];

        if(stoi(roomId) == 0){
          vector<Players>::iterator i;
          for(i = players.begin(); i != players.end(); i++){
            if(i->getSocket() == socket){
              break;
            }
          }
          vector<int> status = i->getGame().getStatus();
          stringstream result;
          copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));

          tmp = split(result.str(), ' ');

          string board = result.str().substr(0,result.str().size() - 6);
          string score1 = tmp[64];
          string score2 = tmp[65];
          
          if(i->getTurn() == 0 || i->getTurn() == 2){
            i->setTurn(1);
          }
          else if(i->getTurn() == 1){
            i->setTurn(2);
          }

          string m = board.append(to_string(i->getTurn())).append(" ")
                        .append(score1).append(" ")
                        .append(score2);
          
          send(socket,m.c_str(),m.length(),0);

        }
        else{
          vector<Room>::iterator it;

          for (it = rooms.begin(); it != rooms.end(); it++) {
            if (it->getId() == stoi(roomId)) {
              if (it->getNumPlayer() == 2) {
                
                if (it->getState() == 1) {
                  it->setState(0);
                }

                vector<int> status = it->getGame().getStatus();
                stringstream result;
                copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
                m = result.str();
                tmp = split(m, ' ');

                string board = m.substr(0,m.size() - 6);
                
                string score1 = tmp[64];
                string score2 = tmp[65];
                string turn = tmp[66];

                string save1 = board;
                string save2 = board;
                string save3 = board;

                string p1 = save1.append(to_string(it->getPlayers()[0].getTurn())).append(" ")
                              .append(it->getPlayers()[0].getName()).append(" ")
                              .append(it->getPlayers()[1].getName()).append(" ")
                              .append(score1).append(" ")
                              .append(score2);

                replaceAll(save2, "-1", "0");
                string p2 = save2.append(to_string(it->getPlayers()[0].getTurn())).append(" ")
                              .append(it->getPlayers()[0].getName()).append(" ")
                              .append(it->getPlayers()[1].getName()).append(" ")
                              .append(score1).append(" ")
                              .append(score2);

                string p3 = save3.append(turn).append(" ")
                              .append(it->getPlayers()[0].getName()).append(" ")
                              .append(it->getPlayers()[1].getName()).append(" ")
                              .append(score1).append(" ")
                              .append(score2);
                
                
                cout << "Turn: " << it->getPlayers()[0].getTurn() << endl;

                // send to player 1
                if(it->getPlayers()[0].getTurn() == 1){
                  send(it->getPlayers()[0].getSocket(),p1.c_str(),p1.length(),0);
                  send(it->getPlayers()[1].getSocket(),p2.c_str(),p2.length(),0);
                }
                else if(it->getPlayers()[0].getTurn() == 2){
                  send(it->getPlayers()[0].getSocket(),p2.c_str(),p2.length(),0);
                  send(it->getPlayers()[1].getSocket(),p1.c_str(),p1.length(),0);
                }

                for(int i = 0; i < it->getSpectators().size(); i++){
                  send(it->getSpectators()[i].getSocket(),p3.c_str(),p3.length(),0);
                }

                cout << "Send to player 1: " << p1 << endl;
                cout << "Send to player 2: " << p2 << endl;

                cout << "Game has started" << endl;
                
              }
            }
          }
        }
        break;
      }
      case '5': {
        // send a move
        cout << "In message 5" << endl;

        // ------------------------
  
        vector<string> move = split(receive, ' ');
        cout << "Received: " << receive << endl;
        string roomId = move[1];
        int x = stoi(move[2]);
        int y = stoi(move[3]);
        vector<int> moveBot;

        if(stoi(roomId) == 0){
          vector<Players>::iterator i;
          for(i = players.begin(); i != players.end(); i++){
            if(i->getSocket() == socket){
              break;
            }
          }
          if(i->getGame().validateInput(x,y)){
            i->getGame().makeMove(x,y);
            i->getGame().checkStatus();
            i->getGame().printStatus();
          }

          vector<int> status = i->getGame().getStatus();
          stringstream result;
          copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
          cout << "Result: " << result.str();
          vector<string> info = split(result.str(),' ');
          string score1 = info[64];
          string score2 = info[65];
          string turn = info[66];
          string board = "";
          for(int i = 0; i<64; i++){
            board.append(info[i]).append(" ");
          }
          
          string message = board;
          
          message.append(turn).append(" ")
                    .append(score1).append(" ")
                    .append(score2);

          cout << "Message: " << message << endl;

          send(socket, message.c_str(),message.length(),0);

          while(stoi(turn) != i->getTurn()){
            moveBot = i->getBot().makeMove(i->getGame().getBoard());
            x = moveBot[0];
            y = moveBot[1];

            if(i->getGame().validateInput(x,y)){
              i->getGame().makeMove(x,y);
              i->getGame().checkStatus();
              i->getGame().printStatus();
            }
            
            status.clear();
            info.clear();
            status = i->getGame().getStatus();
            stringstream resultBot;
            copy(status.begin(), status.end(), ostream_iterator<int>(resultBot, " "));
            cout << "Result: " << resultBot.str();
            info = split(resultBot.str(),' ');
            score1 = info[64];
            score2 = info[65];
            turn = info[66];
            board = "";
            for(int i = 0; i<64; i++){
              board.append(info[i]).append(" ");
            }
            
            message = board;
            
            message.append(turn).append(" ")
                      .append(score1).append(" ")
                      .append(score2);

            cout << "Message: " << message << endl;

            send(socket, message.c_str(),message.length(),0);
          }
        }
        else{
          vector<Room>::iterator it;
          for (it = rooms.begin(); it != rooms.end(); it++) {
            if (it->getId() == stoi(roomId)) {
              
              if (it->getNumPlayer() == 1) {
                vector<int> status = it->getGame().getStatus();
                stringstream result;
                copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
                string m = result.str();
                m = m.substr(0,m.size()-6);
                replaceAll(m,"-1","0");
                m.append("-2 ").append(it->getPlayers()[0].getName()).append(" # 1 0");
                cout << "The other quit. Send to client: " << m << endl;
                send(socket, m.c_str(), m.length(), 0);
              } else {
                cout << "x = " << x << endl;
                cout << "y = " << y << endl;

                // validate input
                if(it->getGame().validateInput(x, y)) {
                  it->getGame().makeMove(x, y);
                  it->getGame().checkStatus();
                  it->getGame().printStatus();
                }

                vector<int> status = it->getGame().getStatus();
                stringstream result;
                copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
                cout << "Result: " << result.str();
                vector<string> info = split(result.str(),' ');
                string score1 = info[64];
                string score2 = info[65];
                string turn = info[66];
                string board = "";
                for(int i = 0; i<64; i++){
                  board.append(info[i]).append(" ");
                }
                
                string message1 = board;
                
                message1.append(turn).append(" ")
                          .append(it->getPlayers()[0].getName()).append(" ")
                          .append(it->getPlayers()[1].getName()).append(" ")
                          .append(score1).append(" ")
                          .append(score2);

                cout << "Message: " << message1 << endl;

                replaceAll(board,"-1","0");
                string message2 = board;
                message2.append(turn).append(" ")
                        .append(it->getPlayers()[0].getName()).append(" ")
                        .append(it->getPlayers()[1].getName()).append(" ")
                        .append(score1).append(" ").append(score2);

                if(turn.compare(to_string(it->getPlayers()[0].getTurn())) == 0 && !it->getGame().gameOver()){ 
                  send(it->getPlayers()[0].getSocket(), message1.c_str(),message1.length(),0);
                  send(it->getPlayers()[1].getSocket(), message2.c_str(),message2.length(),0);

                  for(int i = 0; i < it->getSpectators().size(); i++){
                    send(it->getSpectators()[i].getSocket(),message2.c_str(),message2.length(),0);
                  }

                } else if(turn.compare(to_string(it->getPlayers()[1].getTurn())) == 0 && !it->getGame().gameOver()){
                    send(it->getPlayers()[1].getSocket(), message1.c_str(),message1.length(),0);
                    send(it->getPlayers()[0].getSocket(), message2.c_str(),message2.length(),0);

                    for(int i = 0; i < it->getSpectators().size(); i++){
                      send(it->getSpectators()[i].getSocket(),message2.c_str(),message2.length(),0);
                    }

                } else if(it->getGame().gameOver()){
                    cout<<"Game over\n";
                    turn = "-1";
                    string m = "";
                    for(int i = 0; i<64; i++){
                      m.append(info[i]).append(" ");
                    }
                    m.append(turn).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName()).append(" ").append(score1).append(" ").append(score2);
                    send(it->getPlayers()[1].getSocket(), m.c_str(),m.length(),0);
                    send(it->getPlayers()[0].getSocket(), m.c_str(),m.length(),0);

                    for(int i = 0; i < it->getSpectators().size(); i++){
                      send(it->getSpectators()[i].getSocket(),m.c_str(),m.length(),0);
                    }
                }
              }
              
              break;
            }
          }
        }
        break;
      }

      case '6': {
        cout << "In message 6" << endl;
        vector<string> req = split(receive, ' ');
        string roomId = req[1];
        string state = req[2];

        cout << "State: " << state << endl;

        vector<Room>::iterator it;

        for (it = rooms.begin(); it != rooms.end(); it++) {
          if (it->getId() == stoi(roomId)) {

            if (it->getNumPlayer() == 2) {
              // remove player
              if (it->getPlayers()[0].getSocket() == socket) {
                  it->removePlayer(1);
                  it->setTurn(0,1);
                } else {
                  it->removePlayer(2);
                  it->setTurn(0,1);
                }
              it->setNumPlayer(1);
              if (stoi(state) == 0) {
                

                cout << "Socket: " << it->getPlayers()[0].getSocket() << endl;

                // set number of player in room == 1
                
                vector<int> status = it->getGame().getStatus();
                stringstream result;
                copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
                string m = result.str();
                m = m.substr(0,m.size()-6);
                replaceAll(m,"-1","0");
                m.append("0");
                cout << "Send to server: " << m << endl;
                send(it->getPlayers()[0].getSocket(), m.c_str(),m.length(),0);

                //send to all spectators
                for(int i = 0; i < it->getSpectators().size(); i++){
                  send(it->getSpectators()[i].getSocket(),m.c_str(),m.length(),0);
                }
                
                cout << it->getPlayers()[0].getName() << " now is hosting..." << endl;
              } else {
                // code for quitting during game
                vector<int> status = it->getGame().getStatus();
                stringstream result;
                copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
                string m = result.str();
                m = m.substr(0,m.size()-6);

                m.append("-2 ").append(it->getPlayers()[0].getName()).append(" # 1 0");
              
                cout << "Other quit. Send to client: " << m << endl;
                
                send(it->getPlayers()[0].getSocket(), m.c_str(), m.length(), 0);

                //send to all spectators
                for(int i = 0; i < it->getSpectators().size(); i++){
                  send(it->getSpectators()[i].getSocket(),m.c_str(),m.length(),0);
                }
              } 
              
            } else if (it->getNumPlayer() == 1) {
                it->removePlayer(1);
                it->setNumPlayer(0);
                for(int i = 0; i < it->getSpectators().size(); i++){
                  send(it->getSpectators()[i].getSocket(),"empty",5,0);
                }
            }
            send(socket, "0", 1, 0);
            break;
          }
        }

        break;
      }

      case '7': { // play again
        cout << "In message 7" << endl;
        vector<string> req = split(receive, ' ');
        int roomID = stoi(req[1]);
        string turn = req[2];

        vector<Room>::iterator it;

        for (it = rooms.begin(); it != rooms.end(); it++) {
          if (it->getId() == roomID) {

            if (it->getNumPlayer() == 1) {  // if there is 1 player in a room, set as host
              it->newGame();
              vector<int> status = it->getGame().getStatus();
              stringstream result;
              copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
              m = result.str();
              m = m.substr(0,m.size()-6);
              replaceAll(m,"-1","0");

              m.append("3 ").append(it->getPlayers()[0].getName());
              cout << "Play again: " << m << endl;
              send(socket, m.c_str(), m.length(), 0);
              //send to all spectators
              for(int i = 0; i < it->getSpectators().size(); i++){
                send(it->getSpectators()[i].getSocket(),m.c_str(),m.length(),0);
              }
            } else if (it->getNumPlayer() == 2) {
                // if clicked first
              cout << "State: " << it->getState() << endl;
              if (it->getState() == 0) {
                // wait for another player's response
                it->setState(1);
                it->newGame();
                vector<int> status = it->getGame().getStatus();
                stringstream result;
                copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
                m = result.str();
                m = m.substr(0,m.size()-6);
                replaceAll(m,"-1","0");
                
                m.append("0");
                cout << "Send to server: " << m << endl;
                send(socket, m.c_str(),m.length(),0);
                //send to all spectators
                for(int i = 0; i < it->getSpectators().size(); i++){
                  send(it->getSpectators()[i].getSocket(),m.c_str(),m.length(),0);
                }
              } else if (it->getState() == 1){  // if clicked after
                it->swapTurn();
                vector<int> status = it->getGame().getStatus();
                stringstream result;
                copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
                m = result.str();
                m = m.substr(0,m.size()-6);
                replaceAll(m,"-1","0");
                string m2 = m;
                m2.append(to_string(it->getPlayers()[1].getTurn()));
                string m1 = m;
                m1.append(to_string(it->getPlayers()[0].getTurn()));
                cout << "Send here" << endl;
                // Send to player 2
                send(it->getPlayers()[1].getSocket(), m2.c_str(), m2.length(), 0);
                send(it->getPlayers()[0].getSocket(), m1.c_str(), m1.length(), 0);
                //send to all spectators
                for(int i = 0; i < it->getSpectators().size(); i++){
                  send(it->getSpectators()[i].getSocket(),m.c_str(),m.length(),0);
                }
              }
            }
            break;
          }
        }
        

        break;
      }

      case '8': {
        string roomId = split(receive,' ')[1];
        vector<Room>::iterator it;
        for(it = rooms.begin(); it != rooms.end(); it++){
          if(it->getId() == stoi(roomId)){
            it->removeSpectator(socket);
            break;
          }
        }
        send(socket, "0",1,0);
        break;
      }

      case '9': {
        string difficulty = split(receive,' ')[1];
        int dif = stoi(difficulty);
        vector<Players>::iterator i;
        for(i = players.begin(); i != players.end(); i++){
          if(i->getSocket() == socket){
            break;
          }
        }
        switch(dif){
          case 0: {//exit bot
            i->getGame().newGame();
            send(socket,"0",1,0);
            break;
          }
          case 1: {
            Bot bot = Bot();
            i->setBot(bot);
            vector<int> status = i->getGame().getStatus();
            stringstream result;
            copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
            string m = result.str();
            m = m.substr(0,m.size()-6);
            replaceAll(m,"-1","0");
            m.append("1");
            send(socket,m.c_str(),m.length(),0);
            break;
          }
          case 2: {
            MiniMax bot = MiniMax(2 , 1);
            i->setBot(bot);
            vector<int> status = i->getGame().getStatus();
            stringstream result;
            copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
            string m = result.str();
            m = m.substr(0,m.size()-6);
            replaceAll(m,"-1","0");
            m.append("1");
            send(socket,m.c_str(),m.length(),0);
            break;
          }
          case 3: {
            MiniMax bot = MiniMax(2 , 3);
            i->setBot(bot);
            vector<int> status = i->getGame().getStatus();
            stringstream result;
            copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
            string m = result.str();
            m = m.substr(0,m.size()-6);
            replaceAll(m,"-1","0");
            m.append("1");
            send(socket,m.c_str(),m.length(),0);
            break;
          }
          
        }
        break;
        
      }

      default: break;
    }
    
    if (pthread_mutex_unlock(&mut) != 0) {                                      
      perror("pthread_mutex_unlock() error");                                     
      exit(2);                                                                    
    }

    bzero(buf, MAXLINE);
  }

  if (n <= 0){
    cout<<"Read error"<<endl;
    vector<Room>::iterator it;
    vector<Players>::iterator i;
    if(rooms.empty()){
      return 0;
    }
    for(it = rooms.begin(); it != rooms.end(); it++){
      if(it->getNumPlayer() == 2){
        if(it->getPlayers()[0].getSocket() == socket){
          for(i = players.begin() ; i != players.end(); i++){
            if(i->getName() == it->getPlayers()[0].getName()){
              players.erase(i);
              break;
            }
          }
          it->removePlayer(1);
          it->setTurn(0,1);

          it->setNumPlayer(1);
          it->newGame();
          vector<int> status = it->getGame().getStatus();
          stringstream result;
          copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
          string mess = result.str().substr(0,result.str().size() - 6);
          send(it->getPlayers()[0].getSocket(),mess.c_str(),mess.length(),0);
          for(int i = 0; i < it->getSpectators().size(); i++){
            send(it->getSpectators()[i].getSocket(),mess.c_str(),mess.length(),0);
          }
        }
        else if(it->getPlayers()[1].getSocket() == socket){
          for(i = players.begin() ; i != players.end(); i++){
            if(i->getName() == it->getPlayers()[1].getName()){
              players.erase(i);
              break;
            }
          }
          it->removePlayer(2);
          it->setTurn(0,1);

          
          it->setNumPlayer(1);
          it->newGame();
          vector<int> status = it->getGame().getStatus();
          stringstream result;
          copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
          string mess = result.str().substr(0,result.str().size() - 6);
          send(it->getPlayers()[0].getSocket(),mess.c_str(),mess.length(),0);
          for(int i = 0; i < it->getSpectators().size(); i++){
            send(it->getSpectators()[i].getSocket(),mess.c_str(),mess.length(),0);
          }
        }
        else if(it->findSpectator(socket)){
          it->removeSpectator(socket);
        }
        else continue;

        
        break;
      }
      else if(it->getNumPlayer() == 1){
        if(it->getPlayers()[0].getSocket() == socket){
          for(i = players.begin() ; i != players.end(); i++){
            if(i->getName() == it->getPlayers()[0].getName()){
              players.erase(i);
              break;
            }
          }
          string name = "";
          for(vector<Players>::iterator i = players.begin(); i != players.end(); i++){
            name.append(i->getName()).append(" ");
          }
          cout<<"Current players' names: "<<name<<endl;
          it->removePlayer(1);
        }
        else continue;
        it->setNumPlayer(0);
        break;
      }
    }
    num_threads--; 
  }
	return 0;
}

void replaceAll(string& str, const string& from, const string& to) {
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

bool findStringIC(const string & strHaystack, const string & strNeedle)
{
  auto it = search(
    strHaystack.begin(), strHaystack.end(),
    strNeedle.begin(),   strNeedle.end(),
    [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
  );
  return (it != strHaystack.end() );
}
