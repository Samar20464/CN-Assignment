#include <arpa/inet.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define q1S "socket"
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
 
    printf("\nWriter is leaving");
    pthread_exit(NULL);
}


int main()
{
    struct sockaddr_un behav;
    struct sockaddr_storage Storage;
    int _server,newSocket;

    socklen_t addr_size;
    sem_init(&var1, 0, 1);
    sem_init(&var2, 0, 1);

    printf("\nCreating Server \n");
    if ((_server = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Server: Socket acquiration failed");
        exit(1);
    }
    printf("\nSender Server Created\n");

    behav.sun_family = AF_UNIX;
    strcpy(behav.sun_path, q1S);
    unlink(q1S);

    printf("\nBinding\n");
    if (bind(_server, (struct sockaddr *)&behav, sizeof(struct sockaddr_un)) < 0)
    {
        perror("Unsuccessfull");
        exit(1);
    }
    printf("Binding\n");


     if(listen(_server,10) < 0)
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }


    pthread_t pid[20];
    int i = 0;
    while(1)
    {
        addr_size = sizeof(Storage);
        newSocket = accept(_server,(struct sockaddr*)&Storage,&addr_size);
        int choice = 0;
        recv(newSocket,&choice,sizeof(choice),0);
        if(choice==1)
        {
            if(pthread_create(&readerThread[i++],NULL,reader,&newSocket)!=0)
            {
                printf("Failed to create thread\n");
            }
        }
        else if(choice==2)
        {
            if(pthread_create(&writerThread[i++],NULL,writer,&newSocket)!=0)
            {
                printf("Failed to create thread\n");
            }
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

    }
}

