#include <stdio.h> 
#include <string.h>    
#include <stdlib.h> 
#include<stdbool.h>
#include <errno.h> 
#include <unistd.h>   
#include <arpa/inet.h>    
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h>
#include <sys/select.h>
#include<time.h>
int main(int argc , char *argv[])  
{
    struct sockaddr_in behav,behav2;
    register int _server,con;

    int client_socket[10];
    fd_set readDesc;

    for(int i =0;i<10;i++)
    {
        client_socket[i] =0;
    }

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

    socklen_t sizeBehav;
    int max,sd,activity,acc,valread;
    sizeBehav = sizeof(struct sockaddr_in);
    char store[256],store2[256],store3[100],buffer[1025];
    clock_t start_t,end_t;
    double total_t;
    double total;
    while(1)
    {
        FD_ZERO(&readDesc);
        FD_SET(_server,&readDesc);
        max = _server;

        for(int i =0;i<10;i++)
        {
            sd = client_socket[i];
            if(sd>0)
                FD_SET(sd,&readDesc);
            if(sd>max)
                max = sd;    
        }

        activity = select(max + 1,&readDesc,NULL,NULL,NULL);

        if((activity < 0) && (errno!= EINTR))
        {
            perror("error");
        }

        if(FD_ISSET(_server,&readDesc))
        {
            acc = accept(_server,(struct sockaddr*)&behav2,&sizeBehav);
            start_t = clock();
            printf("Connection Established\n");
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
            for(int i =0;i<10;i++)
            {
                if(client_socket[i]==0)
                {
                    client_socket[i] = acc;
                    break;
                }
            }
        }

       
    }
}
