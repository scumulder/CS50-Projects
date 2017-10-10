/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

//for loop variables
int i, j;

//for all the swapping that'll happen
int swap, shift;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");                         //the "w" stands for writing mode. We're writing to the file
    if (file == NULL)                                           //if the file doesn't exist...
    {
        return 3;
    }

    greet();                                                    // greet user with instructions

    init();                                                     // initialize the board

    while (true)                                                // accept moves until game is won
    {
        clear();                                                // clear the screen

        draw();                                                 // draw the current state of the board
   // break; //THIS HERE

        for (int i = 0; i < d; i++)                             // log the current state of the board (for testing)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(900000);             // usleep(500000)
        }

        // sleep thread for animation's sake
        usleep(900000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

void clear(void)
{
    /* Clears screen using ANSI escape sequences */
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

void greet(void)
{
    /* Greets player */
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(900000);
}

/********************************* FUNCTION: INIT *********************************/
void init(void)
{
    /* Initializes the game's board with tiles numbered 1 through d*d - 1*/

    if(d%2 == 0)                                        //if dimension d is an even number
    {
        shift = 1;
        for(i = 0; i<d; i++)                    //ith row
        {
            for(j = 0; j<d; j++)                //jth column
            {
                    board[i][j] = (d*d) - shift;
                    shift++;

                    if(board[i][j] == 1)                        //This part of the code switches the position of 1 and 2 on the board
                    {
                        swap = board[i][j];
                        board[i][j] = board[i][j-1];
                        board[i][j-1] = swap;
                    }

            }
        }
    }

    if(d%2 != 0)                                                //if dimension d is an odd number
    {
        shift = 1;
        for(i = 0; i<d; i++)                                    //ith row
        {
            for(j = 0; j<d; j++)                                //jth column
            {
                    board[i][j] = (d*d) - shift;
                    shift++;
            }
        }
    }
}

/********************************* FUNCTION: DRAW *********************************/
void draw(void)
{
   /* Prints the board in its current state */
    for(i = 0; i<d; i++)
    {
       for(j = 0; j<d; j++)
       {
           printf("%2i  ", board[i][j]);
       }
        printf("\n");
    }

}
/********************************* FUNCTION: MOVE *********************************/
bool move(int tile)
{
    /* If tile borders empty space, moves tile and returns true, else returns false */
    int t_row, t_col, s_row, s_col;

    //Find the location of the tile
    for(i = 0; i<d; i++)
    {
       for(j = 0; j<d; j++)
       {
            if(board[i][j] == tile)
            {
                t_row = i;
                t_col = j;
                //printf("tile location is %ix%i \n", t_row, t_col);                         //FOR TESTING
            }
       }
    }

    //Find location of the space '0'
    for(i = 0; i<d; i++)
    {
       for(j = 0; j<d; j++)
       {
            if(board[i][j] == 0)
            {
                s_row = i;
                s_col = j;
                //printf("space location is %ix%i \n", s_row, s_col);                       //FOR TESTING
            }
       }
    }
    //Check if the tile is touching the empty space, i.e. '0'
    if((t_row == s_row && (t_col - 1) == s_col) || (t_row == s_row && (t_col + 1) == s_col) || ((t_row - 1) == s_row && t_col == s_col) || ((t_row + 1) == s_row && t_col == s_col))
    {
        //printf("it's valid bitch! \n");                                                   //FOR TESTING
        swap = board[s_row][s_col];
        board[s_row][s_col] = board[t_row][t_col];
        board[t_row][t_col] = swap;

        //usleep(900000);
        return true;
    }
    else
    {
        return false;
    }
}

/********************************* FUNCTION: WON *********************************/
bool won(void)
{
   /* Returns true if game is won (i.e., board is in winning configuration), else false */

    shift = (d*d) - 1;

    for(i = 0; i<d; i++)                                //ith row
    {
        for(j = 0; j<d; j++)                            //jth column
        {
            if((i == d-1) && (j == d-1))                //if the tile you're checking is the last one, check if it's zero
            {
                if(board[i][j] == 0)
                {
                    //printf("I'm a real booarrrdddd!! \n");
                }
                else
                {
                    //printf("So close, where the zero at? \n");
                    return false;
                }
            }

            else if(board[i][j] != (d*d) - shift)       //FOR E.G. if d = 4, check if board[0][0] == 16 - (16 - 1) == 16 - 15 == 1
            {
                //printf("I'm NOT a real board :( \n");
                return false;
            }

            shift--;
        }
    }
    return true;
}
