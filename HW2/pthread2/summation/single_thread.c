//Copyright (c) 2019 Jason Graalum
// 
// Various Summation routines usig pthreads //
// CS 531 Witer 2019 // Jason Graalum
// // January 17, 2019
//

#include "summation.h"

data_t single_thread_loop(size_t);

void single_thread(size_t iterations, size_t number, char verbosity)
{
    double total_clock_time;
    double total_proc_time;
    data_t global_sum = 0;

    // Output data variables
    float min, max, mean, median, variance, std_dev;
    double *time_data, *process_data; 
    time_data = (double *)malloc(sizeof(double)*iterations);
    process_data = (double *)malloc(sizeof(double)*iterations);

    if(verbosity == '1')
        printf("iterations = %zu\nnumber = %ld\n", iterations, number);

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

        global_sum = single_thread_loop(number);
        /*
        for(size_t i = 0; i < number; i++)
            global_sum += i;
        */

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

data_t single_thread_loop(size_t number)
{
    data_t sum;
    for (size_t i = 0; i < number; i++) {
        sum += i;
    }
    return sum;
}

