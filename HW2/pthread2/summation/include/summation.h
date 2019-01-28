
#ifndef _SUMMATION_H_
#define _SUMMATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include <gsl/gsl_statistics_double.h>

void single_thread(size_t, size_t, char);
void multi_thread_global_var(size_t, size_t, size_t, size_t, char);
void multi_thread_global_fine_mutex(size_t, size_t, size_t, size_t, char);
void multi_thread_local_var(size_t, size_t, size_t, size_t, char);

#define DEBUG_MSG(STRING) 
#endif // _SUMMATION_H_

#define LEVEL1_DCACHE_LINESIZE 64
typedef unsigned long long data_t;

typedef struct shared_data_t {
    data_t val;
    pthread_mutex_t lock;
} shared_data_t;

shared_data_t global_sum;
shared_data_t nelems_per_thread;;

size_t nthreads;

typedef struct thread_data_t {
    data_t start_num;
    data_t end_num;
    data_t sum;
} thread_data_t;



