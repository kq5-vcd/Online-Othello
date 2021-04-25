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
#include "Room.hpp"

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 20      /*maximum number of client connections*/

using namespace std;

//int id = 0;
vector<Room> rooms ;

//string map_to_string(map<int,Room>  &m);

vector<string> split(const string& str, const char& delimiter);

void *connection_handler(void *client_socket);

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
  
  int num_threads = 0;
  pthread_t threads[LISTENQ];
  while (num_threads<LISTENQ){
		printf("Listening...\n");
		int client_socket = accept(listenfd, NULL, NULL);
    cout<<client_socket<<endl;
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
  string m;
	char buf[MAXLINE];
  while ((n = recv(socket, buf, MAXLINE, 0)) > 0)
  {
    cout<<socket<<endl;
    char b[n];
    for(int c = 0; c<=n; c++){
      b[c] = buf[c];
    }
    string receive(b);
    if(receive[0] == '1'){
      //string m = map_to_string(rooms);
      m.clear();
      cout<<"check1\n";
      if(!rooms.empty()){
        vector<Room>::iterator it;
        for(it = rooms.begin(); it != rooms.end(); it++){
          if(it->getNumPlayer()>0 && !it->getGame().gameOver()){  
            m.append(to_string(it->getId())).append(" ").append(to_string(it->getGame().getScores()[0])).append(" ").append(to_string(it->getGame().getScores()[1]));
            m.append(",");
          }
        }
        send(socket, m.c_str(),m.length()-1,0);
      }
      else send(socket, "empty",5,0);
    }
    else if(receive[0] == '2'){
      cout<<"check2\n";
      Room *room = new Room(false);
      cout<<"check\n";
      room->setNumPlayer(1);
      room->addPlayer(socket, 1);
      if(rooms.empty()){
        room->setId(1);
      }
      else{
        vector<Room>::iterator it = rooms.end() - 1;
        room->setId(it->getId() + 1);
      }
      //id++;
      //rooms.insert({id,room});
      rooms.push_back(*room);
      m.clear();
      m = to_string(room->getId());
      m.append(" ").append(to_string(room->getPlayers()[0].getTurn()));
      
      send(socket, m.c_str(),m.length(),0);
      cout<<"Player 1 inside room"<<endl;
    }
    else if(receive[0] == '3'){
      cout<<"check3\n";
      string roomId = split(receive,' ')[1];
      //map<int,Room>::iterator it = rooms.find(stoi(roomId));
      vector<Room>::iterator it;
      for(it = rooms.begin(); it!=rooms.end(); it++){
        if(it->getId() == stoi(roomId) && it->getNumPlayer() == 1){
          it->addPlayer(socket, 2);
          cout<<it->getPlayers()[0].getSocket()<<" "<<it->getPlayers()[1].getSocket()<<endl;
          roomId.append(" ").append(to_string(it->getPlayers()[1].getTurn()));
          it->setNumPlayer(2);
          send(socket,roomId.c_str(),roomId.length(),0);
          cout<<"Player 2 joined room"<<endl;
          break;
        }
      }
      if(it == rooms.end()){
        send(socket, "0",1,0);
      }
    }
    else if(receive[0] == '4' || receive[0] == 'S'){
      vector<Room>::iterator it;
      vector<string> move = split(receive,' ');
      string roomId = move[1];
      //Game game = rooms.find(stoi(roomId))->second.getGame();
      for(it = rooms.begin(); it!=rooms.end(); it++){
        if(it->getId() == stoi(roomId)){
          
          break;
        }
      }
      cout<<it->getId()<<endl;

      if(receive[0] == '4'){
        
        
        int x = stoi(move[2]);
        int y = stoi(move[3]);

        if(it->getGame().validateInput(x, y)) {
          cout<<"check move"<<endl;
          it->getGame().makeMove(x, y);
          it->getGame().checkStatus();
          it->getGame().printStatus();
        }
      }
      else if(receive[0] == 'S'){
        //it->newGame();
        cout<<"Check game\n";
      }

      vector<int> status = it->getGame().getStatus();
      stringstream result;
      copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
      if(it->getPlayers().size() == 1) {
        printf("%s\n",result.str().c_str());
        send(socket, result.str().c_str(),result.str().length(),0);
      }
      else if(it->getPlayers().size() == 2 && receive[0] == 'S'){
        send(it->getPlayers()[1].getSocket(), result.str().c_str(),result.str().length(),0);
      }
      else if(it->getPlayers().size() == 2 && receive[0] == '4'){
        cout<<it->getPlayers()[0].getSocket()<<" "<<it->getPlayers()[1].getSocket()<<endl;
        send(it->getPlayers()[0].getSocket(), result.str().c_str(),result.str().length(),0);
        send(it->getPlayers()[1].getSocket(), result.str().c_str(),result.str().length(),0);
        cout<<"check send"<<endl;
      }
      

      if(it->getGame().gameOver()){
        it->getPlayers().clear();
      }

    }
    else if(receive[0] == '5'){
      
      string roomId = split(receive,' ')[1];
      //map<int,Room>::iterator it = rooms.find(stoi(roomId));
      vector<Room>::iterator it;
      for(it = rooms.begin(); it<=rooms.end(); it++){
        if(it->getId() == stoi(roomId)){
          it->setNumPlayer(it->getNumPlayer()-1);
          it->getPlayers().erase(it->getPlayers().begin());
          it->newGame();

          send(socket, "0",1,0);
        }
      }
      //it->second.setNumPlayer(it->second.getNumPlayer()-1);
      
    }
  }
  if (n < 0)
    cout<<"Read error"<<endl;
	
	return 0;
}