#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#define q1C "socket"

int main()
{
   

    printf("\nCreating Client\n");

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
    printf("Sucessful\n");
    int i =0;
    while(i<=19)
    {
       

        if(read(_server,&i,sizeof(i)) ==- 1)
        {
             perror("Failed");
             exit(EXIT_FAILURE);
        }

        printf("%d ",i);

        printf("Packet Recived\n");

        sleep(1);

        int length;

        if((length == send(_server,&i,sizeof(i),0)) < 0)
        {
            perror("Failed");
            exit(EXIT_FAILURE);
        }
    }

    close(_server);
}