#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, free, rand(), srand()
#include <time.h>    // time for random generator
#include <math.h>    // log2
#include <string.h>  // memcpy
#include <limits.h>  // INT_MAX
#include <pthread.h>

typedef struct sub_array{
    int * data;
    int size;
    int height;
} sub_array;

/* Declaration of functions */
void powerOfTwo(int numThreads);
void getInput(int argc, char* argv[], int *numThreads, int* arraySize);
void fillArray(int array[], int arraySize);
void printList(char arrayName[], int array[], int arraySize);
int compare(const void* a_p, const void* b_p);
int* merge(int half1[], int half2[], int mergeResult[], int size);
void* mergeSort(void * );


int main(int argc, char** argv) {
    int numThreads, globalArraySize;
    int *globalArray;
    time_t startTime, localTime, totalTime;
    time_t zeroStartTime, zeroTotalTime, processStartTime, processTotalTime;
    void * ret;
    pthread_t *tid;

    sub_array global;

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
    global.height = height;
    global.size = globalArraySize;

    printList("unsorted", globalArray, globalArraySize);

    pthread_create(&tid[0], NULL, mergeSort, &global);
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
 * Function:    merge
 * Purpose:     Merges half1 array and half2 array into mergeResult
 * Params:      half1, first half of array to merge
 *   			half2, second half of array to merge
 * 				mergeResult, array to store merged result
 * 				size, size of half1 and half2
 */
int* merge(int half1[], int half2[], int mergeResult[], int size){
    int ai, bi, ci;
    ai = bi = ci = 0;

    // integers remain in both arrays to compare
    while ((ai < size) && (bi < size)){
        if (half1[ai] <= half2[bi]){
            mergeResult[ci] = half1[ai];
            ai++;
        } else {
            mergeResult[ci] = half2[bi];
            bi++;
        }
        ci++;
    }
    // integers only remain in rightArray
    if (ai >= size){
        while (bi < size) {
            mergeResult[ci] = half2[bi];
            bi++; ci++;
        }
    }
    // integers only remain in localArray
    if (bi >= size){
        while (ai < size) {
            mergeResult[ci] = half1[ai];
            ai++; ci++;
        }
    }
    return mergeResult;
}

/*-------------------------------------------------------------------
 * Function:    mergeSort
 * Purpose:     implements merge sort: merges sorted arrays from 
 *              processes until we have a single array containing all 
 *  		integers in sorted order
 * Params:	height, height of merge sort tree
 * 		id, rank of the current process
 * 		localArray, local array containing integers of current process
 * 		size, size of localArray on current process
 * 		comm, MPI communicator
 * 		globalArray, globalArray contains either all integers
 *    			if process 0 or NULL for other processes
 */

void* mergeSort(void * array){
    int parent, rightChild, myHeight, localArraySize;
    int *half1, *half2, *mergeResult;
    void * lret, *rret;
    pthread_t tid[2];
    sub_array l, r;
    struct sub_array* subset = (struct sub_array * ) array;

    if(subset->height == 0){
        qsort(subset->data, subset->size, sizeof(int),compare);
        pthread_exit((void*)subset->data);
    }else{
        localArraySize = subset->size/2;
        //localArray = (int*) malloc (localArraySize * sizeof(int)); 
        l.size = localArraySize;
        l.data = subset->data;
        l.height = subset->height - 1;

        r.size = localArraySize;
        r.data = &(subset->data[localArraySize]);
        r.height = subset->height - 1;

        pthread_create(&tid[0], NULL, mergeSort, &l);
        pthread_create(&tid[1], NULL, mergeSort, &r);

        pthread_join(tid[0], &lret);
        pthread_join(tid[1], &rret); 

        mergeResult = (int *) malloc(sizeof(int) * subset->size);

        merge(lret, rret, mergeResult, r.size);

        //free(lret);
        //free(rret);

        pthread_exit((void*)mergeResult);
    }
}

