//Copyright (c) 2019 Jason Graalum
// 
// Various Summation routines usig pthreads //
// CS 531 Witer 2019 // Jason Graalum
// // January 17, 2019
//

#include "summation.h"


void *thread_loop_local(void *);

void multi_thread_local_var(size_t iterations, size_t number, size_t nthreads, size_t cache_line_size, char verbosity)
{
    double total_clock_time;
    double total_proc_time;


    size_t nelems_per_thread = number/nthreads;

    // Output data variables
    float min, max, mean, median, variance, std_dev;
    double *time_data, *process_data; 
    time_data = (double *)malloc(sizeof(double)*iterations);
    process_data = (double *)malloc(sizeof(double)*iterations);

    if(verbosity == '1')
        printf("iterations = %zu\nnumber = %ld\tthreads = %zu\tcache_line_size = %zu\n", iterations, number, nthreads, cache_line_size);


    // Init time vars
    total_clock_time = 0;
    total_proc_time = 0; 
    data_t global_sum;

    thread_data_t *thread_data;
 
    for (int it = 0; it < iterations; it++) {
        //printf("Starting iteration #%d\n",it);

        pthread_t *tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads * cache_line_size);

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

        thread_data = (thread_data_t *)malloc(sizeof(thread_data_t) * nthreads * cache_line_size);
            
        for(size_t t = 0; t < nthreads; t++)
        {
            //printf("Starting thread %zu\n", t);
            thread_data[t*cache_line_size].start_num = (data_t)t*nelems_per_thread;  
            thread_data[t*cache_line_size].end_num = (data_t)((t+1)*nelems_per_thread);  
            tid[t*cache_line_size] = t;
            pthread_create(&tid[t*cache_line_size], NULL, thread_loop_local, &thread_data[t*cache_line_size]);
        }

        void *thread_results;
        for (size_t t = 0; t < nthreads; t++)
        {
            pthread_join(tid[t*cache_line_size], &thread_results);
            global_sum += ((thread_data_t*)thread_results)->sum;
            //printf("Thread returned with local sum = %llu\n", ((thread_data_t*)thread_results)->sum);
        }

        // Capture End Clocks
        clock_gettime(CLOCK_MONOTONIC, &ts_mono_end);
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts_process_end);

        // Store timing data
        time_data[it] = (double)(10e9*(ts_mono_end.tv_sec-ts_mono_start.tv_sec)+(ts_mono_end.tv_nsec-ts_mono_start.tv_nsec));
        process_data[it] = (double)(10e9*(ts_process_end.tv_sec-ts_process_start.tv_sec)+(ts_process_end.tv_nsec-ts_process_start.tv_nsec));

        if (verbosity == '1')
        {
            printf("#%d,%lf,%lf\n", it, time_data[it], process_data[it]);
        }

        total_clock_time += time_data[it];
        total_proc_time += process_data[it];
    }


    if(verbosity == '3')
    {
        printf("%llu, %zu, %lf, %lf, %lf, %lf, %lf, %lf\n", global_sum, iterations,
                gsl_stats_mean(time_data,1,iterations),
                gsl_stats_max(time_data,1,iterations), 
                gsl_stats_min(time_data,1,iterations), 
                gsl_stats_median(time_data,1,iterations), 
                gsl_stats_variance(time_data,1,iterations), 
                gsl_stats_sd(time_data,1,iterations));
    }
    else if(verbosity == '2')
    {
        printf("Sum = %llu\n", global_sum);
        printf("Samples = %zu\n", iterations);
        printf("Mean Clock = %lf\n", gsl_stats_mean(time_data,1,iterations));
        printf("Max Clock = %lf\n", gsl_stats_max(time_data,1,iterations));
        printf("Min Clock = %lf\n", gsl_stats_min(time_data,1,iterations));
        printf("Median Clock = %lf\n", gsl_stats_median(time_data,1,iterations));
        printf("Variance Clock = %lf\n", gsl_stats_variance(time_data,1,iterations));
        printf("StdDev Clock = %lf\n", gsl_stats_sd(time_data,1,iterations));
    }
}

void *thread_loop_local(void *thread_data)
{
    size_t i;
    ((thread_data_t*)thread_data)->sum = 0;
    data_t start_num = ((thread_data_t*)thread_data)->start_num;
    data_t end_num = ((thread_data_t*)thread_data)->end_num;
    //printf("Start Thread - start_num = %llu\tend_num = %llu\n", start_num, end_num);
    for (i = start_num; i < end_num; i++) {
        ((thread_data_t*)thread_data)->sum += i;
    }
    //printf("End Thread - start_num = %llu\tend_num = %llu\n", start_num, end_num);
    
    pthread_exit(thread_data);
}

