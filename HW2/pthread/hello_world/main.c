// Copyright (c) 2019 Jason Graalum
//
// Simple example of pthreads
//
// Taken from CS531 Slides
//
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *hello_msg(void *vargp);

int main()
{
    pthread_t t_id;
    
    pthread_create(&t_id, NULL, hello_msg, NULL);
    pthread_join(t_id, NULL);
    exit(0);
}


