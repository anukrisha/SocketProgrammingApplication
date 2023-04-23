#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;


int main()
{
    int welcomeSocket;
    int newSocket;
    struct sockaddr_in serverAdd;
    struct sockaddr_storage serverStorage;
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

    //Binding address structure to the socket
    if(bind(welcomeSocket,(struct sockaddr*)&serverAdd,sizeof(serverAdd))!=0)
    {
        cout<<"Socket binding failed"<<endl;
        exit(0);
    }
    else
        cout<<"Socket successfully binded"<<endl;



    return 0;
}