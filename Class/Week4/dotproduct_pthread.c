/*****************************************************************************
 * FILE: mpithreads_serial.c
 * DESCRIPTION:
 *   This is a simple serial program that computes the dot product of two 
 *   vectors.  It is the first of four codes used to show the progression
 *   from a serial program to a hybrid MPI/Pthreads program.  The other
 *   relevant codes are:
 *      - mpithreads_threads.c  - A shared memory programming model using 
 *          Pthreads
 *      - mpithreads_mpi.c - A distributed memory programming model with MPI
 *      - mpithreads_both.c - A hybrid model that utilizes both MPI and 
 *          Pthreads to execute on systems that are comprised of clusters 
 *          of SMP's.
 * SOURCE: Vijay Sonnad, IBM
 * LAST REVISED:  01/29/09 Blaise Barney
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>


/*   
     The following structure contains the necessary information to allow the 
     function "dotprod" to access its input data and place its output so that 
     it can be accessed later. 
     */

typedef struct 
{
	double      *a;
	double      *b;
	double     sum; 
	int    veclen; 
} DOTDATA;

#define VECLEN 100000
#define NUM_THREADS 2

DOTDATA dotstr; 

void *dotprod_sum(void * thread_arg)
{
	int id = (*(int *)thread_arg);
	int start = id*(VECLEN/NUM_THREADS);
	int end = start + VECLEN/NUM_THREADS;
	double * local_sum = (double *)malloc(sizeof(double));
	for (int i = start; i < end; i++) {
		*local_sum += dotstr.a[i] * dotstr.b[i];
	}
	//printf("Thread sum = %lf\n", *local_sum);
	pthread_exit((void *)local_sum);
}

/*
   We will use a function (dotprod) to perform the scalar product. All input to 
   this routine is obtained through a structure of type DOTDATA and all output 
   from this function is written into this same structure.  While this is 
   unnecessarily restrictive for a sequential program, it will turn out to be 
   useful when we modify the program to compute in parallel.
   */
void* dotprod(void)
{

	/* Define and use local variables for convenience */

	int start, end, i; 
	double mysum, *x, *y;

	start=0;
	end = dotstr.veclen;
	x = dotstr.a;
	y = dotstr.b;

	/*
	   Perform the dot product and assign result to the appropriate variable in 
	   the structure. 
	   */

        pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * NUM_THREADS);
        size_t *slice_count = (size_t *)malloc(sizeof(size_t) * NUM_THREADS);
	dotstr.sum = 0;
	for (int i=0; i<NUM_THREADS ; i++) 
	{
	    slice_count[i] = i;
            pthread_create(&tid[i], NULL, dotprod_sum, &slice_count[i]);
        }

	void *thread_sum;
	for (i=0; i< NUM_THREADS; i++) {
		pthread_join(tid[i], &thread_sum);
		dotstr.sum += (*(double *)thread_sum);
	}
        return NULL;
}


/*
   The main program initializes data and calls the dotprd() function.  Finally, 
   it prints the result.
   */

int main (int argc, char* argv[])
{
	struct timeval start_time, end_time;
	struct timeval start_loop_time, end_loop_time;
	gettimeofday(&start_time, NULL);

	int i,len;
	double *a, *b;

	/* Assign storage and initialize values */
	len = VECLEN;
	a = (double*) malloc (len*sizeof(double));
	b = (double*) malloc (len*sizeof(double));

	for (i=0; i<len; i++) {
		a[i]=1;
		b[i]=a[i];
	}

	dotstr.veclen = len; 
	dotstr.a = a; 
	dotstr.b = b; 
	dotstr.sum=0;

	/* Perform the  dotproduct */

        gettimeofday(&start_loop_time, NULL);
        dotprod ();
        gettimeofday(&end_loop_time, NULL);

	/* Print result and release storage */ 
	//printf ("Done. PThread version: sum =  %f \n", dotstr.sum);
	free (a);
	free (b);

        gettimeofday(&end_time, NULL);
        long sec = end_time.tv_sec - start_time.tv_sec;
        long usec = end_time.tv_usec - start_time.tv_usec;
        printf("%ld,", sec*1000000+usec);

        sec = end_loop_time.tv_sec - start_loop_time.tv_sec;
        usec = end_loop_time.tv_usec - start_loop_time.tv_usec;
        printf("%ld\n", sec*1000000+usec);
}
