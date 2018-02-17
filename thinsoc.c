#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <errno.h>

#define PORT 668

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
    while(1)
    {
        char *destinationIP = (char*)malloc(15 * sizeof(char));
        sprintf(destinationIP, "192.168.1.%i", count%253);
        struct sockaddr_in dest;
        dest.sin_family = AF_INET;
        dest.sin_addr.s_addr = inet_addr(destinationIP);
        dest.sin_port = htons(PORT);
        char *payload = "HELLOHEL";
        int bytesSent = sendto(server_fd, payload, sizeof(payload), 0, (struct sockaddr *)&dest, sizeof(dest));
        printf("%i sent\n",bytesSent);
        printf("ERROR: %s\n", strerror(errno));
        int msglen = sizeof(unsigned char)*100;
        unsigned char *msg = (unsigned char *)malloc(msglen);
        // int lengthWeGot = recv(server_fd, msg, msglen, 0);
        // if (lengthWeGot>0)
        // {
        //     printf("%s\n", msg);
        // } else {
        //     printf("we got nothing\n");
        // }
        free(msg);
        free(destinationIP);
        count+=1;
        if (count%253==0)
        {
            blasts +=1 ;
            printf("blasted %i\n",blasts);
            sleep(1);
        }
    }
}