//Copyright (c) 2019 Jason Graalum
//
// Simple thread to print "hello world"
//
#include <stdio.h>
#include <stdlib.h>

void *hello_msg(void *vargp) {
  printf("Hello, world!\n");
  return NULL;
}
