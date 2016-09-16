#include <stdio.h>
#include <stdlib.h>
#include "arrays.h"

/*****************************  fillRandomly  *****************************
 * void fillRandomly(int rows, int cols, int arr[][cols])
 *
 * Description: Takes a 2D array pointer and fills it with random values
 * less than SEED (constant defined in arrays.h). Takes care to ensure outer bounds
 * are 0.
 *
 * Process:
 * 1.) Fill boundary rows and columns with zeroes.
 * 2.) Fill all others with random values
 *
 * Parameter    Direction   Description
 * --------------------------------------------------------------------
 * rows         in          total number of rows in array
 * cols         in          total number of columns in array
 * arr          out         filled with 0s around outer bounds
 *                          and random values between
 *
 * NOTES:
 * - C99 and later accept function declaration as long as column values
 *   for 2D array appear in parameter list before arrays.
 ***********************************************************************/
void fillRandomly(int rows, int cols, int arr[][cols])
{
    int i, j;
    int lastRow = rows - 1;
    int lastCol = cols - 1;
    
    /*************** 1 - Fill outer rows/columns with 0 *****************/
    // first row
    for (i = 0; i < cols; i++)
        arr[0][i] = 0;
    
    // last row
    for (i = 0; i < cols; i++)
        arr[lastRow][i] = 0;
    
    // first column
    for (i = 0; i < rows; i++)
        arr[i][0] = 0;
    
    // last column
    for (i = 0; i < rows; i++)
        arr[i][lastCol] = 0;
    
    /*************** 2 - Fill all others randomly *****************/
    // Always start at arr1[1][1]
    // Always end arr1[rows - 2][cols - 2]
    for (i = 1; i < lastRow; i++)
    {
        for (j = 1; j < lastCol; j++)
        {
            arr[i][j] = rand() % RANGE;
        }
    }
}

/****************************   print  ********************************
 * void print(int rows, int cols, int arr[][cols])
 *
 * Description: Used for printing out 2D array values.
 *
 * Process:
 * 1.) Standard 2D array printing function, nothing special.
 *
 * Parameter     Direction   Description
 * ---------------------------------------------------------------------
 * rows          in          total number of rows in arr
 * cols          in          total number of columns in arr
 * arr           in          printed array
 *
 * NOTES:
 * - C99 and later accept function declaration as long as column values
 *   for 2D array appear in parameter list before arrays.
 ***********************************************************************/
void print(int rows, int cols, int arr[][cols])
{
    int i;
    int j;
    printf("\n");
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            printf("%-6d", arr[i][j]);
        }
        printf("\n\n");
    }
}
