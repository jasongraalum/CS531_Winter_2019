//Copyright (c) 2019 Jason Graalum
//
// Summation routine usig pthreads
//
// CS 531 Witer 2019
// Jason Graalum
// 
// January 17, 2019
//

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *sum_thread(void *vargp)
{
    int myid = *((int *)vargp);
    size_t start = myid * nelems_per_thread;
    size_t end = start + nelems_per_thread;
    size_t i;
    for (i = start; i < end; i++) {
       global_sum += i;
}
    return NULL;
}



