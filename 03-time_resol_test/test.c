#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "timing_usr.h"

int main(int argc, char const *argv[])
{
    timing_unit tt;

    while(1)
    {
        timing_start(tt);
        usleep(1E3);
        timing_stop(tt);

        printf("%ld.%06ld\n", tt.result.tv_sec, tt.result.tv_usec);
        usleep(1E6);
    }
    
    return 0;
}
