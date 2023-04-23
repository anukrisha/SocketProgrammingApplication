#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

//chatting function
void serverChat(int serverSocket, struct sockaddr_in clientAdd )
{
    char message[1024];
    int n;
    socklen_t add_size;
    add_size= sizeof(clientAdd);

    for (;;)
    {
        bzero(message,1024);

        //Receving message from client
        recvfrom(serverSocket, message, sizeof(message), 0, ( struct sockaddr *) &clientAdd,&add_size);

        //Printing client's message
        cout<<"Client: "<<message<<endl;
        bzero(message,1024);
        cout<<"Enter your message: ";
        n=0;
        while ((message[n++] = getchar()) != '\n');

        //sending message to client
        sendto(serverSocket, message, sizeof(message), 0, (const struct sockaddr *) &clientAdd,add_size);

        if(strncmp(message,"exit",4)==0)
        {
            cout<<"Exiting chat from server side"<<endl;
            break;
        }

    }

}

int main()
{
    int welcomeSocket;
    int newSocket;
    struct sockaddr_in serverAdd;
    struct sockaddr_in clientAdd;
    socklen_t add_size;

    //creating server socket
    welcomeSocket=socket(AF_INET,SOCK_DGRAM,0);

    if (welcomeSocket== -1) 
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

    memset(&clientAdd, 0, sizeof(clientAdd));

    //Binding address structure to the socket
    if(bind(welcomeSocket,(struct sockaddr*)&serverAdd,sizeof(serverAdd))!=0)
    {
        cout<<"Socket binding failed"<<endl;
        exit(0);
    }
    else
        cout<<"Socket successfully binded"<<endl;

    //calling chatting function
    serverChat(welcomeSocket,clientAdd);

    //closing the socket
    close(welcomeSocket);

    return 0;
}