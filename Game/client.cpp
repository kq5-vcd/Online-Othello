#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#define MAXLINE 4096   /*max text line length*/
#define SERV_PORT 3000 /*port*/

using namespace std;

bool checkNumber(string str);

int main(int argc, char **argv)
{
    int sockfd, n;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE], recvline[MAXLINE];

    //basic check of the arguments
    //additional checks can be inserted
    if (argc != 2)
    {
        perror("Usage: TCPClient <IP address of the server");
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

    while ((n = recv(sockfd, recvline, MAXLINE, 0))>0)
    {
        int x,y;
        string inputX = "";
        string inputY = "";
        string sendMess = "";

        cout << "Select row: ";
        cin >> inputX;

        if(!checkNumber(inputX)){
            inputX = "-1";
        }

        cout << "Select column: ";
        cin >> inputY;

        if(!checkNumber(inputY)){
            inputY = "-1";
        }

        sendMess.append(inputX).append(" ").append(inputY);

        send(sockfd, sendMess.c_str(), sendMess.length(), 0);

        
        
        cout<<"Received from the server: ";
        string mess(recvline);
        cout<<mess<<endl;
        
        
        
    }
    if (recv(sockfd, recvline, MAXLINE, 0) == 0)
    {
        cout<<"The game is finished\n";
        exit(0);
    } 

    exit(0);
}

bool checkNumber(string str) {
   for (int i = 0; i < int(str.length()); i++) if (isdigit(str[i]) == false) return false;
   return true;
}

