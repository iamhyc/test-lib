
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

int main(int argc, char const *argv[])
{
    uint64_t t0, t1;

    while(1)
    {
        t0 = rdtsc();
        // printf("%lld\n", t0);
        
        usleep(1E3);

        t1 = rdtsc();
        printf("%lld\n", t1-t0);

        usleep(1E6);
    }
    
    return 0;
}
