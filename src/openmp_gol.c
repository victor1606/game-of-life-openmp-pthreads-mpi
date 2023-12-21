#include <stdio.h>
#include <omp.h>

/* dimensions of the screen */
#define BOARD_WIDTH 500
#define BOARD_HEIGHT 500
// #define BOARD_WIDTH 10
// #define BOARD_HEIGHT 10

/* set everything to zero */
int board[BOARD_WIDTH][BOARD_HEIGHT];
int newboard[BOARD_WIDTH][BOARD_HEIGHT];

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

void play() {
    /*
        (copied this from some web page, hence the English spellings...)

        1.STASIS : If, for a given cell, the number of on neighbours is
        exactly two, the cell maintains its status quo into the next
        generation. If the cell is on, it stays on, if it is off, it stays off.

        2.GROWTH : If the number of on neighbours is exactly three, the cell
        will be on in the next generation. This is regardless of the cell's current state.

        3.DEATH : If the number of on neighbours is 0, 1, 4-8, the cell will
        be off in the next generation.
    */
    int i, j, a;

#pragma omp parallel for private(i, j, a) shared(newboard)
    for (i = 0; i < BOARD_WIDTH; i++) {
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

    for (i = 0; i < BOARD_WIDTH; i++) {
        for (j = 0; j < BOARD_HEIGHT; j++) {
            board[i][j] = newboard[i][j];
        }
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

    f = fopen(name, "r");
}

int main(int argc, char *argv[]) {
    int i;

    if (argc != 2) {
        perror("Usage: ./gol [input_file]");
        return -1;
    }

    initialize_board(board);
    read_file(board, argv[1]);

    /* Play the game of life for 2000 generations */
    for (i = 0; i < 2000; i++) {
        // Uncomment the line below if you want to print each generation
        // print(board);
        play();
    }

    return 0;
}
