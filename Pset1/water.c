#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int minutes;    
        printf("Enter number of minutes\n"); //prompt user to enter input
        minutes = get_int();
        
        if (minutes > 0)
        {
            int bottles = minutes * 12;
            printf("Minutes: %i\nBottles: %i\n", minutes, bottles);
        }
}

 
