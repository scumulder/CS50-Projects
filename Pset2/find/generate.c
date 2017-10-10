/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,LIMIT), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // TODO: if the number of arguments entered is not 2 OR 3, return an error
    /*
                    argc!=2     argc!=3     if()    
        argc = 1 |      1    |    1     |    1
               2 |      0    |    1     |    0
               3 |      1    |    0     |    0
               4 |      1    |    1     |    1
    */
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // TODO: turn the string entered as an argument, change it to type int and store it in n
    int n = atoi(argv[1]);

    // TODO: if a seed value was entered at start of program execution, convert it from string to int within the srand48 function
    //       that seeds drand48. 
    //       if a seed value wasn't entered, use time(NULL) to seed it
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // TODO: Once all the seeding is done, generate n random numbers
    //       drand48 generates double numbers between [0.0, 1.0). so when you multiply it by LIMIT,
    //       you are changing the range to [0, LIMIT)
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
