#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#define q1S "socket"


int main()
{
    struct sockaddr_un behav,behav2;
    register int _server,con;


    printf("\nCreating Server \n");
    if ((_server = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    {
        perror("Server: Socket acquiration failed");
        exit(EXIT_FAILURE);
    }
    printf("\nSender Server Created\n");

    behav.sun_family = AF_UNIX;
    strcpy(behav.sun_path, q1S);
    unlink(q1S);

    printf("\nBinding\n");
    if (bind(_server, (struct sockaddr *)&behav, sizeof(struct sockaddr_un)) < 0)
    {
        perror("Unsuccessfull");
        exit(EXIT_FAILURE);
    }
    printf("Binding\n");


     if(listen(_server,3) < 0)
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }

    int len2;

    if((con = accept(_server,(struct sockaddr*)&behav2,&len2)) == -1)
    {
        perror("Failed");
        exit(EXIT_FAILURE);
    }


    printf("Server Connect sucessfully\n");
    int i =0;
    while (i<= 20)
    {
        printf("%d\n",i);
        send(con,&i,sizeof(i),0);

        if(read(con,&i,sizeof(i))== -1)
        {
            perror("Failed");
            exit(EXIT_FAILURE);
        }
        i+=1;
    }

    close(_server);


}