#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define q1C "socket"

void* clinetThread(void *args)
{
    printf("\nCreating Client\n");  


    int client_req = *((int*) args);
    register int _server, len;
    struct sockaddr_un behav;
    if ((_server = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("\nAcquiration failed\n\n");
        exit(1);
    }

    printf("Client Created\n");


    behav.sun_family = AF_UNIX;
    strcpy(behav.sun_path, q1C);
    int log = -1;
    len = sizeof(behav.sun_family) + strlen(behav.sun_path);

    printf("Creating\n");
    if (connect(_server, (struct sockaddr *)&behav, len) < 0) 
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }

    send(_server,&client_req,sizeof(client_req),0);

    close(_server);
    pthread_exit(NULL);
}


int main()
{

    printf("1. Hey\n");
    printf("2. Hi\n");


    int choice;
    scanf("%d", &choice);
    pthread_t pid;


    switch (choice)
    {
    case 1:{
        int client_req = 1;
        pthread_create(&pid,NULL,clinetThread,&client_req);
        sleep(10);
    break;}
    case 2:{
        int client_req = 2;
        pthread_create(&pid,NULL,clinetThread,&client_req);
        sleep(10);
    break;}
    default:
        printf("Invalid Input\n");
        break;
    }
    printf("Connection Established\n");
    pthread_join(pid,NULL);

}