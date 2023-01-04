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
#include <pthread.h>
#include <semaphore.h>
#include<time.h>
sem_t var1,var2;
pthread_t pid;
pthread_t writerThread[50];
pthread_t readerThread[50];

int count = 0;

void* reader(void* para)
{

    sem_wait(&var1);
    count++;

    if(count==1)
        sem_wait(&var2);

    sem_post(&var1);

    printf("\n%d reader is inside", count);

    sleep(2);

    sem_wait(&var1);
    count--;

    if(count==0)
        sem_post(&var2);
    
    printf("\n%d Reader is leaving",count + 1);
    pthread_exit(NULL);
}


void* writer(void* para)
{
    printf("\nWriter is trying to enter");
 
    // Lock the semaphore
    sem_wait(&var2);
 
    printf("\nWriter has entered");
 
    // Unlock the semaphore
    sem_post(&var2);
 
    printf("\nClinet is leaving");
    pthread_exit(NULL);
}

int main()
{
    struct sockaddr_in behav,behav2;
    struct sockaddr_storage Storage;
    register int _server,con,newSocket;


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

     if(listen(_server,10) < 0)
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }

    pthread_t pid[20];
    socklen_t sizeBehav;
    sizeBehav = sizeof(struct sockaddr_in);
    char *ip;
    int i =0;
    char store[256],store2[256],store3[100];
    clock_t start_t,end_t;
    double total_t;
    double total;
    while (1)
    {
        int acc = accept(_server,(struct sockaddr*)&behav2,&sizeBehav);
        int choice = 0;
        start_t = clock();
        recv(newSocket,&choice,sizeof(choice),0);
        if(choice==1)
        {
            if(pthread_create(&readerThread[i++],NULL,reader,&acc)!=0)
            {
                printf("Failed to create thread\n");
            }
        }
        else if(choice==2)
        {
            if(pthread_create(&writerThread[i++],NULL,writer,&acc)!=0)
            {
                printf("Failed to create thread\n");
            }
        }
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
        if(i>=10)
        {
            i =0;
            while(i < 10)
            {
                pthread_join(writerThread[i++],
                             NULL);
                pthread_join(readerThread[i++],
                             NULL);
            }
            i=0;
        }
        i++;
    }
    
}