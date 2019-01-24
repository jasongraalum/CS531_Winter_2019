
#ifndef _SUMMATION_H_
#define _SUMMATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include <gsl/gsl_statistics_double.h>



#ifdef SPACE_VARS
  #define LEVEL1_DCACHE_LINESIZE 64
#endif

#endif // _SUMMATION_H_

typedef unsigned long long data_t;
volatile data_t *global_sum;
#ifndef SP_SUM
    pthread_t *tid;
    void *sp_summation(void *vargp);
#endif

#ifdef PTH_1_GVR_CONT
void *pth_1_gvar_cont_sum(void *vargp);
#endif
#ifdef PTH_1_GVR_END
void *pth_1_gvar_end_sum(void *vargp);
#endif
#ifdef PTH_N_GVR
void *pth_nth_gvar_sum(void *vargp);
#endif

