//Copyright (c) 2019 Jason Graalum
//
// Summation routine using pthreads
//
// CS 531 Witer 2019
// Jason Graalum
// 
// January 17, 2019
//

#include "summation.h"

void *pth_1_gvar_cont_sum(void *vargp)
{
    int myid = *((int *)vargp);
    size_t start = myid * nelems_per_thread;
    size_t end = start + nelems_per_thread;
    size_t i;
    data_t local_sum = 0;
    for (i = start; i < end; i++) {
        local_sum += i;

    }
// Add locks
    global_sum += local_sum;
    return NULL;
}



