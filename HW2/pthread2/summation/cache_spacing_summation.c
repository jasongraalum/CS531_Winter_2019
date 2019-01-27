//Copyright (c) 2019 Jason Graalum
// 
// Various Summation routines usig pthreads //
// CS 531 Witer 2019 // Jason Graalum
// // January 17, 2019
//
// HW 2 - Part A.
//
// summation.c
//
// A configurable tool to measure the pthread efficency
//
// The routine sums number from 1 to n where n is a long long int.
//
// There are four modes
// 1. sp_sum() : Single process summation
// #define SP_SUM
//
// 2. pth_1_gvar_cont_sum : Multithreaded with single global variable containing the sum using locks
//    where the global sum is continually updated by the summation thread
// #define PTH_1_GVR_CONT
//
// 3. pth_1_gvar_end_sum : Multithreaded with single global variable containing the sum using locks
//    where the global sum is updated at the end of the summation thread
// #define PTH_1_GVR_END
//
// 4. pth_n_gvar_sum : Multithreaded with an array of global variables containing the sums of each thread
//    and the main process aggregates the sum of each thread.
// #define PTH_N_GVR
//
//
// An addition option that can be adjusted at compile time is the spacing of the input number and the
// array of thread sums. The spacing is equal to either 1 or the Data Cache Line Length
//
//

#include "summation.h"

int main ( int argc, char **argv ) {

    // Usage details
    if(argc < 4) { 
        printf("Usage: %s iterations number num_threads\n",argv[0]); 
        exit(-1);
    }

    int iterations = (int) atoi(argv[1]);
    size_t number = (size_t) atoi(argv[2]);
    size_t nthreads = (size_t) atoi(argv[3]);

    // Case Setup
    double total_clock_time;
    double total_proc_time;
    data_t global_sum = 0;

    struct pthread_arg_t {
        int id;
        size_t start_num;
        size_t end_num;
    };
    struct pthread_arg_t *thread_args;
    thread_args = (struct pthread_arg_t *)malloc(sizeof(struct pthread_arg_t));
        

#ifdef LEVEL1_DCACHE_LINESIZE
    int var_cache_spacing = LEVEL1_DCACHE_LINESIZE/sizeof(int);
#else
    int var_cache_spacing = 1; 
#endif

#ifdef SP_SUM
#endif

#ifdef PTH_1_GVAR_CONT
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    data_t nelems_per_thread = (data_t)number/nthreads;
    int *thread_id = (int *)malloc(sizeof(int) * nthreads * var_cache_spacing) ;
#endif

#ifdef PTH_1_GVAR_END
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    data_t nelems_per_thread = (data_t)number/nthreads;
    int *thread_id = (int *)malloc(sizeof(int) * nthreads * var_cache_spacing) ;
#endif

#ifdef PTH_NTH_GVA
    pthread_t *tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    data_t nelems_per_thread = (data_t)number/nthreads;
    int *thread_id = (int *)malloc(sizeof(int) * nthreads * var_cache_spacing) ;
    data_t *local_sum = (data_t *)malloc(sizeof(data_t) * nthreads * var_cache_spacing); 
#endif


    // Output data variables
    float min, max, mean, median, variance, std_dev;
    double *time_data, *process_data; 
    time_data = (double *)malloc(sizeof(double)*iterations);
    process_data = (double *)malloc(sizeof(double)*iterations);

    //printf("iterations = %d\nnumber = %ld\tthreads = %ld\telems_per_thread = %ld\n", iterations, number, nthreads, nelems_per_thread);

    total_clock_time = 0;
    total_proc_time = 0;
    for (int it = 0; it < iterations; it++) {
        global_sum = 0;

        //
        // Setup struct for time keeping
        // Instead of using timeofday, I decided to use the CPU and PROCESS timers
        //
        struct timespec ts_mono_start;
        struct timespec ts_mono_end;
        struct timespec ts_process_start;
        struct timespec ts_process_end;
        // Capture Start Clocks
        clock_gettime(CLOCK_MONOTONIC, &ts_mono_start);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_start);

#ifdef SP_SUM
        for(size_t i = 0; i < number; i++)
            global_sum += i;

#else

        /* Create threads and wait for them to finish */
        for (int t = 0; t < nthreads; t++) 
        {
            thread_args->id = t;
            thread_args->start_num = t * nelems_per_thread;
            thread_args->end_num = (t + 1) * nelems_per_thread;

            thread_id[t*var_cache_spacing] = t;
            
#ifdef PTH_1_GVAR_CONT
            pthread_create(&tid[t], NULL, pth_1_gvar_cont_sum, &thread_id[t*var_cache_spacing]);
#elif PTH_1_GVAR_END
            pthread_create(&tid[t], NULL, pth_1_gvar_end_sum, &thread_id[t*var_cache_spacing]);
#elif PTH_NTH_GVAR
            pthread_create(&tid[t], NULL, pth_nth_gvar_sum, thread_args);
#endif
        }

        for (int t = 0; t < nthreads; t++)
        {
            pthread_join(tid[t], NULL);
            global_sum += local_sum[i*var_cache_spacing]; 
            printf("Thread %d joined with %lld\n",i,sum_local[i*var_cache_spacing]);
        }
#endif

        // Capture End Clocks
        clock_gettime(CLOCK_MONOTONIC, &ts_mono_end);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_end);

        // Store timing data
        time_data[it] = (double)(10e9*(ts_mono_end.tv_sec-ts_mono_start.tv_sec)+(ts_mono_end.tv_nsec-ts_mono_start.tv_nsec));
        process_data[it] = (double)(10e9*(ts_process_end.tv_sec-ts_process_start.tv_sec)+(ts_process_end.tv_nsec-ts_process_start.tv_nsec));
        printf("#%d\tl%lf\t%lf\n", it, time_data[it], process_data[it]);
        total_clock_time += time_data[it];
        total_proc_time += process_data[it];
    }

    printf("Samples = %d\n", iterations);
    printf("Mean Clock = %lf\n", gsl_stats_mean(time_data,1,iterations));
    printf("Max Clock = %lf\n", gsl_stats_max(time_data,1,iterations));
    printf("Min Clock = %lf\n", gsl_stats_min(time_data,1,iterations));
    printf("Median Clock = %lf\n", gsl_stats_median(time_data,1,iterations));
    printf("Variance Clock = %lf\n", gsl_stats_variance(time_data,1,iterations));
    printf("StdDev Clock = %lf\n", gsl_stats_sd(time_data,1,iterations));
}
