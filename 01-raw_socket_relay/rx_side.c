#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "raw_sock.h"

#define UDP_PORT 12306
#define RAW_PORT 13333 //magic number
#define BUFLEN   65536

static const int ENABLE = 1;
// static const int DISABLE = 0;
static const int MAXSIZE = 256*1024;

// for transmitting
int set_rawsocket(void)
{
    int sock;

    if( (sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0)
    {
        perror("Failed to create raw socket");
        return -1;
    }

    // NOTE: not necessary
    /* if(setsockopt(sock, SOL_SOCKET, SO_DONTROUTE, &ENABLE, sizeof(int)) < 0)
    {
        perror("Failed to set raw_socket DONTROUTE");
        return -1;
    } */

    return sock;
}

// for receiving
int set_udpsocket(void)
{
    int sock;
    struct sockaddr_in si_me;

    if( (sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("Failed to create udp socket");
        return -1;
    }

    if(setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &MAXSIZE, sizeof(int)) < 0)
    {
        perror("Failed to set udp_socket RECVBUF_SIZE");
        return -1;
    }

    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(UDP_PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock, (struct sockaddr *)&si_me, sizeof(struct sockaddr)) < 0)
    {
        perror("Failed to set udp_socket binding");
        return -1;
    }

    return sock;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in si_local;
    char buf[BUFLEN];
    int rawfd, udpfd, recv_len;
    struct iphdr *iph;
    // struct tcphdr *tcph;
    // struct udphdr *udph;

    if ( ((rawfd = set_rawsocket()) < 0) || ((udpfd = set_udpsocket()) < 0) )
    {
        exit(1);
    }

    memset((char *) &si_local, 0, sizeof(si_local));
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(RAW_PORT);
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        if ( (recv_len=recvfrom(udpfd, buf, BUFLEN, 0, NULL, NULL)) < 0)
        {
            perror("udp_socket receive error");
            break;
        }
        else
        {
            iph = (struct iphdr *)buf;
            *(GET_DESTP(iph)) = htons(RAW_PORT);
            // iph->daddr = inet_addr("192.168.1.14");
            // iph->check = 0;

            if (sendto(rawfd, buf, recv_len, 0, 
                (struct sockaddr *)&si_local, sizeof(struct sockaddr)) < 0)
            {
                perror("raw_socket send error");
                break;
            }

            printf("I AM THERE\n");
        }
    }

    return 0;
}
