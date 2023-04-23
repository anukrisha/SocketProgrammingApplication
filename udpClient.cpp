#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

//chatting function
void clientChat(int clientSocket, struct sockaddr_in serverAdd)
{
    char message[1024];
    int n;
    socklen_t len;

    for(;;)
    {
        bzero(message,1024);
        cout<<"Enter your message: ";
        n=0;
        while ((message[n++] = getchar()) != '\n');

        //sending message to server
        sendto(clientSocket, message, sizeof(message),0, (const struct sockaddr *) &serverAdd, sizeof(serverAdd));
        bzero(message,sizeof(message));

        //receiveing message from client
        recvfrom(clientSocket, message, sizeof(message), 0, (struct sockaddr *) &serverAdd,&len);
        cout<<"Server: "<<message<<endl;

        if(strncmp(message,"exit",4)==0)
        {
            cout<<"Exiting chat from client side"<<endl;
            break;
        }
    }

}

int main()
{
    int clientSocket;
    struct sockaddr_in serverAdd;
    socklen_t add_size;

    //Creating client socket
    clientSocket= socket(AF_INET,SOCK_DGRAM,0);

    if (clientSocket== -1) 
    {
        cout<<"socket creation failed"<<endl;
        exit(0);
    }
    else
        cout<<"Socket successfully created"<<endl;

    //Assigining server address
    serverAdd.sin_family= AF_INET;
    serverAdd.sin_port= htons(1234);
    serverAdd.sin_addr.s_addr= inet_addr("127.0.0.1");
    memset(serverAdd.sin_zero,'\0',sizeof(serverAdd.sin_zero));

    //calling chatting function
    clientChat(clientSocket,serverAdd);

    //closing the socket
    close(clientSocket);

    return 0;
}