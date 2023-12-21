#include <stdio.h>
#include <pthread.h>

#define BOARD_WIDTH 500
#define BOARD_HEIGHT 500

int board[BOARD_WIDTH][BOARD_HEIGHT];
int newboard[BOARD_WIDTH][BOARD_HEIGHT];

#define NUM_THREADS 8

void initialize_board(int board[][BOARD_HEIGHT]) {
    int i, j;
    for (i = 0; i < BOARD_WIDTH; i++)
        for (j = 0; j < BOARD_HEIGHT; j++)
            board[i][j] = 0;
}

int xadd(int i, int a) {
    i += a;
    while (i < 0)
        i += BOARD_WIDTH;
    while (i >= BOARD_WIDTH)
        i -= BOARD_WIDTH;
    return i;
}

int yadd(int i, int a) {
    i += a;
    while (i < 0)
        i += BOARD_HEIGHT;
    while (i >= BOARD_HEIGHT)
        i -= BOARD_HEIGHT;
    return i;
}

int adjacent_to(int board[][BOARD_HEIGHT], int i, int j) {
    int k, l, count;
    count = 0;
    for (k = -1; k <= 1; k++)
        for (l = -1; l <= 1; l++)
            if (k || l)
                if (board[xadd(i, k)][yadd(j, l)])
                    count++;
    return count;
}

void *play_region(void *arg) {
    int id = *(int *)arg;
    int a;

    int start_row = id * BOARD_HEIGHT / NUM_THREADS;
    int end_row = (id + 1) * BOARD_HEIGHT / NUM_THREADS - 1;
    if (id == NUM_THREADS - 1) {
        end_row = BOARD_HEIGHT - 1;
    }

    for (int i = start_row; i <= end_row; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
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

    return NULL;
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

void parallel_play() {
    pthread_t threads[NUM_THREADS];
    int args[NUM_THREADS];

    for (int t = 0; t < NUM_THREADS; t++) {
        args[t] = t;
        pthread_create(&threads[t], NULL, play_region, &args[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    swap_boards();
}

int read_file(int board[][BOARD_HEIGHT], char *name)
{
    FILE *f;
    int i, j;
    int width, height;

    f = fopen(name, "r");
    if (f == NULL)
    {
        printf("Can't open file %s\n", name);
        return -1;
    }
    fscanf(f, "%d %d\n", &width, &height);

    while (fscanf(f, "%d %d\n", &i, &j) != EOF)
    {
        board[i][j] = 1;
    }
    fclose(f);

    f = fopen(name, "r");
    return 0;
}

int main(int argc, char *argv[]) {
    int i, result;

    if (argc != 2) {
        perror("Usage: ./pthreads_gol [input_file]");
        return -1;
    }

    initialize_board(board);
    result = read_file(board, argv[1]);
    if(result == -1)
    {
        return 0;
    }
    
    /* Play the game of life for 2000 generations */
    for (i = 0; i < 2000; i++) {
        // Uncomment the line below if you want to print each generation
        //print(board);
        parallel_play();
    }
    return 0;
}
