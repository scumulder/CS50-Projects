/**
 * Prompts user for as many as MAX values until EOF is reached,
 * then proceeds to search that "haystack" of values for given needle.
 *
 * Usage: ./find needle
 *
 * where needle is the value to find in a haystack of values
 */

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

// maximum amount of hay
const int MAX = 65536;

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)                                  //only accept arguments "./find" and "needle"
    {
        printf("Usage: ./find needle\n");
        return -1;
    }

    // remember needle
    int needle = atoi(argv[1]);                     //convert "needle" to an actual integer
                                                    //this is the integer that we will search for in the "haystack"

    // fill haystack
    int size;
    int haystack[MAX];                              //haystack is an array that has a size of 65536
    for (size = 0; size < MAX; size++)              //from 0 to 65536
    {
        // wait for hay until EOF i.e. CTRL+d
        printf("\nhaystack[%i] = ", size);          //haystack[0], haystack[1],....,haystack[65536]
        int straw = get_int();                      //user enters an integer value
        if (straw == INT_MAX)                       //what is INT_MAX?
        {
            break;
        }

        // add hay to stack
        haystack[size] = straw;                     //add the integer "straw" to the address haystack[size]

    }
    printf("\n");

    // sort the haystack
    sort(haystack, size);                           //look at helpers.c to see what sort does

    // try to find needle in haystack
    if (search(needle, haystack, size))             //look at helpers.c to see what search does
    {
        printf("\nFound needle in haystack!\n\n");
        return 0;
    }
    else
    {
        printf("\nDidn't find needle in haystack.\n\n");
        return 1;
    }
}
