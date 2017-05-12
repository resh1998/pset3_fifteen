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
int dim;

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
    dim = atoi(argv[1]);
    if (dim < DIM_MIN || dim > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < dim - 1)
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
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */

void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */

void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */

void init(void)
{

    int val = (dim * dim) - 1;
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            board[i][j] = val;
            val--; 
        }
    }

    if (dim % 2 == 0)
    {
        board[dim - 1][dim - 3] = 1;
        board[dim - 1][dim - 2] = 2;
    }
}

/**
 * Prints the board in its current state.
 */

void draw(void)
{
    int ascii = 95;
    for(int i = 0;i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            if (board[i][j] == 0) 
            {
                printf("%2c",ascii);
            }
            else
            {
                printf("%2i",board[i][j]);
            }    
        }

        printf("\n");

    }

    printf("\n"); 
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */

bool move(int tile)
{
    // location of underscore 
    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            if (board[i][j] == 0)
            {
                // location of tile
                for(int x = 0; x < dim; x++)
                {
                    for(int y = 0; y < dim; y++)
                    {
                        if (board[x][y] == tile)
                        {
                            int temp;
                            if (x == i && (y == (j + 1)))
                            {
                                temp = board[i][j];
                                board[i][j] = board[x][y];
                                board[x][y] = temp;
                                return true;
                            }

                            else if (x == i && (y == (j - 1)))
                            {
                                temp = board[i][j];
                                board[i][j] = board[x][y];
                                board[x][y] = temp;
                                return true;    
                            }

                            else if( j == y && (x == (i + 1)))
                            {
                                temp = board[i][j];
                                board[i][j] = board[x][y];
                                board[x][y] = temp;
                                return true;     
                            }

                            else if (j == y && (x == (i - 1)))
                            {
                                temp = board[i][j];
                                board[i][j] = board[x][y];
                                board[x][y] = temp;
                                return true;
                            }
                        }   
                    }
                }
            }
        }
    }

    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */

bool won(void)
{

    int block = 1;
    int count = 0;

    for(int i = 0; i < dim; i++)
    {
        for(int j = 0; j < dim; j++)
        {
            if ((i != (dim - 1)) || (j != (dim - 1)))
            {
                if (board[i][j] == block)
                {
                    block++;
                    count++;
                }

                else
                {
                    block++;
                }
            }
 
            else if (i == (dim - 1) && j == (dim - 1))
            {
                if (board[i][j] == 0)
                {
                    count++;
                }
            }
        }
    }

    if (count == (dim * dim))
    {
        return true;
    }
    return false;
}

