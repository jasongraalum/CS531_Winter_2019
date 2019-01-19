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

void *sum_thread(void *vargp);

typedef unsigned long data_t;
volatile data_t global_sum;
size_t nelems_per_thread;
size_t nthreads;

int main ( int argc, char **argv ) {

  global_sum = 0;

  if(argc < 2) { 
    printf("Usage: %s n k\n",argv[0]); 
    exit(-1);
  }

  pthread_t *tid;

  size_t number = (size_t) atoi(argv[1]);
  size_t nthreads = (size_t) atoi(argv[2]);

  nelems_per_thread = number/nthreads;

  tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);

  int *myid;
  myid = (int *)malloc(sizeof(int)*nthreads);

  printf("number = %ld\tthreads = %ld\telems_per_thread = %ld\n", number, nthreads, nelems_per_thread);

/* Create threads and wait for them to finish */
  for (int i = 0; i < nthreads; i++) {
    myid[i] = i;
    Pthread_create(&tid[i], NULL, sum_thread, &myid[i]);
  }
  for (int i = 0; i < nthreads; i++)
    Pthread_join(tid[i], NULL);

}

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



