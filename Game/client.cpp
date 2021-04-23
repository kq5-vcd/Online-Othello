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
#define BUFF_SIZE 1024

using namespace std;

bool checkNumber(string str);

int main(int argc, char **argv)
{
    int sockfd, n,m;
    struct sockaddr_in servaddr;
    char buff[BUFF_SIZE + 1];
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
    while(1){
        memset(buff,'\0',(strlen(buff)+1));
        cout<<"Input\n";
        fgets(buff, BUFF_SIZE, stdin);
        cout<<"asdf\n";
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
        cout<<mess<<endl;
    }

    exit(0);
}


