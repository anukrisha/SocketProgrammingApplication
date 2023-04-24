#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<fstream>
#define SIZE 1024
using namespace std;

//Function for sending file
void send_file(FILE *fp, int sockfd)
{
  int n;
  char data[SIZE] = {0};
 
  while(fgets(data, SIZE, fp) != NULL) 
  {
    if (send(sockfd, data, sizeof(data), 0) == -1) 
    {
      cout<<"Error in sending file"<<endl;
      exit(0);
    }
    bzero(data, SIZE);
  }
}

void clientfile(int clientSocket)
{

    //char *filename= new char[1024];
    cout<<"Input the file name: ";
    //fgets(filename, 1024, stdin);
    string filename;
    cin>>filename;

    FILE *fp;
    fp = fopen(filename.c_str(), "r");
    if (fp == NULL) 
    {
        cout<<"Error in reading file."<<endl;
        exit(0);
    }
 
    send_file(fp, clientSocket);
    cout<<"File sent successfully"<<endl;
 
}

//chatting function
void clientChat(int clientSocket)
{

    
    char buffer[1024];
    int n;

    for(;;)
    {
        bzero(buffer,1024);
        n=0;

        cout<<"Enter your message: ";
        while ((buffer[n++] = getchar()) != '\n');

        //sending message to server
        send(clientSocket,buffer,sizeof(buffer),0);
        bzero(buffer,sizeof(buffer));

        recv(clientSocket,buffer,sizeof(buffer),0);
        cout<<"Server: "<<buffer<<endl;
        

        if(strncmp(buffer,"exit",4)==0)
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
    clientSocket= socket(AF_INET,SOCK_STREAM,0);

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

    //connecting socket to server
    add_size= sizeof(serverAdd);
    if(connect(clientSocket,(struct sockaddr *)&serverAdd,add_size)!=0)
    {
        cout<<"Connection with server failed"<<endl;
        exit(0);
    }
    else
        cout<<"Connected to the server"<<endl;
    
    char c[1024];
    int choice;
 
    recv(clientSocket,c,sizeof(c),0);
    cout<<c;
    cin>>choice;
    send(clientSocket,&choice,sizeof(choice),0);

    if (choice==0)
    {
       
        //calling chatting function
        clientChat(clientSocket);
    }
    else if(choice==1)
    {
        clientfile(clientSocket);
    }

    
    //closing the socket
    close(clientSocket);


    return 0;
}