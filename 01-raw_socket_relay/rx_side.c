
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>

#define UDP_PORT 12306
#define RAW_PORT 13333 //magic number
#define BUFLEN   1500

static const int ENABLE = 1;
// static const int DISABLE = 0;
static const int SI_SIZE = sizeof(struct sockaddr_in);
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

    if(setsockopt(sock, SOL_SOCKET, SO_DONTROUTE, &ENABLE, sizeof(int)) < 0)
    {
        perror("Failed to set raw_socket DONTROUTE");
        return -1;
    }

    return sock;
}

// for receiving
int set_udpsocket(void)
{
    int sock;
    struct sockaddr_in si_me;

    if( (sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
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
    if (bind(sock, (struct sockaddr *)&si_me, SI_SIZE) < 0)
    {
        perror("Failed to set udp_socket binding");
        return -1;
    }

    return sock;
}

int main(int argc, char const *argv[])
{
    struct sockaddr_in si_local, si_remote;
    char buf[BUFLEN];
    socklen_t slen;
    int rawfd, udpfd, recv_len;
    struct iphdr *iph;
    struct tcphdr *tcph;
    struct udphdr *udph;

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
        if ( (recv_len=recvfrom(udpfd, buf, BUFLEN, 0, (struct sockaddr*)&si_remote, &slen)) < 0)
        {
            perror("udp_socket receive error");
            break;
        }
        else
        {
            iph = (struct iphdr *)buf;
            iph->check = 0; //empty the IP checksum
            if(iph->protocol==IPPROTO_TCP)
            {
                tcph = (struct tcphdr *)(buf + iph->ihl*4);
                tcph->dest = htons(RAW_PORT);
                tcph->check = 0; //empty the TCP checksum
            }
            else
            {
                udph = (struct udphdr *)(buf + iph->ihl*4);
                udph->dest = htons(RAW_PORT);
                udph->check = 0; ////empty the UDP checksum
            }

            if (sendto(udpfd, buf, recv_len, 0, (struct sockaddr *)&si_local, slen) < 0)
            {
                perror("raw_socket send error");
                break;
            }
        }
    }

    return 0;
}
