#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    string name;
    //char initials[10];
    //int j = 0;
    
    
    printf("Whaddup gimme yo name\n");
    name = get_string();
    
    if (name != NULL)                                       //Check that the string input is valid
    {
        printf("%c", toupper(name[0]));                     //straight up print the first letter/char of the entered string
        
        for(int i = 0; i < strlen(name); i++)               //no. of iterations equals the length of the string or no. of chars in a string
        {
            do
            {
                if(name[i] == ' ')                              //if the ith character in the string is a space....
                {
                    printf("%c", toupper(name[i+1]));
                } 
            }
            while(name[i] == '\0');
        }
        
        printf("\n");
    }

    
}

