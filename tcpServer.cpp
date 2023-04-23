#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

//chatting function
void serverChat(int newSocket)
{
    char message[1024];
    int n;

    for (;;)
    {
        bzero(message,1024);

        //Receving message from client
        recv(newSocket,message, sizeof(message),0);

        //Printing clinet's message
        cout<<"Client: "<<message<<endl;
        bzero(message,1024);
        cout<<"Enter your message: ";
        n=0;
        while ((message[n++] = getchar()) != '\n');

        //sending message to client
        send(newSocket,message,sizeof(message),0);

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
    struct sockaddr_storage serverStorage;
    socklen_t add_size;

    //creating server socket
    welcomeSocket=socket(AF_INET,SOCK_STREAM,0);

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

    //Binding address structure to the socket
    if(bind(welcomeSocket,(struct sockaddr*)&serverAdd,sizeof(serverAdd))!=0)
    {
        cout<<"Socket binding failed"<<endl;
        exit(0);
    }
    else
        cout<<"Socket successfully binded"<<endl;

    //Listening to the socket
    if(listen(welcomeSocket,5)!=0)
    {
        cout<<"Listen failed"<<endl;
        exit(0);    
    }
    else
        cout<<"Server listening"<<endl;

    //Accepting and creating a new socket
    add_size=sizeof(serverStorage);
    newSocket=accept(welcomeSocket,(struct sockaddr*)&serverStorage,&add_size);

    if(newSocket<0)
    {
        cout<<"Server accept failed"<<endl;
        exit(0);
    }
    else
        cout<<"Server accepted the client"<<endl;

    //calling chatting function
    serverChat(newSocket);

    //closing the socket
    close(welcomeSocket);
    return 0;
}