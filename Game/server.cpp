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
#include "Room.hpp"

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections*/

using namespace std;

//int id = 0;
int Room::id = 0;

string map_to_string(map<int,Room>  &m);

vector<string> split(const string& str, const char& delimiter);

int main(int argc, char **argv)
{
  //map<int,Room> rooms;
  vector<Room> rooms;
  
  
  int listenfd, connfd, n;
  pid_t childpid;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr, servaddr;
  string mess;

  //Create a socket for the soclet
  //If sockfd<0 there was an error in the creation of the socket
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("Problem in creating the socket");
    exit(2);
  }

  //preparation of the socket address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  //bind the socket
  bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  //listen to the socket by creating a connection queue, then wait for clients
  listen(listenfd, LISTENQ);

  cout<<"Server running...waiting for connections."<<endl;
  

  for (;;)
  {
    clilen = sizeof(cliaddr);
    //accept a connection
    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);

    cout<<"Received request..."<<endl;

    if ((childpid = fork()) == 0)
    { //if it’s 0, it’s child process


      cout<<"Child created for dealing with client requests"<<endl;
      //close listening socket
      close(listenfd);
      while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
      {
        cout<<"check"<<endl;
        char b[n];
				for(int c = 0; c<=n; c++){
					b[c] = buf[c];
				}
        string receive(b);
        if(receive[0] == '1'){
          //string m = map_to_string(rooms);
          string m = "";
          
          if(!rooms.empty()){
            
            vector<Room>::iterator it;
            for(it = rooms.begin(); it<=rooms.end(); it++){
              if(it->getNumPlayer()>0 && !it->getGame().gameOver()){  
                m.append(to_string(it->getId())).append(" ").append(to_string(it->getGame().getScores()[0])).append(" ").append(to_string(it->getGame().getScores()[1]));
                m.append(",");
              }
            }
          }
          send(connfd, m.c_str(),m.length()-1,0);
        }
        else if(receive[0] == '2'){
          cout<<"check\n";
          Room room = Room(false);
          room.setNumPlayer(1);
          //id++;
          //rooms.insert({id,room});
          rooms.push_back(room);
          string m = to_string(room.getId());
          send(connfd, m.c_str(),m.length(),0);
          cout<<"Room ID: "<<m<<endl;
        }
        else if(receive[0] == '3'){
          string roomId = split(receive,' ')[1];
          //map<int,Room>::iterator it = rooms.find(stoi(roomId));
          vector<Room>::iterator it;
          for(it = rooms.begin(); it<=rooms.end(); it++){
            if(it->getId() == stoi(roomId)){
              send(connfd,roomId.c_str(),roomId.length(),0);
              it->setNumPlayer(2);
              cout<<"Join"<<endl;
            }
          }
          if(it == rooms.end() || it->getNumPlayer() == 0){
            send(connfd, "0",1,0);
          }
          // else{
          //   send(connfd,roomId.c_str(),roomId.length(),0);
          //   it->setNumPlayer(2);
          //   cout<<"Join"<<endl;
          // }
        }
        else if(receive[0] == '4'){
          vector<string> move = split(receive,' ');
          string roomId = move[1];
          //Game game = rooms.find(stoi(roomId))->second.getGame();
          vector<Room>::iterator it;
          Game game = Game(false);
          for(it = rooms.begin(); it<=rooms.end(); it++){
            if(it->getId() == stoi(roomId)){
              game = it->getGame();
            }
          }
          int x = stoi(move[2]);
          int y = stoi(move[3]);

          if(game.validateInput(x, y)) {
            game.makeMove(x, y);
            game.checkStatus();
          }

          if(game.gameOver()){
            send(connfd, "0",1,0);
            break;
          }
        }
        else if(receive[0] == '5'){
          send(connfd, "0",1,0);
          string roomId = split(receive,' ')[1];
          //map<int,Room>::iterator it = rooms.find(stoi(roomId));
          vector<Room>::iterator it;
          for(it = rooms.begin(); it<=rooms.end(); it++){
            if(it->getId() == stoi(roomId)){
              it->setNumPlayer(it->getNumPlayer()-1);
            }
          }
          //it->second.setNumPlayer(it->second.getNumPlayer()-1);
          break;
        }
      }
      if (n < 0)
        cout<<"Read error"<<endl;
      exit(0);
    }
    //close socket of the server
    close(connfd);
  }
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