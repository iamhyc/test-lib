
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define DST_PORT 13425 //magic number
#define BUFLEN   1024

int main(int argc, char const *argv[])
{
    struct sockaddr_in si_local, si_remote;
    char buf[BUFLEN];
    socklen_t slen;
    int sock, recvlen;

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
        if ( (recvlen=recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr*)&si_remote, &slen)) < 0)
        {
            perror("recv error");
        }
        else{
            //cope with data here
            continue;
        }
    }

    return 0;
}
