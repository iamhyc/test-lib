#ifndef __TIMING_USR_H__
#define __TIMING_USR_H__

#include <stdint.h>

uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

typedef struct
{
    uint64_t marker[2];
    uint64_t result;
}timing_unit;

#define timing_start(x) \
    do { \
        x.marker[0] = rdtsc(); \
    } while(0)

#define timing_stop(x) \
    do { \
        x.marker[1] = rdtsc(); \
        x.result = x.marker[1] - x.marker[0]; \
    } while(0)

#endif