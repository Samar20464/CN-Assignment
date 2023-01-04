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
#include<sys/poll.h>
#include<fcntl.h>
#include<sys/signal.h>
#include<sys/select.h>
#include <sys/ioctl.h>
#include<poll.h>
#include<time.h>
int main()
{
    struct sockaddr_in behav,behav2;
    register int _server,con;

    struct pollfd fds[100];
    int numfds = 0;
    int max_fd = 0;
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
    if (setsockopt(_server, SOL_SOCKET,SO_REUSEADDR, (&opt),
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
    sizeBehav = sizeof(struct sockaddr_in);
    char *ip;
    int i =0;
    char store[256],store2[256],store3[100];
    pid_t childpid;
    int acc,timeout;
    clock_t start_t,end_t;
    double total_t;
    double total;
    memset(&fds,0,sizeof(fds));

    fds[0].fd = _server;
    fds[0].events = POLLIN;
    numfds++;
    timeout = 3 * 60 * 1000;
    int curr_size,rc;
    
    while(1)
    {
        char ch;
        int index;
        int nread;

        printf("Waiting for client (%d total)...\n", numfds);

        poll(fds,numfds,timeout);
        for(index=0;index<numfds;index++)
        {
            if(fds[index].revents && POLLIN)
            {
                if(fds[index].fd == _server)
                {

                    acc = accept(_server,(struct sockaddr*)&behav2,&sizeBehav);
                    start_t = clock();
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
                    fds[numfds].fd = _server;
                    fds[numfds].events = POLLIN;
                    numfds++;
                    printf("Adding client on fd %d\n", _server);
                }
                else
                {
                    ioctl(fds[index].fd, FIONREAD, &nread);
                    if(nread==0)
                    {
                        close(fds[index].fd);
                        numfds--;
                        fds[index].events = 0;
                        printf("Removing client on fd %d\n", fds[index].fd);
                        fds[index].fd = -1;
                    }
                    else
                    {
                    read(fds[index].fd, &ch, 1);
                    printf("Serving client on fd %d\n", fds[index].fd);
                    ch++;
                    write(fds[index].fd, &ch, 1);
                    }
                }
            }
        }
    }

}
