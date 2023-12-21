#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

/* dimensions of the screen */
#define BOARD_WIDTH 500
#define BOARD_HEIGHT 500
//  #define BOARD_WIDTH 10
//  #define BOARD_HEIGHT 10

/* set everything to zero */
int board[BOARD_WIDTH + 1][BOARD_HEIGHT];
int newboard[BOARD_WIDTH + 1][BOARD_HEIGHT];

void initialize_board(int board[][BOARD_HEIGHT]) {
    int i, j;
    for (i = 0; i < BOARD_WIDTH; i++)
        for (j = 0; j < BOARD_HEIGHT; j++)
            board[i][j] = 0;
}

/* add to a width index, wrapping around like a cylinder */
int xadd(int i, int a)
{
    i += a;
    while (i < 0)
        i += BOARD_WIDTH;
    while (i >= BOARD_WIDTH)
        i -= BOARD_WIDTH;
    return i;
}

/* add to a height index, wrapping around */
int yadd(int i, int a)
{
    i += a;
    while (i < 0)
        i += BOARD_HEIGHT;
    while (i >= BOARD_HEIGHT)
        i -= BOARD_HEIGHT;
    return i;
}

/* return the number of on cells adjacent to the i,j cell */
int adjacent_to(int board[][BOARD_HEIGHT], int i, int j)
{
    int k, l, count;

    count = 0;

    /* go around the cell */
    for (k = -1; k <= 1; k++)
        for (l = -1; l <= 1; l++)

            /* only count if at least one of k,l isn't zero */
            if (k || l)
                if (board[xadd(i, k)][yadd(j, l)])
                    count++;
    return count;
}

void play(int rank, int num_procs) {
    int i, j, a;
    int start_row = rank * (BOARD_HEIGHT / num_procs);
    int end_row = (rank + 1) * (BOARD_HEIGHT / num_procs);
    if (rank == num_procs - 1) {
            end_row = BOARD_HEIGHT;
        }

    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < BOARD_HEIGHT; j++) {
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

}

void swap_boards() {
    int i, j;
    for (i = 0; i < BOARD_WIDTH; i++)
        for (j = 0; j < BOARD_HEIGHT; j++) {
            board[i][j] = newboard[i][j];
        }
}

void print(int board[][BOARD_HEIGHT]) {
    int i, j;
    for (j = 0; j < BOARD_HEIGHT; j++) {
        for (i = 0; i < BOARD_WIDTH; i++) {
            printf("%c", board[i][j] ? 'x' : ' ');
        }
        printf("\n");
    }
}

/* read a file into the life board */
void read_file(int board[][BOARD_HEIGHT], char *name) {
    FILE *f;
    int i, j;
    int width, height;

    f = fopen(name, "r");
    fscanf(f, "%d %d\n", &width, &height);

    while (fscanf(f, "%d %d\n", &i, &j) != EOF) {
        board[i][j] = 1;
    }
    fclose(f);
}

int main(int argc, char *argv[]) {
    int i, rank, num_procs, dim;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    if (argc != 2) {
        if (rank == 0) {
            perror("Usage: mpirun -n [num_procs] ./gol_mpi [input_file]");
        }
        MPI_Finalize();
        return -1;
    }

    if(rank == 0){
        initialize_board(board);
        read_file(board, argv[1]);
        for (int t = 1; t < num_procs; t++) {
            MPI_Send(&(board[0][0]), BOARD_HEIGHT * BOARD_WIDTH, MPI_INT, t, 0, MPI_COMM_WORLD);
        }
    }
    dim = BOARD_HEIGHT / num_procs;

    if(rank > 0){
        MPI_Recv(&(board[0][0]), BOARD_HEIGHT * BOARD_WIDTH, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
    }

    /* Play the game of life for 2000 generations */
    for (i = 0; i < 2000; i++) {
        //  if(rank == 0)
        //     print(board);
        play(rank, num_procs);
        MPI_Barrier(MPI_COMM_WORLD);

        if(rank != 0 && rank < num_procs - 1)
            MPI_Send(&(newboard[rank * dim][0]), dim * BOARD_WIDTH, MPI_INT, 0, rank, MPI_COMM_WORLD);
        else if(rank == num_procs - 1)
            MPI_Send(&(newboard[rank * dim][0]), BOARD_WIDTH * (BOARD_HEIGHT - rank * (BOARD_HEIGHT / num_procs)), MPI_INT, 0, rank, MPI_COMM_WORLD);

         if(rank == 0){
            for(int k = 1;k < num_procs - 1 ;k++){
                MPI_Recv(&(newboard[k * dim][0]), dim * BOARD_WIDTH, MPI_INT, k, k, MPI_COMM_WORLD, NULL);
            }
            MPI_Recv(&(newboard[(num_procs - 1) * dim][0]), BOARD_WIDTH * (BOARD_HEIGHT - (num_procs - 1) * (BOARD_HEIGHT / num_procs)), MPI_INT, num_procs - 1, num_procs - 1, MPI_COMM_WORLD, NULL);
            for(int k = 1;k < num_procs;k++)
                MPI_Send(&(newboard[0][0]), BOARD_HEIGHT * BOARD_WIDTH, MPI_INT, k, 0, MPI_COMM_WORLD);
        }

        if(rank != 0)
            MPI_Recv(&(newboard[0][0]), BOARD_HEIGHT * BOARD_WIDTH, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
        swap_boards();
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
