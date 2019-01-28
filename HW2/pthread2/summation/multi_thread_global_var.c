//Copyright (c) 2019 Jason Graalum
// 
// Various Summation routines usig pthreads //
// CS 531 Witer 2019 // Jason Graalum
// // January 17, 2019
//

#include "summation.h"


void *thread_loop(void *);

void multi_thread_global_var(size_t iterations, size_t number, size_t nthreads, size_t cache_line_size, char verbosity)
{
    int total_clock_time;
    double total_proc_time;

    // Output data variables
    float min, max, mean, median, variance, std_dev;
    double *time_data; 
    double *process_data; 
    time_data = (double *)malloc(sizeof(double)*iterations);
    process_data = (double *)malloc(sizeof(double)*iterations);

    if(verbosity == '3')
        printf("iterations = %zu\nnumber = %ld\tthreads = %zu\tcache_line_size = %zu\n", iterations, number, nthreads, cache_line_size);


    // Init time vars
    total_clock_time = 0;
    total_proc_time = 0;


    for (int it = 0; it < iterations; it++) {
        //printf("Starting iteration #%d\n", it);
        pthread_mutex_lock(&(global_sum.lock));
        global_sum.val = 0;
        pthread_mutex_unlock(&(global_sum.lock));

        pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads * cache_line_size);
        size_t *slice_count = (size_t *)malloc(sizeof(size_t) * nthreads * cache_line_size);

        //
        // Setup struct for time keeping
        // Instead of using timeofday, I decided to use the CPU and PROCESS timers
        //
        //struct timespec ts_mono_start;
        //struct timespec ts_mono_end;
        struct timespec ts_process_start;
        struct timespec ts_process_end;
        struct timeval t_time_start;
        struct timeval t_time_end;

        // Capture Start Clocks
        //clock_gettime(CLOCK_MONOTONIC, &ts_mono_start);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_start);
        gettimeofday(&t_time_start, NULL);

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
            //printf("Thread %zu ended with global sum = %llu\n", t, global_sum);
        }

        // Capture End Clocks
        //clock_gettime(CLOCK_MONOTONIC, &ts_mono_end);
        gettimeofday(&t_time_end, NULL);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_end);

        // Store timing data
        //time_data[it] = (double)(10e9*(ts_mono_end.tv_sec-ts_mono_start.tv_sec)+(ts_mono_end.tv_nsec-ts_mono_start.tv_nsec));
        time_data[it] = (double)(t_time_end.tv_sec - t_time_start.tv_sec);
        process_data[it] = (double)(10e9*(ts_process_end.tv_sec-ts_process_start.tv_sec)+(ts_process_end.tv_nsec-ts_process_start.tv_nsec));
        if (verbosity == '1')
        {
            printf("#%d,%lf,%lf,%zu,%zu\n", it, time_data[it], process_data[it], nthreads, cache_line_size);
       //     printf("#%d,%lf,%zu,%zu\n", it, process_data[it], nthreads, cache_line_size);
        }

        total_clock_time += time_data[it];
        total_proc_time += process_data[it];
    }

    pthread_mutex_destroy(&(global_sum.lock));

    if(verbosity == '3')
    {
        printf("%llu, %zu, %lf, %lf, %lf, %lf, %lf, %lf\n", global_sum.val, iterations,
                gsl_stats_mean(time_data,1,iterations),
                gsl_stats_max(time_data,1,iterations),
                gsl_stats_min(time_data,1,iterations),
                gsl_stats_median(time_data,1,iterations),
                gsl_stats_variance(time_data,1,iterations),
                gsl_stats_sd(time_data,1,iterations));
    }
    else if (verbosity == '2')
    {
        printf("Sum = %zu\n", iterations);
        printf("Samples = %zu\n", iterations);
        printf("Mean Clock = %lf\n", gsl_stats_mean(time_data,1,iterations));
        printf("Max Clock = %lf\n", gsl_stats_max(time_data,1,iterations));
        printf("Min Clock = %lf\n", gsl_stats_min(time_data,1,iterations));
        printf("Median Clock = %lf\n", gsl_stats_median(time_data,1,iterations));
        printf("Variance Clock = %lf\n", gsl_stats_variance(time_data,1,iterations));
        printf("StdDev Clock = %lf\n", gsl_stats_sd(time_data,1,iterations));
    }
}

void *thread_loop(void *vargp)
{
    int myid = *((int *)vargp);
    pthread_mutex_lock(&(nelems_per_thread.lock));
    size_t start = myid * nelems_per_thread.val;
    size_t end = start + nelems_per_thread.val;
    pthread_mutex_unlock(&(nelems_per_thread.lock));
    size_t i;
    // Critical Section
    pthread_mutex_lock(&(global_sum.lock));
    //printf("Locked(%d)\n",myid);

    for (i = start; i < end; i++) {
        global_sum.val += i;
    }
    //printf("Unlocked(%d)\n",myid);
    pthread_mutex_unlock(&(global_sum.lock));

    return NULL;
}

