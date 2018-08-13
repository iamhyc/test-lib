#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <net/ethernet.h>
#include "raw_sock.h"

#define RAW_PORT 12222
#define UDP_PORT 12306
#define BUFLEN   65536

// static const int ENABLE = 1;
// static const int DISABLE = 0;
static const int SI_SIZE = sizeof(struct sockaddr_in);

// for receiving
int set_rawsocket(void)
{
    int sock;

    if( (sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) < 0)
    {
        perror("Failed to create raw socket");
        return -1;
    }

    return sock;
}

// for transmitting
int set_udpsocket(void)
{
    int sock;

    if( (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("Failed to create udp socket");
        return -1;
    }

    return sock;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in si_local;
    int rawfd, udpfd, recv_len; 
    char buf[BUFLEN];
    socklen_t saddr_len;
    struct sockaddr saddr;
    // struct ethhdr *eth;
    struct iphdr *iph;

    if ( ((rawfd = set_rawsocket()) < 0) || ((udpfd = set_udpsocket()) < 0) )
    {
        exit(1);
    }

    memset((char *) &si_local, 0, sizeof(si_local));
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(UDP_PORT);
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);

    while(1)
    {
        // memset(buf, 0, sizeof(buf));
        if ( (recv_len=recvfrom(rawfd, buf, BUFLEN, 0, &saddr, &saddr_len)) < 0)
        {
            perror("raw_socket receive error");
            break;
        }
        else
        {
            iph = (struct iphdr*)buf;

            if (GET_DEST(iph)==RAW_PORT)
            {
                if (sendto(udpfd, iph, recv_len, 0, (struct sockaddr *)&si_local, SI_SIZE) < 0)
                {
                    perror("udp_socket send error");
                    break;
                }
                printf("I AM HERE\n");
            }
        }
    }

    return 0;
}
