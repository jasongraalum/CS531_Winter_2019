//Copyright (c) 2019 Jason Graalum
// 
// Various Summation routines usig pthreads //
// CS 531 Witer 2019 // Jason Graalum
// // January 17, 2019
//

#include "summation.h"


void *thread_loop(void *);

void multi_thread_global_var(size_t iterations, size_t number, size_t nthreads, size_t cache_line_size, char output_opt)
{
    double total_clock_time;
    double total_proc_time;
    global_sum = 0;

    // Output data variables
    float min, max, mean, median, variance, std_dev;
    double *time_data, *process_data; 
    time_data = (double *)malloc(sizeof(double)*iterations);
    process_data = (double *)malloc(sizeof(double)*iterations);

    //printf("iterations = %d\nnumber = %ld\tthreads = %ld\telems_per_thread = %ld\n", iterations, number, nthreads, nelems_per_thread);


    // Init time vars
    total_clock_time = 0;
    total_proc_time = 0;

 
    for (int it = 0; it < iterations; it++) {
        global_sum = 0;

        pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads * cache_line_size);
        size_t *slice_count = (size_t *)malloc(sizeof(size_t) * nthreads * cache_line_size);

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

        for(size_t t = 0; t < nthreads; t++)
        {
            //printf("Starting thread %zu\n", t);
            slice_count[t*cache_line_size] = t;
            tid[t*cache_line_size] = t;
            pthread_create(&tid[t*cache_line_size], NULL, thread_loop, &slice_count[t*cache_line_size]);
        }

        for (size_t t = 0; t < nthreads; t++)
        {
            pthread_join(tid[t*cache_line_size], NULL);
            printf("Thread %zu ended with global sum = %llu\n", t, global_sum);
        }

        // Capture End Clocks
        clock_gettime(CLOCK_MONOTONIC, &ts_mono_end);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_end);

        // Store timing data
        time_data[it] = (double)(10e9*(ts_mono_end.tv_sec-ts_mono_start.tv_sec)+(ts_mono_end.tv_nsec-ts_mono_start.tv_nsec));
        process_data[it] = (double)(10e9*(ts_process_end.tv_sec-ts_process_start.tv_sec)+(ts_process_end.tv_nsec-ts_process_start.tv_nsec));
        if (output_opt == '1')
        {
            printf("#%d\tl%lf\t%lf\n", it, time_data[it], process_data[it]);
        }

        total_clock_time += time_data[it];
        total_proc_time += process_data[it];
    }

    pthread_mutex_destroy(&global_sum_lock);

    printf("Samples = %zu\n", iterations);
    printf("Mean Clock = %lf\n", gsl_stats_mean(time_data,1,iterations));
    printf("Max Clock = %lf\n", gsl_stats_max(time_data,1,iterations));
    printf("Min Clock = %lf\n", gsl_stats_min(time_data,1,iterations));
    printf("Median Clock = %lf\n", gsl_stats_median(time_data,1,iterations));
    printf("Variance Clock = %lf\n", gsl_stats_variance(time_data,1,iterations));
    printf("StdDev Clock = %lf\n", gsl_stats_sd(time_data,1,iterations));
}

void *thread_loop(void *vargp)
{
    int myid = *((int *)vargp);
    size_t start = myid * nelems_per_thread;
    size_t end = start + nelems_per_thread;
    size_t i;
    data_t local_sum = 0;
    // Critical Section
        //printf("Locked(%d)\n",myid);
    for (i = start; i < end; i++) {
        local_sum += i;
    }
        pthread_mutex_lock(&global_sum_lock);
        global_sum += local_sum;
        //printf("Unlocked(%d)\n",myid);
        pthread_mutex_unlock(&global_sum_lock);

    return NULL;
}

