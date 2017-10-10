/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>
#include "helpers.h"
#include <stdio.h>
#include <math.h>

/********************************************** FUNCTION: SEARCH *********************************************/
/*  Returns true if value is in array of n values, else false.

    1. The implementation must return false immediately if n is non-positive.
    2. The implementation must return true if value is in values and false if value is not in values.
    3. The running time of your implementation must be in O(log n).
*/

bool search(int value, int values[], int n)     //28, {27,28,29}, 3
{
    // TODO: implement a searching algorithm

    int first, last, middle;
    int size;

    first = 0;          //[0]
    last = n-1;         //3-1 = [2]
    middle = floor((first + last) /2);      //[1]
    size = last - first + 1;        //3

    while(size > 0)
    {
        if(values[middle] == value)
        {
            return true;
        }

        if(values[middle] > value)
        {
            last = middle - 1;
            middle = floor((last + first)/2);
        }

        if(values[middle] < value)
        {
            first = middle + 1;
            middle = floor((last + first)/2);
        }

        size = last - first + 1;
    }

    return false;
/*Simple version: */
    // if(n < 0)
    // {
    //     return false;
    // }

    // for(int i = 0; i<n; i++)
    // {
    //     if(values[i] == value)
    //     {
    //         return true;
    //     }
    // }

}

/*********************************************** FUNCTION: SORT **********************************************/
/*  Sorts array of n values.

    1. Your implemenation must sort, from smallest to largest, the array of numbers that it’s passed.
    2. The running time of your implementation must be in O(n2), where n is the array’s size.
*/
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm
    int min, min_index, temp;

    /*INSERT SORTING ALGORITHM*/
    for(int i = 0; i<(n - 1); i++)   //2
    {
        min = values[i];             //20
        for(int k = (i+1); k<(n); k++)       //3 to 7
        {
            if(min > values[k])
            {
                min = values[k];
                min_index = k;
                temp = values[i];
                values[i] = values[min_index];
                values[min_index] = temp;
            }
        }
    }
        // for(int j = 0; j< n; j++)
        // {
        //   printf("%i, ", values[j]);
        // }
        // printf("\n");

    return;
}
