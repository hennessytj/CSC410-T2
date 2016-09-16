//
//  arrays.h
//  t2_v2
//
//  Created by Timothy Hennessy on 9/13/16.
//  Copyright © 2016 Timothy Hennessy. All rights reserved.
//

#ifndef arrays_h
#define arrays_h


// M = rows, N = columns for 2D arrays
// The number of columns must be known at compile time
#define M 8
#define N 8

// Used with rand() to determine range [0, RANGE)
#define RANGE 100
#define SEED  1

// Number of generations to run
#define GENERATIONS 1

// Booleans
#define true    1
#define false   0

// Errors
#define GENERIC_ERROR_CODE      10
#define ERROR_DIMENSION_SIZE    11

void fillRandomly(int rows, int cols, int arr[][cols]);
void print(int rows, int cols, int arr[][cols]);

#endif /* arrays_h */
