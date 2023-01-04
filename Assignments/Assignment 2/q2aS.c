#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<netdb.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include <sys/types.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#include <stdio.h>  
#include <errno.h>  
#include <arpa/inet.h>  
#include <stdlib.h>  
int main()
{
    struct sockaddr_in behav,behav2;
    register int _server,con;


    printf("\nCreating Server \n");

    if ((_server = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error in creating server");
        exit(1);
    }
    printf("\nSender Server Created\n");


    behav.sin_family = AF_INET;
    behav.sin_addr.s_addr = INADDR_ANY;

    behav.sin_addr.s_addr = inet_addr("127.0.0.1");

    behav.sin_port = htons(1200);

    int opt = 1;
    if (setsockopt(_server, SOL_SOCKET,SO_REUSEADDR, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    printf("\nBinding\n");
    if (bind(_server, (struct sockaddr *)&behav, sizeof(behav)) < 0)
    {
        perror("Unsuccessfull");
        exit(1);
    }
    printf("Binding Sucessfull\n");

     if(listen(_server,1) < 0)
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }


    socklen_t sizeBehav;
    sizeBehav = sizeof(struct sockaddr_in);
    char *ip;
    int i =0;
    char store[256],store2[256],store3[100];
    bool run = true;
    while (run)
    {
        int acc = accept(_server,(struct sockaddr*)&behav2,&sizeBehav);
        
        printf("Connection Established\n");
        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(behav2.sin_addr), ip, INET_ADDRSTRLEN);

        printf("No: %d Connection established with IP : %s and PORT : %d\n",i+1,ip, ntohs(behav2.sin_port));
        int port = ntohs(behav2.sin_port);
        i++;
        recv(acc, store2, 256, 0);
        printf("Client : %s\n", store2);
        strcpy(store, ip);
        run = false;
        sprintf(store3,"%d",port);
        send(acc,store3,256,0);
        send(acc, store, 256, 0); 
    }
    
     close(_server);
}