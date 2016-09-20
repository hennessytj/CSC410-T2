#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "define.h"
/***********************************************************************
 * t2_v3.c written by DSU_410 team ...
 *
 * What's new:
 * -Uses two globally defined arrays A and B to compute values across
 *  generations.
 * -Array A is always printed out and contains the values of the most
 *  recent generation.  The array B is always used to compute the values.
 *
 * Description: Given an MxN matrix compute the
 * sums of each cell and its neighbors.  Use the sum
 * and the set of rules to derive the each cell's
 * new value.
 *
 * compile: %gcc main.c arrays.c -o t2_v3
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
 ************************************************************************/

// Global extern basis variables
// Used to make working with threads easier since each thread shares
// data which includes global variables
int A[M][N] = {0};
int B[M][N];
int CURRENT_GENERATION = 0;

/*****************************  newValue  *****************************
 * int newValue(int sum, int cellValue)
 *
 * Description: Takes an integer and uses rules by Tom to determine
 * new value.
 *
 * Process:
 * 1.) Assign to local variable value new value based on rules.
 * 3.) Return value
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
 * -The order of precedence of the rules is determined by their
 *  order of execution. The rule at the top has the highest
 *  precedence and the rule at the bottom has the lowest.
 * -Can't go negative, assign 0
 *
 * Conditions    Rules          Range
 * -------------------------------------------------------------------
 * % 10 == 0    Assign 0        Sums evenly divisible by 10
 * Under 50		Add 3           Sums less than or equal to 49 not 
 *                              divisible by 10
 * Over  50		Subtract 3      Sums between 51 and 150 not divisible 
 *                              by 10
 * Over 150		1               Sums 151 and greater not divisible by 10
 ***********************************************************************/
int newValue(int sum, int cellValue)
{
    int value = -9999;
    if (sum % 10 == 0)               value = 0;
    else if (sum < 50)               value = cellValue + 3;
    else if (sum > 50 && sum < 150)  value = ((cellValue - 3) < 0) ? 0 : (cellValue - 3);
    else                             value = 1;
    
    return value;
}

/*****************************  computeSum  *****************************
 * int computeSum(int i, int j)
 *
 * Description: Takes a 2D array and computes sum of B[i][j] and
 * its neighbors.  Returns this value to the calling environment.
 *
 * Process:
 *     Assumption: no indices i or j are passed to this function
 *     that will be out of bounds or not capable of being computed.
 * 1.) The values of neighbor cells are hardcoded and added together
 *     with B[i][j].
 * 2.) Sum is returned.
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * i            in          row of computed element
 * j            in          column of computed element
 *
 * NOTES:
 * - Please see assumption under Process above.
 * - Uses globally defined array B.
 *
 * Neighbor diagram (assume X is B[i][j]):
 *    Start here:  1 ->  2  -> 3
 *                             |
 *                             V
 *                 8 ->  X     4
 *                 ^           |
 *                 |           V
 *                 7 <-  6  <- 5
 * Neighbor order goes 1, 2, 3, 4, 5, 6, 7, 8, X
 ***********************************************************************/
int computeSum(int i, int j)
{
    int sum = 0;
    sum = B[i - 1][j - 1] +   // 1
    B[i - 1][  j  ] +         // 2
    B[i - 1][j + 1] +         // 3
    B[  i  ][j + 1] +         // 4
    B[i + 1][j + 1] +         // 5
    B[i + 1][  j  ] +         // 6
    B[i + 1][j - 1] +         // 7
    B[  i  ][j - 1] +         // 8
    B[  i  ][  j  ];          // X
    return sum;
}

/*****************************  updateCells  *****************************
 * void updateCells(int start, int end)
 *
 * Description: Takes a 2D array and computes a new positive integer
 * value for each applicable cell based on a set of rules.  The new
 * value is stored in another array.
 *
 * Process:
 * 1.) For each cell in gloabl array B, call computeSum for indices
 *     i and j.
 * 2.) Sum is used with a set of conditions and rules to determine
 *     the new value of the cell in global array A.  A separate function 
 *     is called for this part, newValue(sum).
 * 3.) The new value is stored into global array A.
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * start        in          first row worked on
 * end          in          last row worked on
 *
 * NOTES:
 * - The outer perimeter is not in play.  The outer perimeter of both
 *   arrays are 0's used to help programmer.
 ***********************************************************************/
void updateCells(int start, int end)
{
    int i;
    int j;
    int sum = 0;
    for (i = start; i < end; i++)
    {
        for (j = 1; j < N - 1; j++)
        {
            // get sum of neighbors and current cell
            sum = computeSum(i, j);
            // store newValue based on rules into arr2
            A[i][j] = newValue(sum, B[i][j]);
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
        updateCells(startRow + 1, endRow + 1);
    
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
    fillRandomly(M, N, B);
    // Print out values returned by random filling function
    printf("Initial Values ---------------------------  \n");
    print(M, N, B);
    
    // Array A always contains current values, array B is used for intermediate results
    while (CURRENT_GENERATION < TOTAL_GENERATIONS)
    {
        spinUpThreads();
        printf("Gen:  %d ---------------------------  \n", CURRENT_GENERATION);
        print(M, N, A);
        copyArray(M, N, A, B);  // copy values from A into B
        CURRENT_GENERATION++;
    }
    
    return 0;
}
