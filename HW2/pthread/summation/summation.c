//Copyright (c) 2019 Jason Graalum
// // Summation routine usig pthreads
//
// CS 531 Witer 2019
// Jason Graalum
// // January 17, 2019
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *sum_thread(void *vargp);

typedef unsigned long data_t;
extern volatile data_t global_sum;
extern size_t nelems_per_thread;
extern size_t nthreads;
extern data_t *sum;

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
  sum = (data_t *) malloc(sizeof(data_t)*nthreads);

  int *myid;
  myid = (int *)malloc(sizeof(int)*nthreads);

  printf("number = %ld\tthreads = %ld\telems_per_thread = %ld\n", number, nthreads, nelems_per_thread);

/* Create threads and wait for them to finish */
  for (int i = 0; i < nthreads; i++) {
    myid[i] = i;
    pthread_create(&tid[i], NULL, sum_thread, &myid[i]);
  }
  for (int i = 0; i < nthreads; i++)
  {
    pthread_join(tid[i], NULL);
    global_sum += sum[i]; 
    printf("Thread %d joined with %ld\n",i,sum[i]);
  }

  for (int i = nthreads * nelems_per_thread; i < number; i++)
    global_sum += i;

  printf("Global Sum = %ld\n", global_sum);

}
