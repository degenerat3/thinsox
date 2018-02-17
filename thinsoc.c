#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>
#include <pthread.h>
#include <string.h>

#define PORT 668


//listener
void *listenSOX(int socketFD)
{
    while(1)
    {
        int msglen = sizeof(unsigned char)*500;
        unsigned char *msg = (unsigned char *)malloc(msglen);
        int lengthWeGot = recv(socketFD, msg, msglen, 0);
        if (lengthWeGot>0)
        {
            printf("l %i sock got %s\n", lengthWeGot, msg);//fiz later
        }
        free(msg);
        usleep(1000);
    }
    return NULL;
}

int sendSOX(unsigned char *msg, unsigned char *from, int socketFD)
{
    int sizeOfPayload = sizeof(unsigned char)*8+strlen(msg)+1;
    unsigned char *payload = (unsigned char *)malloc(sizeOfPayload); //SOX | NAME0 | MSG...
    memcpy(payload, "SOX",3);
    memcpy(payload+3, from, strlen(from));
    memcpy(payload+3+strlen(from), msg, strlen(msg));
    printf("SENDING %i : %s \n", sizeOfPayload, payload);
    int totalBytesSent=0;
    for (int i = 0; i < 254; ++i)
    {
        char *destinationIP = (char*)malloc(15 * sizeof(char));
        sprintf(destinationIP, "192.168.1.%i", i);
        struct sockaddr_in dest;
        dest.sin_family = AF_INET;
        dest.sin_addr.s_addr = inet_addr(destinationIP);
        dest.sin_port = htons(PORT);
        totalBytesSent += sendto(socketFD, payload, sizeOfPayload, 0, (struct sockaddr *)&dest, sizeof(dest));
        free(destinationIP);
    }
    free(payload);
    return totalBytesSent;
}


int main(int argc, char **argv) 
{
    int server_fd, new_socket, valread;
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    //get network interfaces
    struct sockaddr_in myaddr;
    struct in_addr addr;
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //myaddr.sin_addr.s_addr = inet_addr("192.168.1.107");
    myaddr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&myaddr, sizeof(myaddr))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    int blasts = 0;

    pthread_t listener_thread;
    if(pthread_create(&listener_thread, NULL, listenSOX, server_fd)) {
        fprintf(stderr, "Error creating thread\n");
        exit(EXIT_FAILURE);
    }

    char *iname = (unsigned char *)malloc(sizeof(unsigned char)*5);
    printf("What is you name?(5 characters max): ");
    fgets(iname, 5, stdin);
    char *name = (unsigned char *)malloc(sizeof(unsigned char)*5);
    memcpy(name,iname,5);

    while(1)
    {
        //input
        unsigned char *msg = "This is a sample message that someone might send (:";
        unsigned char *name = "Bobby";
        sendSOX(msg, name, server_fd);
        sleep(1);
    }
}