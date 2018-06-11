
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>

#define DST_PORT 13333 //magic number
#define BUFLEN   1024

static const int ENABLE = 1;
static const int DISABLE = 0;
static const int SI_SIZE = sizeof(struct sockaddr_in);
static const int MAXSIZE = 256*1024;

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
    si_me.sin_port = htons(RECV_PORT);
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
    int sock, recvlen;
    struct timeval tstamp;

    if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("Failed to create UDP socket");
        exit(1);
    }

    memset((char *) &si_local, 0, sizeof(si_local));
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(DST_PORT);
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&si_local, sizeof(si_local)) < 0)
    {
        perror("Failed to bind UDP socked");
        exit(1);
    }

    while(1)
    {
        memset(buf, 0, sizeof(buf));
        if ( (recvlen=recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr*)&si_remote, &slen)) < 0)
        {
            perror("recv error");
        }
        else{
            gettimeofday(&tstamp, NULL);
            //display content here
            printf("%s[%ld.%06ld]\n", buf, tstamp.tv_sec, tstamp.tv_usec);
        }
    }

    return 0;
}
