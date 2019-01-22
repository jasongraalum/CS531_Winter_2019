//Copyright (c) 2019 Jason Graalum
// 
// Various Summation routines usig pthreads
//
// CS 531 Witer 2019
// Jason Graalum
// 
// January 17, 2019
//
// HW 2 - Part A.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

void *sum_thread_1(void *vargp);
void *sum_thread_2(void *vargp);

typedef unsigned long long data_t;
extern volatile data_t global_sum;
extern size_t nelems_per_thread;
extern size_t nthreads;
extern data_t *sum;

int main ( int argc, char **argv ) {

    struct timespec ts_start;
    struct timespec ts_end;

    clock_gettime(CLOCK_MONOTONIC, &ts_start);

    global_sum = 0;

    if(argc < 4) { 
        printf("Usage: %s mode n k\n",argv[0]); 
        printf("\tmode : s, 1, 2");
        exit(-1);
    }

    pthread_t *tid;

    int mode = (int) atoi(argv[1]);
    size_t number = (size_t) atoi(argv[2]);
    size_t nthreads = (size_t) atoi(argv[3]);

    nelems_per_thread = number/nthreads;

    tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    sum = (data_t *) malloc(sizeof(data_t)*nthreads);

    int *myid;
    myid = (int *)malloc(sizeof(int)*nthreads);

    printf("number = %ld\tthreads = %ld\telems_per_thread = %ld\n", number, nthreads, nelems_per_thread);

    /* Create threads and wait for them to finish */
    for (int i = 0; i < nthreads; i++) {
        myid[i] = i;
        switch (mode) {
            case 1: 
                pthread_create(&tid[i], NULL, sum_thread_1, &myid[i]);
                break;
            case 2: 
                pthread_create(&tid[i], NULL, sum_thread_2, &myid[i]);
                break;
            default:
                exit(-2);
        }
    }

    for (int i = 0; i < nthreads; i++)
    {
        pthread_join(tid[i], NULL);
        global_sum += sum[i]; 
        printf("Thread %d joined with %lld\n",i,sum[i]);
    }

    for (int i = nthreads * nelems_per_thread; i < number; i++)
        global_sum += i;

    printf("Global Sum = %lld\n", global_sum);
    clock_gettime(CLOCK_MONOTONIC, &ts_end);

    printf("%lld.%.9lld\n", (long long)ts_end.tv_sec-ts_start.tv_sec, (long long)ts_end.tv_nsec-ts_start.tv_nsec);
}
