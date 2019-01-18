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

void main ( int argc, char **argv ) {

  if(argc < 2) { 
    printf("Usage: %s n k\n",argv[0]); 
    exit(-1);
  }

  pthread_t *tid;

  typedef unsigned long data_t;
  volatile data_t global_sum;

  size_t number = (size_t) atoi(argv[1]);
  size_t nthreads = (size_t) atoi(argv[2]);

  size_t nelems_per_thread = number/nthreads;

  printf("number = %ld\tthreads = %ld\telems_per_thread = %ld\n", number, nthreads, nelems_per_thread);
}

