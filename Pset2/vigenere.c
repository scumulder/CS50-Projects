#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if(argc != 2)                                               //we only want two arguments: the keyword and ./vigenere
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }
    
    char k[strlen(argv[1])];   
    
    for(int i = 0; i<strlen(argv[1]); i++)
    {
        if(isupper(argv[1][i]))                     //if the ith character in the keyword is uppercase
        {
            k[i] = argv[1][i] - 65;
            //printf("%i\n", k[i]);
        }
        
        else if(islower(argv[1][i]))                ////if the ith character in the keyword is lowercase
        {
            k[i] = argv[1][i] - 97;
            //printf("%i\n", k[i]);
        }
        
        else
        {
            printf("Usage: ./caesar k\n");
            return 1;
        }
    }
    

    string plaintext;
    int ciphertext;
    int j = 0;
    printf("plaintext: ");
    plaintext = get_string();
    printf("ciphertext: ");
    
    for(int i = 0; i < strlen(plaintext); i++)
    {
        do
        {
            if(isalpha(plaintext[i]))
            {
                if(j > (strlen(argv[1]) - 1))
                {
                    j = 0;
                }
                ciphertext = (int) plaintext[i] + k[j];                //k was constant in caesar. In vigenere is changes based on the p[i]
                j++;
                if (islower(plaintext[i]) && ciphertext > 122)
                {
                     ciphertext = ciphertext - 26;
                }
                
                if (isupper(plaintext[i]) && ciphertext > 90)
                {
                     ciphertext = ciphertext - 26;
                }
                
                printf("%c", (char) ciphertext);
            }
           
          else
            {
                printf("%c", plaintext[i]);
            }
        }
        while (plaintext[i] == '\0');    
    }
    
    printf("\n");
}