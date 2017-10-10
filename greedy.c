#include <stdio.h>
#include <cs50.h>
#include <math.h>

int main(void)
{
    float change, change2;
    int counter = 0;

    do
    {
        printf("O hai! How much change is owed?\n");
        change = get_float();
        change2 = round(100 * change);
    }
    while (change < 0.0);

    while (change2 >= 25)
    {
        change2 = change2 - 25;
        counter++;
    }

    while (change2 >= 10)
    {
        change2 = change2 - 10;
        counter++;
    }

    while (change2 >= 5)
    {
        change2 = change2 - 5;
        counter++;
    }

    while (change2 >= 1)
    {
        change2 = change2 - 1;
        counter++;
    }

    printf("%i\n", counter);
}