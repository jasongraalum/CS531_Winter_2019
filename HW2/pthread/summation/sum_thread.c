//Copyright (c) 2019 Jason Graalum
//
// Summation routine usig pthreads
//
// CS 531 Witer 2019
// Jason Graalum
// 
// January 17, 2019
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef unsigned long data_t;
volatile data_t global_sum;
size_t nelems_per_thread;
size_t nthreads;

data_t *sum;

void *sum_thread(void *vargp)
{
    int myid = *((int *)vargp);
    size_t start = myid * nelems_per_thread;
    size_t end = start + nelems_per_thread;
    size_t i;
    sum[myid] = 0;
    for (i = start; i < end; i++) {
       sum[myid] += i;
    }

    return &sum;
}



