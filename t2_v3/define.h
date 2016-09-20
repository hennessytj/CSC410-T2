#ifndef define_h
#define define_h

// Threads
#define NUM_THREADS 5

// Important generation data
#define TOTAL_GENERATIONS 4


// M = rows, N = columns for 2D arrays
// The number of columns must be known at compile time
// Required for work division algorithm, so even if using a
// statically declared array for testing, make sure M and N are
// correct
#define OFFSET 2
#define M 3 + OFFSET
#define N 3 + OFFSET

// Used with rand() to determine range [0, RANGE)
#define RANGE 20

void copyArray(int rows, int cols, int arr1[][cols], int arr2[][cols]);
void fillRandomly(int rows, int cols, int arr[][cols]);
void print(int rows, int cols, int arr[][cols]);

#endif /* define_h */
