#ifndef arrays_h
#define arrays_h

// M = rows, N = columns for 2D arrays
// The number of columns must be known at compile time
// Required for work division algorithm, so even if using a
// statically declared array for testing, make sure M and N are
// correct
#define M 8
#define N 8

// Used with rand() to determine range [0, RANGE)
#define RANGE 100
#define SEED  1

// Booleans
#define TRUE    1
#define FALSE   0

// Errors
#define GENERIC_ERROR_CODE      10
#define ERROR_DIMENSION_SIZE    11

void fillRandomly(int rows, int cols, int arr[][cols]);
void print(int rows, int cols, int arr[][cols]);

#endif /* arrays_h */
