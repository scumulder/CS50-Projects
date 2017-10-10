/* Mario Less */

#include <stdio.h>
#include  <cs50.h>

int main(void)
{
    int height, width, col;

    do
    {
        printf("Enter a height value between 0 and 23\n");
        height = get_int();
    }
    while (height < 0 || height > 23);

    width = height + 1;

    for (int i = 0; i < height ; i++)                   //eg. height = 0, 1, 2, 3 (or row)
    {
          //now in rows 0, 1, 2, 3

          for (col = i; col < (height - 1) ; col++)         //1st: (height - 1) = 3; col = 0, 1, 2
          {                                                 //2nd: (3 - 1) = 2; col = 0, 1
              printf(" ");                                  //3rd: (2 - 1) = 1; col = 0
          }

          for (col = (height - 1 - i); col < width; col++)      //col = 3, 4
          {
              printf("#");
          }

        printf("\n");
    }
}
