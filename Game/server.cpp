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
#define SERV_PORT 8910 /*port*/
#define LISTENQ 20      /*maximum number of client connections*/

using namespace std;

//int id = 0;
int num_threads = 0;
vector<Room> rooms ;
vector<string> playersName;

//string map_to_string(map<int,Room>  &m);

vector<string> split(const string& str, const char& delimiter);
vector<string> simple_tokenizer(string& s);
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
            cout<<"in"<<endl;
            if(it->getNumPlayer() == 1) {
              m.append(to_string(it->getId())).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(to_string(it->getGame().getScores()[0])).append(" # ").append(to_string(it->getGame().getScores()[1]));
            }
            else{
              m.append(to_string(it->getId())).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(to_string(it->getGame().getScores()[0])).append(" ").append(it->getPlayers()[1].getName()).append(" ").append(to_string(it->getGame().getScores()[1]));
            }
            m.append(",");
            cout<<m<<endl;
          }
        }
        send(socket, m.c_str(),m.length()-1,0);
      }
      else send(socket, "empty",5,0);
    }
    else if(receive[0] == '2'){
      cout<<"check2\n";
      Room *room = new Room(false);
      string username1 = split(receive,' ')[1];
      room->setNumPlayer(1);
      room->addPlayer(socket, 1, username1);
      cout<<room->getPlayers().size()<<endl;
      cout<<room->getPlayers()[0].getSocket()<<endl;
      if(rooms.empty()){
        room->setId(1);
      }
      else{
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
      send(socket, m.c_str(),m.length(),0);
      cout<<"Player 1 inside room"<<endl;
      
    }
    else if(receive[0] == '3'){
      cout<<"check3\n";
      vector<string> join = split(receive,' ');
      string roomId = join[1];
      string username2 = join[2];
      vector<Room>::iterator it;
      for(it = rooms.begin(); it!=rooms.end(); it++){
        if(it->getId() == stoi(roomId) && it->getNumPlayer() == 1){
          cout<<it->getPlayers().size()<<endl;
          it->addPlayer(socket,2,username2);
        
          cout<<it->getPlayers()[0].getSocket()<<" "<<it->getPlayers()[1].getSocket()<<endl;
          vector<int> status = it->getGame().getStatus();
          stringstream result;
          copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
          string mess1 = result.str();
          mess1 = mess1.substr(0,mess1.size()-6);
          string mess2 = mess1;
          replaceAll(mess2,"-1","0");
          mess1.append(to_string(it->getPlayers()[0].getTurn())).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName());
          mess2.append(to_string(it->getPlayers()[1].getTurn())).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName());
          it->setNumPlayer(2);
          send(it->getPlayers()[0].getSocket(),mess1.c_str(),mess1.length(),0);
          send(it->getPlayers()[1].getSocket(),mess2.c_str(),mess2.length(),0);
          cout<<"Player 2 joined room"<<endl;
          break;
        }
      }
      if(it == rooms.end()){
        send(socket, "0",1,0);
      }
    }
    else if(receive[0] == '4'){
      vector<Room>::iterator it;
      vector<string> move = split(receive,' ');
      string roomId = move[1];
      for(it = rooms.begin(); it!=rooms.end(); it++){
        if(it->getId() == stoi(roomId)){
          
          break;
        }
      }
      cout<<it->getId()<<endl;

      // while(it->getNumPlayer() == 1){
      //   for(it = rooms.begin(); it!=rooms.end(); it++){
      //     if(it->getId() == stoi(roomId)){
      //       break;
      //     }
      //   }
      //   string mess = "W";
      //   mess.append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName());
      //   send(socket,mess.c_str(),mess.length(),0);
      //   usleep(2000000);
      // }
        
        
      int x = stoi(move[2]);
      int y = stoi(move[3]);

      if(it->getGame().validateInput(x, y)) {
        cout<<"check move"<<endl;
        it->getGame().makeMove(x, y);
        it->getGame().checkStatus();
        it->getGame().printStatus();
      }

      vector<int> status = it->getGame().getStatus();
      stringstream result;
      copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
      string mess = "";
      cout<<result.str()<<endl;
      if(it->getPlayers().size() == 1) {
        printf("%s\n",result.str().c_str());
        send(socket, mess.c_str(),mess.length(),0);
      }
      else if(it->getPlayers().size() == 2){
        cout<<it->getPlayers()[0].getSocket()<<" "<<it->getPlayers()[1].getSocket()<<endl;
        vector<string> info = split(result.str(),' ');
        string score1 = info[64];
        string score2 = info[65];
        string turn = info[66];
        for(int i = 0; i<64; i++){
          mess.append(info[i]).append(" ");
        }
        cout<<it->getGame().gameOver()<<endl;
        mess.append(turn).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName()).append(" ").append(score1).append(" ").append(score2);
        if(turn.compare("1") == 0 && !it->getGame().gameOver()){
          send(it->getPlayers()[0].getSocket(), mess.c_str(),mess.length(),0);
          string m = "";
          for(int i = 0; i<64; i++){
            m.append(info[i]).append(" ");
          }
          replaceAll(m,"-1","0");
          m.append(turn).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName()).append(" ").append(score1).append(" ").append(score2);
          send(it->getPlayers()[1].getSocket(), m.c_str(),m.length(),0);
        }
        else if(turn.compare("2") == 0 && !it->getGame().gameOver()){
          send(it->getPlayers()[1].getSocket(), mess.c_str(),mess.length(),0);
          string m = "";
          for(int i = 0; i<64; i++){
            m.append(info[i]).append(" ");
          }
          replaceAll(m,"-1","0");
          m.append(turn).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName()).append(" ").append(score1).append(" ").append(score2);
          send(it->getPlayers()[0].getSocket(), m.c_str(),m.length(),0);
        }
        else if(it->getGame().gameOver()){
          cout<<"Game over\n";
          turn = "-1";
          string m = "";
          for(int i = 0; i<64; i++){
            m.append(info[i]).append(" ");
          }
          m.append(turn).append(" ").append(it->getPlayers()[0].getName()).append(" ").append(it->getPlayers()[1].getName()).append(" ").append(score1).append(" ").append(score2);
          send(it->getPlayers()[1].getSocket(), m.c_str(),m.length(),0);
          send(it->getPlayers()[0].getSocket(), m.c_str(),m.length(),0);
        }
        
        cout<<"check send"<<endl;
      }
      

      if(it->getGame().gameOver()){
        it->removePlayer(1);
        it->removePlayer(1);
      }

    }
    else if(receive[0] == '5'){
      
      string roomId = split(receive,' ')[1];
      vector<Room>::iterator it;
      for(it = rooms.begin(); it<=rooms.end(); it++){
        if(it->getId() == stoi(roomId)){
          if(it->getNumPlayer() == 2){
            if(it->getPlayers()[0].getSocket() == socket){
              it->removePlayer(1);
              it->setTurn(0,1);
            }
            else {
              it->removePlayer(2);
            }
            it->setNumPlayer(1);
            it->newGame();
            vector<int> status = it->getGame().getStatus();
            stringstream result;
            copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
            string mess = result.str().substr(0,result.str().size() - 6);
            send(it->getPlayers()[0].getSocket(),mess.c_str(),mess.length(),0);
          }
          else if(it->getNumPlayer() == 1){
            it->removePlayer(1);
            it->setNumPlayer(0);
          }

          send(socket, "0",1,0);
          break;
        }
      }
      
    }
    else if(receive[0] == '6'){
      string name = split(receive,' ')[1];
      vector<string>::iterator i = find(playersName.begin(),playersName.end(),name);
      if(i != playersName.end()){
        send(socket,"0",1,0);
      }
      else{
        playersName.push_back(name);
        send(socket,"1",1,0);
      }
    }
  }
  if (n <= 0){
    cout<<"Read error"<<endl;
    vector<Room>::iterator it;
    vector<string>::iterator i;
    for(it = rooms.begin(); it<=rooms.end(); it++){
      if(it->getNumPlayer() == 2){
        if(it->getPlayers()[0].getSocket() == socket){
          i = find(playersName.begin(),playersName.end(),it->getPlayers()[0].getName());
          playersName.erase(i);
          it->removePlayer(1);
          it->setTurn(0,1);
        }
        else if(it->getPlayers()[1].getSocket() == socket){
          i = find(playersName.begin(),playersName.end(),it->getPlayers()[1].getName());
          playersName.erase(i);
          it->removePlayer(2);
        }
        else continue;

        it->setNumPlayer(1);
        it->newGame();
        vector<int> status = it->getGame().getStatus();
        stringstream result;
        copy(status.begin(), status.end(), ostream_iterator<int>(result, " "));
        string mess = result.str().substr(0,result.str().size() - 6);
        send(it->getPlayers()[0].getSocket(),mess.c_str(),mess.length(),0);
        break;
      }
      else if(it->getNumPlayer() == 1){
        if(it->getPlayers()[0].getSocket() == socket){
          i = find(playersName.begin(),playersName.end(),it->getPlayers()[0].getName());
          playersName.erase(i);
          stringstream name;
          copy(playersName.begin(), playersName.end(), ostream_iterator<string>(name, " "));
          cout<<"Current players' names: "<<name.str()<<endl;
          it->removePlayer(1);
          it->setTurn(0,1);
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

vector<string> simple_tokenizer(string& s)
{
    stringstream ss(s);
    vector<string> word;
    string token;
    while (ss >> token) {
        word.push_back(token);
    }
    cout<<"check token"<<endl;
    return word;
}