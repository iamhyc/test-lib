#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "timing_usr.h"

int main(int argc, char const *argv[])
{
    uint64_t t0, t1;
    timing_unit tt;

    while(1)
    {
        timing_start(tt);
        usleep(1E3);
        timing_stop(tt);

        printf("%ld\n", tt.result);
        usleep(1E6);
    }
    
    return 0;
}
