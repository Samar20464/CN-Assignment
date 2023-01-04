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
#include <resolv.h>
#include <sys/epoll.h>
#include<time.h>
#define MAX_EVENTS 10
#define MAX_CONNECTIONS 100
int main()
{
    struct sockaddr_in behav,behav2;
    register int _server,con;

    struct epoll_event events[MAX_CONNECTIONS];

    printf("\nCreating Server \n");

    if ((_server = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error in creating server");
        exit(1);
    }
    printf("\nSender Server Created\n");

    int epoC,s;
    epoC= epoll_create(MAX_EVENTS);
    struct epoll_event event;
    struct epoll_event *pollfds;
    event.events = EPOLLIN; 
    event.data.fd = _server;



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
    int bin;
    bin = bind(_server, (struct sockaddr *)&behav, sizeof(behav));
    if (bin < 0)
    {
        perror("Unsuccessfull");
        exit(1);
    }
    printf("Binding Sucessfull\n");

     if(listen(_server,10) == 0)
    {
       printf("Listening...\n\n");
    }

    s = epoll_ctl(epoC, EPOLL_CTL_ADD, _server, &event);

    if(s==-1)
    {
        perror("Error");
        abort();
    }

    pollfds = malloc(sizeof(struct epoll_event)*MAX_EVENTS);
    for(int i =0;i<MAX_CONNECTIONS;i++)
    {
        pollfds[i].data.fd =0;
        pollfds[i].events =   EPOLLIN;
    }
    socklen_t sizeBehav;
    char *ip;
    int i =0;
    char store[256],store2[256],store3[100],buf[100];
    pid_t childpid;
    int acc,ready,strLen;
    clock_t start_t,end_t;
    double total_t;
    double total;
    while(1)
    {
        ready = epoll_wait(epoC,pollfds, MAX_EVENTS, -1);

        if(ready ==0)
        {
            break;
        }
        start_t = clock();
        puts("return epoll_wait");
        for(int i =0;i<ready;i++)
        {
            if(pollfds[i].data.fd == _server)
            {
                acc = accept(_server,(struct sockaddr*)&behav2,&sizeBehav);
                sizeBehav = sizeof(behav2);
                event.events=EPOLLIN;
                event.data.fd=acc;
                epoll_ctl(epoC, EPOLL_CTL_ADD, acc, &event);
                printf("connected client: %d\n", acc-4);
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(behav2.sin_addr), ip, INET_ADDRSTRLEN);

                printf("Connection established with IP : %s and PORT : %d\n",ip, ntohs(behav2.sin_port));
                int port = ntohs(behav2.sin_port);
                recv(acc, store2, 256, 0);
                printf("Client : %s\n", store2);
                strcpy(store, ip);
                sprintf(store3,"%d",port);
                send(acc,store3,256,0);
                send(acc, store, 256, 0);  
                end_t = clock();
                total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                
                if(total_t>0)
                {
                    total+= total_t;
                    printf("Total time is %f\n",total);
                    printf("Total time taken by CPU for this client: %f\n", total_t  );
                }
            }
            else{
                while(1)
                {
                    acc = accept(_server,(struct sockaddr*)&behav2,&sizeBehav);
                sizeBehav = sizeof(behav2);
                event.events=EPOLLIN;
                event.data.fd=acc;
                epoll_ctl(epoC, EPOLL_CTL_ADD, acc, &event);
                printf("connected client: %d\n", acc-4);
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(behav2.sin_addr), ip, INET_ADDRSTRLEN);

                printf("Connection established with IP : %s and PORT : %d\n",ip, ntohs(behav2.sin_port));
                int port = ntohs(behav2.sin_port);
                recv(acc, store2, 256, 0);
                printf("Client : %s\n", store2);
                strcpy(store, ip);
                sprintf(store3,"%d",port);
                send(acc,store3,256,0);
                send(acc, store, 256, 0);  
                end_t = clock();
                total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                
                if(total_t>0)
                {
                    total+= total_t;
                    printf("Total time is %f\n",total);
                    printf("Total time taken by CPU for this client: %f\n", total_t  );
                }
                }
                close(pollfds[i].data.fd);
                epoll_ctl(epoC, EPOLL_CTL_DEL, pollfds[i].data.fd, NULL);
                printf("closed client: %d\n", pollfds[i].data.fd);
            }
        }
        
    }
    close(_server);
    close(epoC);
}