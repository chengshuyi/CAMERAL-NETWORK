#ifndef DEBUG_H
#define DEBUG_H

#include "stdio.h"

#define DEBUG 0

#define debug_print(fmt, ...)                 \
    do                                        \
    {                                         \
        if (DEBUG)                            \
            printf("File: " __FILE__          \
                   ", Line: %05d: " fmt "\n", \
                   __LINE__, ##__VA_ARGS__);  \
    } while (0)

#endif

/*
not clear for \n and /n.
*/

