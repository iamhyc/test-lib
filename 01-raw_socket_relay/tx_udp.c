#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>

#define DST_PORT 12435 //magic number
#define BUFLEN   1024

int main(int argc, char const *argv[])
{
    struct sockaddr_in si_local;
    int cnt, sock, slen=sizeof(struct sockaddr);
    struct timeval tstamp;
    char buf[BUFLEN];

    if ((sock=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("Failed to create UDP socket");
        exit(1);
    }

    memset((char *) &si_local, 0, sizeof(si_local));
    si_local.sin_family = AF_INET;
    si_local.sin_port = htons(DST_PORT);
    si_local.sin_addr.s_addr = htonl(INADDR_ANY);

    while(1)
    {
        usleep(0.1E6);

        cnt ++;
        gettimeofday(&tstamp, NULL);
        sprintf(buf, "[SEQ %4d][TIM %ld.%06ld]", cnt, tstamp.tv_sec, tstamp.tv_usec);
        if (sendto(sock, buf, BUFLEN, 0, (struct sockaddr *)&si_local, slen) < 0)
        {
            perror(buf);
            continue;
        }
    }

    return 0;
}
