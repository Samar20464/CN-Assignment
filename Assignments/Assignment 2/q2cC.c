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



void* clinetThread(void *args)
{
    printf("\nCreating Client\n");  


    int client_req = *((int*) args);
    register int _server, len;
    struct sockaddr_in behav;
    printf("\nCreating Clinet \n");

    if ((_server = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Error in creating clinet");
        exit(1);
    }
    printf("\nSender Clinet Created\n");


    behav.sin_family = AF_INET;
    behav.sin_addr.s_addr = INADDR_ANY;
    behav.sin_addr.s_addr = inet_addr("127.0.0.1");
    behav.sin_port = htons(1200);

    socklen_t sizeBehav= sizeof behav;
    char *ip;
    int i =0;
    char buffer1[256],buffer2[256],buffer3[256];
    int conn = connect(_server,(struct sockaddr*)&behav,sizeof behav);
        if(conn<0)
        {
            perror("Failed");
            exit(EXIT_FAILURE);
        }
    while(1)
    {

        strcpy(buffer2, "Hello");
        send(_server, buffer2, 256, 0);
        recv(_server, buffer1, 256, 0);
        recv(_server, buffer3, 256, 0);
        printf("Port is : %s\n Ip is %s\n", buffer1,buffer3);
    }

    close(_server);
    pthread_exit(NULL);

}



int main()
{
    printf("1.Option 1\n");
    printf("2. Option 2\n");


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