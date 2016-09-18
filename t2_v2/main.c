/***********************************************************************
 * t2_v2.c written by Timothy Hennessy
 *
 * What's new: 
 * -Adds random array filling.  Fills array's of MxN size,
 *  using constants M and N to determine dimensions.
 * -If a value is negative, it is replaced by zero before
 *  being transfered into new array cell.
 * -Created new arrays.c to store common array functions and
 *  random array filling function.
 * -Created arrays.h to store all constants and the arrays.c
 *  function prototypes.
 *
 * Description: Given an MxN matrix compute the
 * sums of each cell and its neighbors.  Use the sum
 * and the set of rules to derive the each cell's
 * new value.
 *
 * compile: %gcc -o t2_v2 t2_v2.c
 * execute: ./t2_v2
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
#include "arrays.h"

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
 * - If value is negative, we return the absolute value of it to prevent
 * it from "going negative."
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
    else if (sum > 50 && sum <= 150) value = ((cellValue - 3) < 0) ? 0 : (cellValue - 3);
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
 * rows         in          total number of rows in arrays
 * cols         in          total number of columns in arrays
 * arr1         in          values used in computing new positive
 *                          integer values for each cell
 * arr2         out         used to return new positive values
 *
 * NOTES:
 * - C99 and later accept function declaration as long as column values
 *   for 2D array appear in parameter list before arrays.
 * - The outer perimeter is not in play.  The outer perimeter of both
 *   arrays are 0's used to help programmer.
 ***********************************************************************/
void updateCells(int rows, int cols, int arr1[][cols], int arr2[][cols])
{
    int i;
    int j;
    int sum = 0;
    // Always start updating cells at arr1[1][1]
    // Always end arr1[rows - 2][cols - 2]
    for (i = 1; i < rows - 1; i++)
    {
        for ( j = 1; j < cols - 1; j++)
        {
            // get sum of neighbors and current cell
            sum = computeSum(cols, i, j, arr1);
            // store newValue based on rules into arr2
            arr2[i][j] = newValue(sum, arr1[i][j]);
        }
    }
}

int main(int argc, const char * argv[])
{
    // TODO: test to make sure M and N are sufficiently large
    int generation = 0;
    int totalGenerations = GENERATIONS;
    int A[M][N] = {0};
    //int B[M][N];
    
    /*
    fillRandomly(M, N, B);
    printf("Gen:  %d ---------------------------  \n", generation);
    print(M, N, B);
     */
    
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
    
    printf("Gen:  %d ---------------------------  \n", generation);
    print(M, N, B);
    
    // Will loop for the value of totalGenerations
    // generation is used to determine the order of
    // switching arrays used in intermediate calculations
    while (generation < totalGenerations)
    {
        if (generation % 2 == 0)
        {
            // transfer new values based on rules into array A
            updateCells(M, N, B, A);
            printf("Gen:  %d ---------------------------  \n", generation + 1);
            print(M, N, A);
            //printf("Sum is %d\n", computeSum(n, 2, 1, B));
        }
        else
        {
            // transfer new values based on rules into array B
            updateCells(M, N, A, B);
            printf("Gen:  %d ---------------------------  \n", generation + 1);
            print(M, N, B);
        }
        generation++;
    }
    
    return 0;
}
