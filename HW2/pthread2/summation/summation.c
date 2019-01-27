//Copyright (c) 2019 Jason Graalum
// // Various Summation routines usig pthreads //
// CS 531 Witer 2019 // Jason Graalum
// // January 17, 2019
//
// HW 2 - Part A.
//
// summation.c
//
// A configurable tool to measure the pthread efficency
//
// The routine sums number from 1 to n where n is a long long int.  //
// There are four modes
// 1. sp_sum() : Single process summation
//
// 2. pth_1_gvar_cont_sum : Multithreaded with single global variable containing the sum using locks
//    where the global sum is continually updated by the summation thread
//
// 3. pth_1_gvar_end_sum : Multithreaded with single global variable containing the sum using locks
//    where the global sum is updated at the end of the summation thread
//
// 4. pth_n_gvar_sum : Multithreaded with an array of global variables containing the sums of each thread
//    and the main process aggregates the sum of each thread.
//
// An addition option that can be adjusted at compile time is the spacing of the input number and the
// array of thread sums. The spacing is equal to either 1 or the Data Cache Line Length
//
//

#include "summation.h"


int main ( int argc, char **argv ) {

    // Usage details
    if(argc < 4) { 
        printf("Usage: %s mode iterations number num_threads <cache line size> <verbosity>\n",argv[0]); 
        printf("\tmode - s(single thread), g(multithreaded global variable), l(multithreaded local variable)\n");
        printf("\tcache line size - default=0\n");
        printf("\verbosity - 0 = final statistics only, 1 = all data points\n");

        exit(-1);
    }

    int arg_pos = 1;

    // Program Arguments
    char mode = (char) argv[arg_pos++][0];
    size_t iterations = (size_t) atoi(argv[arg_pos++]);
    size_t number = (size_t) atoi(argv[arg_pos++]);

    size_t cache_line_size = 1;
    char output_opt = '0';
    


    switch (mode) {
    case 's' : 
        // Single thread
        if(argc > arg_pos) output_opt  = (char) argv[arg_pos++][0];

        //printf("Running single threaded. %zu iterations. Summing to %zu. Verbosity is %c\n",iterations, number,output_opt); 
        single_thread(iterations, number, output_opt);
        break;
    case 'g' : 
        // Multithread with global var
        // Set global var 
        nthreads = (size_t) atoi(argv[arg_pos++]);
        nelems_per_thread.val = number/nthreads;
        if(argc > arg_pos) cache_line_size  = (size_t) atoi(argv[arg_pos++]);
        if(argc > arg_pos) output_opt  = (char) argv[arg_pos++][0];

        //printf("Running multi threaded with global var. %zu iterations. Summing to %zu with %zu threads. Cache Line Size is %zu. Verbosity is %c\n",iterations, number, nthreads, cache_line_size, output_opt); 
        multi_thread_global_var(iterations, number, nthreads, cache_line_size, output_opt);
        break;
    case 'l' : 
        // Multithread with a local variable and return value from thread
        // Set global var 
        nthreads = (size_t) atoi(argv[arg_pos++]);
        nelems_per_thread.val = number/nthreads;
        if(argc > arg_pos) cache_line_size  = (size_t) atoi(argv[arg_pos++]);
        if(argc > arg_pos) output_opt  = (char) argv[arg_pos++][0];

        //printf("Running multi threaded with local var. %zu iterations. Summing to %zu with %zu threads. Cache Line Size is %zu. Verbosity is %c\n",iterations, number, nthreads, cache_line_size, output_opt); 
        multi_thread_local_var(iterations, number, nthreads, cache_line_size, output_opt);
        break;
    default :
        printf("Unknown mode %c.\t Valid modes: s, g, l\n",mode);
        exit(-1);
    }

}

