#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, string argv[])                       //argv[0] = ./caesar, argv[1] = k
{
    if(argc != 2)
    {
        printf("Usage: ./caesar k\n");
        return 1;
    }

    int k;
    k = atoi(argv[1]);
    while(k>26)
    {
        k = k - 26;
    }

    string plaintext;
    int ciphertext;
    printf("plaintext: ");
    plaintext = get_string();
    printf("ciphertext: ");

    for(int i = 0; i < strlen(plaintext); i++)
    {
       do
        {
            if(isalpha(plaintext[i]))
            {
                if(islower(plaintext[i]))
                {
                    ciphertext = (int) plaintext[i] + k;

                    if (ciphertext > 122)
                    {
                        ciphertext = ciphertext - 26;
                    }

                    printf("%c", (char) ciphertext);
                }

                else if(isupper(plaintext[i]))
                {
                    ciphertext = (int) plaintext[i] + k;

                    if (ciphertext > 90)
                    {
                        ciphertext = ciphertext - 26;
                    }
                    printf("%c", (char) ciphertext);
                }
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