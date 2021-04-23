#include <stdlib.h>
#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Game.hpp"

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/
#define LISTENQ 8      /*maximum number of client connections*/

using namespace std;

int id = 0;

string map_to_string(map<int,Game>  *m);

int main(int argc, char **argv)
{
  map<int,Game> rooms;
  
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
      //game.testing();
      bool isOn = !game.gameOver();
      //close listening socket
      close(listenfd);
      string mess = game.getStatus();
      //send(connfd, (mess = game.getStatus()).c_str(),mess.length(),0);
      
      while ((n = recv(connfd, buf, MAXLINE, 0)) > 0)
      {

        char b[n];
				for(int c = 0; c<=n; c++){
					b[c] = buff[c];
				}
        string receive(b);
        switch(receive[0]){
          case '1':
            string m = map_to_string(&rooms);
            send(connfd, m.c_str(),m.length(),0);
            break;
          case '2':
            Game game = Game(8,8);
            rooms.insert({++id,game});
            string m = to_string(id);
            send(connfd, m.c_str(),m.length(),0);
            break;
          case '3':
            string roomId = split(receive," ")[1];
            

        }
        
        if(isOn){
          string move(buf);
          size_t pos = 0;
          int token, cor[2], i = 0;
          
          game.getStatus();

          while ((pos = move.find(" ")) != string::npos) {
              token = stoi(move.substr(0, pos));
              cout << token << endl;
              cor[i] = token;
              i++;
              move.erase(0, pos + 1);
          }
          if(game.validateInput(x, y)) {
              game.makeMove(x, y);
              game.checkStatus();
          }

        }
        else{ 
          strcpy(buf,"server\n");

          //send(connfd, buf, strlen(buf), 0);
          cout<<"Client closed\n";
          break;
        }

        send(connfd, mess.c_str(),mess.length(),0);
        
      }

      if (n < 0)
        cout<<"Read error"<<endl;
      exit(0);
    }
    //close socket of the server
    close(connfd);
  }
}

string map_to_string(map<int,Game> m) {
  string output = "";
  string convrt = "";
  string result = "";

	for (map<int,Game>::const_iterator it = m.cbegin(); it != m.cend(); it++) {
    if(!it->second.gameOver()){
      convrt = to_string(it->first);
      output.append(convrt).append(" ").append((it->second.)).append(" ").append(it->second.).append(",");
    }
  }
	
	result = output.substr(0, output.size() - 1 );
	
  return result;
}

vector<string> split(const string& str, const string& delimiter)
{
    vector<string> cont;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        cont.push_back(token);
    }
    return cont;
}