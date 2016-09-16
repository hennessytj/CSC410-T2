/***********************************************************************
 * t2_v3.c written by DSU_410 team ...
 *
 * What's new:
 * -Added parallel implementation using static scheduling
 * -If a value is negative, it is replaced by zero before
 *  being transfered into new array cell.
 * -Fixed contants, variables, and various terminology
 *
 * Description: Given an MxN matrix compute the
 * sums of each cell and its neighbors.  Use the sum
 * and the set of rules to derive the each cell's
 * new value.
 *
 * compile: %gcc -o t2_v3 t2_v3.c
 * execute: ./t2_v3
 *
 * Process:
 * 1.) Compute a cell sum (based on its value and its
 *     neighbors value).
 * 2.) Use the sum with the conditions and rules to
 *     determine the new value.
 *
 * 1.) Determining a cell's value:
 * To determine the sum of cell X, add each neighbor
 * and the value of X.  E.g., 1+2+3+5+X+7+9+10+11
 *    1   2   3   4
 *    5   X   7   8
 *    9  10  11  12
 *   13  14  15  16
 *
 * 2.) Receive new value
 * Conditions    Rules
 * -------------------------------------------
 * % 10 == 0		0
 * Under 50		Add 3
 * Over  50		Subtract 3	Can't go negative
 * Over 150		1
 *
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "arrays.h"

// Global extern basis variables
// Used to make working with threads easier since each thread shares
// data which includes global variables
int A[M][N] = {0};
int B[M][N];

// Threads
#define NUM_THREADS 5

// Important generation data
#define TOTAL_GENERATIONS 1
int CURRENT_GENERATION = 0;

/*****************************  newValue  *****************************
 * int newValue(int sum, int cellValue)
 *
 * Description: Takes an integer and uses rules by Tom to determine
 * new value.
 *
 * Process:
 * 1.) Assign to value appropriate new value based on rules.
 * 3.) Value is returned.
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * sum          in          integer used by rules
 * cellValue    in          current cell value used to determine new
 *                          value
 *
 * NOTES:
 * -If the new value is going to be negative as a result of
 *  applying the rule, assign 0 instead of new negative value
 *
 * Conditions    Rules
 * -------------------------------------------
 * % 10 == 0		0
 * Under 50		Add 3
 * Over  50		Subtract 3	Can't go negative, assign 0
 * Over 150		1
 ***********************************************************************/
int newValue(int sum, int cellValue)
{
    int value = -9999;
    if (sum % 10 == 0)              value = 0;
    else if (sum < 50)              value = cellValue + 3;
    else if (sum > 50 && sum < 150) value = ((cellValue - 3) < 0) ? 0 : (cellValue - 3);
    else                            value = 1;
    
    return value;
}

/*****************************  computeSum  *****************************
 * int computeSum(int cols, int i, int j, int arr[][cols])
 *
 * Description: Takes a 2D array and computes sum of arr[i][j] and
 * its neighbors.  Returns this value to the calling environment.
 *
 * Process:
 *     Assumption: no element arr[i][j] is passed to this function
 *     that will be out of bounds or not capable of being computed.
 * 1.) The values of neighbor cells are hardcoded and added together
 *     with arr[i][j].
 * 2.) Sum is returned.
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * cols         in          total number of columns in array, used for
 *                          function declaration
 * i            in          row of computed element
 * j            in          column of computed element
 * arr          in          used to compute sum
 *
 * NOTES:
 * - C99 and later accept function declaration as long as column values
 *   for 2D array appear in parameter list before arrays.
 * - Please see assumption under Process above.
 *
 * Neighbor diagram (assume X is arr[i][j]):
 *    Start here:  1 ->  2  -> 3
 *                             |
 *                             V
 *                 8 ->  X     4
 *                 ^           |
 *                 |           V
 *                 7 <-  6  <- 5
 * Neighbor order goes 1, 2, 3, 4, 5, 6, 7, 8, X
 ***********************************************************************/
int computeSum(int cols, int i, int j, int arr[][cols])
{
    int sum = 0;
    sum = arr[i - 1][j - 1] +   // 1
    arr[i - 1][  j  ] +         // 2
    arr[i - 1][j + 1] +         // 3
    arr[  i  ][j + 1] +         // 4
    arr[i + 1][j + 1] +         // 5
    arr[i + 1][  j  ] +         // 6
    arr[i + 1][j - 1] +         // 7
    arr[  i  ][j - 1] +         // 8
    arr[  i  ][  j  ];          // X
    return sum;
}

/*****************************  updateCells  *****************************
 * void updateCells(int rows, int cols, int arr1[][cols], int arr2[][cols])
 *
 * Description: Takes a 2D array and computes a new positive integer
 * value for each applicable cell based on a set of rules.  The new
 * value is stored in another array.
 *
 * Process:
 * 1.) For each cell in arr1, call computeSum.
 * 2.) Sum is used with a set of conditions and rules to determine
 *     the new value of the cell in arr2.  A separate function is called
 *     for this part, newValue(sum).
 * 3.) The new value is stored into arr2.
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * start        in          first row worked on
 * end          in          last row worked on
 *
 * NOTES:
 * - C99 and later accept function declaration as long as column values
 *   for 2D array appear in parameter list before arrays.
 * - The outer perimeter is not in play.  The outer perimeter of both
 *   arrays are 0's used to help programmer.
 ***********************************************************************/
void updateCells(int start, int end)
{
    int i;
    int j;
    int sum = 0;
    int arr1[M][N];
    int arr2[M][N];
    
    /* Figure out how to assign B's pointer to arr1 and so on...
    if (CURRENT_GENERATION % 2 == 0) // B is arr1, A is arr2
    else // A is arr1, B is arr2
     */
    
    for (i = start; i <= end; i++)
    {
        for (j = 1; j < N - 1; j++)
        {
            // get sum of neighbors and current cell
            sum = computeSum(M, i, j, arr1);
            // store newValue based on rules into arr2
            arr2[i][j] = newValue(sum, arr1[i][j]);
        }
    }
}

/*****************************  entryPoint  ********************************
 * void * entryPoint(void *param)
 *
 * Description: The entry point for each pthread in program. Uses static
 * work partition algorithm to assign jobs to each thread based on their
 * tid. Jobs are the number of rows a thread will work on.
 *
 * Process:
 * 1.) Determine how many rows each thread is responsible for.
 * 2.) Call updateCells
 * 3.) Thread exits
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * param        in          void pointer cast to be an int representing
 *                          each thread id.
 *
 * NOTES:
 * - Built to be flexible for changing values of M (number of rows) and
 *   NUM_THREADS, defined constants.
 **************************************************************************/
void *entryPoint(void *param)
{
    int i;
    int tid = (int) param;
    int rows = M - 2;   // first and last row don't count
    int numRows = rows / NUM_THREADS;
    int remainingRows = rows % NUM_THREADS;
    int startRow = numRows * tid;
    int endRow;
    // last thread is one less than NUM_THREADS
    // last thread is assigned remaining number of rows
    if (tid == NUM_THREADS - 1)
    {
        endRow = numRows * tid + numRows + remainingRows;
    }
    else
    {
        endRow = numRows * tid + numRows;
    }
    
    for (i = startRow; i < endRow; i++)
        printf("tid %d does row %d\n", tid, i + 1);
    
    // updateCells(startRow + 1, endRow + 1);
    
    pthread_exit(NULL);
}

/*****************************  spinUpThreads  *****************************
 * void spinUpThreads()
 *
 * Description: Sets up threads to work on 2D array.
 *
 * Process:
 * 1.) Create NUM_THREADS (defined constant) and start them off at entryPoint.
 *     entryPoint is a function which will provide the thread with tasks.
 * 2.) Join threads upon their exit.
 * 3.) Return to main function.
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * None
 *
 * NOTES:
 * - N/A
 ***********************************************************************/
void spinUpThreads()
{
    pthread_t tid[NUM_THREADS];
    long t;
    void *status;
    
    for (t = 0; t < NUM_THREADS; t++)
        pthread_create(&tid[t], NULL, entryPoint, (void *) t);
    
    for (t = 0; t < NUM_THREADS; t++)
        pthread_join(tid[t], &status);
}

int main(int argc, const char * argv[])
{
    // Will be generation 0, generation 0 does not count towards total generations
    // computed by this program
     fillRandomly(M, N, B);
     printf("Gen:  %d ---------------------------  \n", CURRENT_GENERATION);
     print(M, N, B);
    
    /* Tom's array, used for comparison test
    int B[][N] = {
        {0,  0,  0,   0,  0,  0,  0, 0},
        {0,  3,  6,  17, 15, 13, 15, 0},
        {0,  6, 12,   9,  1,  2,  7, 0},
        {0, 10, 19,   3,  6,  0,  6, 0},
        {0, 12, 16,  11,  8,  7,  9, 0},
        {0,  2, 10,   2,  3,  7, 15, 0},
        {0,  9,  2,   2, 18,  9,  7, 0},
        {0,  0,  0,   0,  0,  0,  0, 0}
    };
    printf("Gen:  %d ---------------------------  \n", CURRENT_GENERATION);
    print(M, N, B);
     */
    
    // Will loop for the value of totalGenerations
    // generation is used to determine the order of
    // switching arrays used in intermediate calculations
    while (CURRENT_GENERATION < TOTAL_GENERATIONS)
    {
        if (CURRENT_GENERATION % 2 == 0)
        {
            // transfer new values based on rules into array A
            // array B is used for initial values, array A gets new rule based values
            spinUpThreads();
            printf("Gen:  %d ---------------------------  \n", CURRENT_GENERATION + 1);
            print(M, N, A);
            //printf("Sum is %d\n", computeSum(n, 2, 1, B));
        }
        else
        {
            // transfer new values based on rules into array B
            // array A is used for prev values, array B gets new rule based values
            spinUpThreads();
            printf("Gen:  %d ---------------------------  \n", CURRENT_GENERATION + 1);
            print(M, N, B);
        }
        CURRENT_GENERATION++;
    }
    
    return 0;
}
