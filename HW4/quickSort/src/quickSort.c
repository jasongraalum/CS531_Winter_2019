#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, free, rand(), srand()
#include <time.h>    // time for random generator
#include <math.h>    // log2
#include <string.h>  // memcpy
#include <limits.h>  // INT_MAX
#include <pthread.h>

typedef struct Array {
    int * data;
    int lo;
    int hi;
} Array ;

/* Declaration of functions */
void powerOfTwo(int numThreads);
void getInput(int argc, char* argv[], int *numThreads, int* arraySize);
void fillArray(int array[], int arraySize);
void printList(char arrayName[], int array[], int arraySize);
int partition(int array[], int lo, int hi);
void* quickSort(void * );


int main(int argc, char** argv) {
    int numThreads, globalArraySize;
    int *globalArray;
    time_t startTime, localTime, totalTime;
    time_t zeroStartTime, zeroTotalTime, processStartTime, processTotalTime;
    void * ret;
    pthread_t *tid;

    Array  global;

    // get size of global array
    getInput(argc, argv, &numThreads, &globalArraySize);

    // check for odd processes
    powerOfTwo(numThreads);

    // calculate total height of tree
    int height = log2(numThreads);
    tid = (pthread_t *)malloc(sizeof(pthread_t));

    // if process 0, allocate memory for global array and fill with values
    globalArray = (int*) malloc (globalArraySize * sizeof(int));
    fillArray(globalArray, globalArraySize);

    global.data = globalArray;
    global.lo = 0;
    global.hi = globalArraySize-1;

    printList("unsorted", globalArray, globalArraySize);

    pthread_create(&tid[0], NULL, quickSort, &global);
    pthread_join(tid[0], &ret);

    printList("sortedArray", (int*)ret, globalArraySize);

    free(ret);

    return 0;
}


/*-------------------------------------------------------------------
 * Function:   powerOfTwo
 * Purpose:    Check number of processes, if not power of 2 prints message
 * Params:     rank of the current process
 * 			   numThreads, number of processes
 */

void powerOfTwo( int numThreads) {
    int power;
    power = (numThreads != 0) && ((numThreads & (numThreads - 1)) == 0);
    if (!power) {
        printf("(%d)Number of processes must be power of 2.\n",numThreads);
        exit(-1);
    }
}

/*-------------------------------------------------------------------
 * Function:   getInput
 * Purpose:    Get input from user for array size
 * Params:     argc, argument count
 * 			   argv[], points to argument vector
 *     		   rank of the current process
 * 			   numThreads, number of processes
 * 			   arraySize, points to array size
 */

void getInput(int argc, char* argv[], int *numThreads, int* arraySize){
    if (argc != 3){
        fprintf(stderr, "usage: %s <number of threads> <size of array> \n", argv[0]);
        fflush(stderr);
        *arraySize = -1;
    } else if ((atoi(argv[2])) % (atoi(argv[1])) != 0) {
        fprintf(stderr, "size of array must be divisible by number of processes \n");
        fflush(stderr);
        *arraySize = -1;
    } else {
        *arraySize = atoi(argv[2]);
        *numThreads = atoi(argv[1]);
    }

    // negative arraySize ends the program
    if (*arraySize <= 0) {
        exit(-1);
    }
}
/*-------------------------------------------------------------------
 * Function:   fillArray
 * Purpose:    Fill array with random integers
 * Params:     array, the array being filled
 *   		   arraySize, size of the array
 *     		   rank of the current process
 */

void fillArray(int array[], int arraySize) {
    int i;
    // use current time as seed for random generator
    srand(time(0));
    for (i = 0; i < arraySize; i++) {
        array[i] = rand() % 100; //INT_MAX
    }
}

/*-------------------------------------------------------------------
 * Function:   printList
 * Purpose:    Prints the contents of a given list of a process
 * Params:     rank of the current process
 * 			   arrayName, name of array
 *     		   array, array to print
 *   		   arraySize, size of array
 */

void printList(char arrayName[], int array[], int arraySize) {
    printf("%s: ", arrayName);
    for (int i = 0; i < arraySize; i++) {
        printf(" %d", array[i]);
    }
    printf("\n");
}

/*-------------------------------------------------------------------
 * Function:    Compare - An Introduction to Parallel Programming by Pacheco
 * Purpose:     Compare 2 ints, return -1, 0, or 1, respectively, when
 *              the first int is less than, equal, or greater than
 *              the second.  Used by qsort.
 */

int compare(const void* a_p, const void* b_p) {
    int a = *((int*)a_p);
    int b = *((int*)b_p);

    if (a < b)
        return -1;
    else if (a == b)
        return 0;
    else /* a > b */
        return 1;
}
/*-------------------------------------------------------------------
 * Function:    partition
 * Purpose:     implements quick sort partition
 * Params:	lo, hi - indexes of array
 *              array - full array
 *    			if process 0 or NULL for other processes
 */
int partition(int * A, int lo, int hi)
{
    int pivot = A[(lo+hi)/2];
    int i = lo - 1;
    int j = hi + 1;
    printList("Pre-Paritition", A,hi-lo+1);
    printf("Partition with pivot %d from %d to %d\t", pivot, lo, hi);
    while(1) {
        do {
            i++;
        }while(A[i] < pivot);
        do {
            j--;
        } while(A[j] > pivot);
        if(i >= j) {
            printf(" Pivot = %d at %d\n",pivot, j);
            printList("Parititioned", A,hi-lo+1);
            return(j);
        }

        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}


/*-------------------------------------------------------------------
 * Function:    quickSort
 * Purpose:     implements quick sort: partitions array and calls quicksort
 *              on upper and lower partitions
 *  		integers in sorted order
 * Params:	lo, hi - indexes of array
 *              array - full array
 *    			if process 0 or NULL for other processes
 */

void* quickSort(void * A){
    pthread_t tid[2];
    Array lo_array, hi_array;
    void *lo_ret, *hi_ret;
    struct Array* array = (struct Array * ) A;
    int p;

    if(array->lo < array->hi) {
        p = partition(array->data, array->lo, array->hi);

        lo_array.data = array->data;
        lo_array.lo = array->lo;
        lo_array.hi = p;
        pthread_create(&tid[0], NULL, quickSort, &lo_array);
        pthread_join(tid[0], &lo_ret);

        hi_array.data = array->data;
        hi_array.lo = p+1;
        hi_array.hi = array->hi;
        pthread_create(&tid[1], NULL, quickSort, &hi_array);
        pthread_join(tid[1], &hi_ret);
    }
    pthread_exit((void*)array->data);
}

