#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 8910 /*port*/
#define BUFF_SIZE 1024

using namespace std;

bool is_number(const string& s);
vector<string> simple_tokenizer(string& s);

int main(int argc, char **argv)
{
    int sockfd, n, nextTurn, me;
    struct sockaddr_in servaddr;
    char buff[BUFF_SIZE + 1];
    char sendline[MAXLINE], recvline[MAXLINE];
    vector<string> op;

    //basic check of the arguments
    //additional checks can be inserted
    if (argc != 2)
    {
        perror("Usage: client <IP address of the server");
        exit(1);
    }

    //Create a socket for the client
    //If sockfd<0 there was an error in the creation of the socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Problem in creating the socket");
        exit(2);
    }

    //Creation of the socket
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    servaddr.sin_port = htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Problem in connecting to the server");
        exit(3);
    }
    while(1){
        memset(buff,'\0',(strlen(buff)+1));
        cout<<"Input\n";
        fgets(buff, BUFF_SIZE, stdin);
        send(sockfd, buff, strlen(buff)-1, 0);
        memset(recvline,'\0',(strlen(recvline)+1));
        if ((n = recv(sockfd, recvline, MAXLINE, 0)) == 0)
        {
            cout<<"The game is finished\n";
            exit(0);
        } 

        cout<<"Received from the server: ";
        char b[n];
        for(int c = 0; c<=n; c++){
            b[c] = recvline[c];
        }
        string mess(b);
        op = simple_tokenizer(mess);
        cout<<mess<<endl;

        memset(recvline,'\0',(strlen(recvline)+1));

        if(op.size() == 2){
            string join = "S";
            join.append(" ").append(op[0]);
            me = stoi(op[1]);
            cout<<me<<endl;
            cout<<"check\n";
            send(sockfd,join.c_str(),join.length(),0);
            cout<<"check send\n";
            while((n = recv(sockfd, recvline,MAXLINE,0)) > 0){
                cout<<"check recv"<<endl;
                recvline[n] = '\0';
                string m(recvline);
                cout<<m<<endl;
                if(m.compare("0") == 0){
                    cout<<"Exit room\n";
                    break;
                }
                vector<string> status = simple_tokenizer(m);
                cout<<status.size()<<endl;
                cout<<"check status\n";
                if(status[66].compare("-1") == 0){
                    cout<<"Game over\n";
                    break;
                }
                nextTurn = stoi(status[66]);
                if(me == nextTurn){
                    cout<<"Input move: \n";
                    fgets(buff, BUFF_SIZE, stdin);
                    send(sockfd, buff, strlen(buff)-1, 0);
                    memset(recvline,'\0',(strlen(recvline)+1));
                }
                cout<<"continue\n";
            }
        }
    }

    exit(0);
}

bool is_number(const string& s)
{
    return !s.empty() && find_if(s.begin(), 
        s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
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
