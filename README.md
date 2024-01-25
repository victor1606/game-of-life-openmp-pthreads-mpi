# Conway's Game of Life

## Description

Conway's Game of Life is a cellular automaton, a "game" with 0 players that unfolds on a two-dimensional square grid. The cells on the grid can have a value of 1 (alive) or 0 (dead) and evolve according to the following rules:

A live cell with fewer than two live neighbors dies.
A live cell with more than three live neighbors dies.
A live cell with two or three live neighbors survives, unchanged, in the next generation.
A dead cell with exactly three live neighbors becomes alive.
The initial configuration of cells can be created by a human, but all subsequent generations are entirely determined by the rules above. The goal of the game is to find patterns that evolve in interesting ways.

## Usage
To generate an initial grid: <br>
    ```python3 ./in/gen_test.py [width] [height]```

For compilation and execution: <br>
    `make`<br>
    `make run_large (500x500 test)`<br>
    OR <br>
    `make run_small (10x10 test)`

For testing runtime performance and algorithm correctness: <br>
    `./test.sh [small/large]`
