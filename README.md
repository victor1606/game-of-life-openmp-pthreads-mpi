## Conway's Game of Life

Game of Life is a cellular automaton, a 0-player "game" that runs on a two-dimensional square grid. The cells on the grid can have a value of 1 (alive) or 0 (dead) and evolve according to the following rules:

* a living cell with less than two living neighbours dies.
* a living cell with more than three living neighbours dies.
* a living cell with two or three living neighbours survives, unchanged, into the next generation.
* a dead cell with exactly three living neighbours becomes alive.

The initial cell configuration may be created by a human, but all subsequent generations are completely determined by the above rules. The goal of the game is to find patterns that evolve in interesting ways.

## Usage

To generate an initial table:
```
python3 ./in/gen_test.py \[width\] \[height\]
```

To compile & run:   
```
make   
make run_large (500x500 test)  
```
OR   
```
make
make run_small (10x10 test)
```

For testing run times + algorithm correctness:
```
./test.sh \[small/large\]
```

## Profiling serial code


Functions to be parallelized (with maximum CPU Time on serial version):

```
void play(int board[][BOARD_HEIGHT])
{
    int i, j, a, newboard[BOARD_WIDTH][BOARD_HEIGHT];

    /* for each cell, Life rules apply */
    for (i = 0; i < BOARD_WIDTH; i++)
    {
        for (j = 0; j < BOARD_HEIGHT; j++)
        {
            a = adjacent_to(board, i, j);
            if (a == 2)
                newboard[i][j] = board[i][j];
            if (a == 3)
                newboard[i][j] = 1;
            if (a < 2)
                newboard[i][j] = 0;
            if (a > 3)
                newboard[i][j] = 0;
        }
    }

    /* copy new board back to old board */
    for (i = 0; i < BOARD_WIDTH; i++)
        for (j = 0; j < BOARD_HEIGHT; j++)
        {
            board[i][j] = newboard[i][j];
        }
}

/* returns the number of on cells adjacent to cell i,j */
int adjacent_to(int board[][BOARD_HEIGHT], int i, int j)
{
    int k, l, count;
    count = 0;

    /* surround cell */
    for (k = -1; k <= 1; k++)
        for (l = -1; l <= 1; l++)
            /* count only if at least one of k,l is non-zero */
            if (k || l)
                if (board[xadd(i, k)][yadd(j, l)])
                    count++;
    return count;
}
```

## OpenMP scheduling comparison
The graphs show that the optimal variant is the one with chunk size of 10. The time differences for the variant with 8 threads propagate to the variants with a higher number of threads, and it is observed that they become even more pronounced (the difference between the variant with chunk size of 100 and 10 becomes even greater for 16 and 32 threads). The scheduling policy does not affect the execution time very much, the fastest variant for each type remaining the one with chunk size of 10.  


## Speedup comparison Haswell
It can be seen that the openMP variant has the highest speedup, being close to pthreads except for some bumps that occur for the 16 and 24 threads threads. The speedup of the MPI variant decreases as the number of threads increases. In all three variants, bumps can be seen for the 4 and 6 threaded threads, with the speedup decreasing sharply for the second variant. After the 24-thread variant, the speedup for pthreads remains approximately constant.

## Efficiency comparison Haswell
The above results are also reflected for the efficiency, which decreases as the number of threads increases, with the same differences being observed for 6, 16 and 24 threads.

## Speedup comparison Nehalem
The results are similar to those on the Haswell partition, showing the same slowing trends for the MPI version and the same differences between Pthreads and OpenMP.

## Efficiency comparison Nehalem
As with the Haswell partition, the efficiency of the 3 variants decreases as the number of threads chosen increases. For MPI it is closest to 0.
