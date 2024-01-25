CC = gcc
MPICC = mpicc
CFLAGS = -Wall -Wextra -g
INPUT_LARGE = ./in/input.txt
INPUT_SMALL = ./in/input_small.txt
OUTPUT_LARGE_SERIAL = ./out/serial_large.out
OUTPUT_LARGE_OPENMP = ./out/openmp_large.out
OUTPUT_LARGE_PTHREADS = ./out/pthreads_large.out
OUTPUT_LARGE_MPI = ./out/mpi_large.out
OUTPUT_SMALL_SERIAL = ./out/serial_small.out
OUTPUT_SMALL_OPENMP = ./out/openmp_small.out
OUTPUT_SMALL_PTHREADS = ./out/pthreads_small.out
OUTPUT_SMALL_MPI = ./out/mpi_small.out

build:
	$(CC) serial_gol.c -o ./bin/serial_gol $(CFLAGS)
	$(CC) pthreads_gol.c -pthread -o ./bin/pthreads_gol $(CFLAGS)
	$(CC) openmp_gol.c -fopenmp -o ./bin/openmp_gol $(CFLAGS)
	$(MPICC) mpi_gol.c -o ./bin/mpi_gol $(CFLAGS)

run: run_large

run_large:
	./bin/serial_gol $(INPUT_LARGE) > $(OUTPUT_LARGE_SERIAL)
	./bin/openmp_gol $(INPUT_LARGE) > $(OUTPUT_LARGE_OPENMP)
	./bin/pthreads_gol $(INPUT_LARGE) > $(OUTPUT_LARGE_PTHREADS)
	mpirun -n 8 --oversubscribe ./bin/mpi_gol $(INPUT_LARGE) > $(OUTPUT_LARGE_MPI)

run_small:
	./bin/serial_gol $(INPUT_SMALL) > $(OUTPUT_SMALL_SERIAL)
	./bin/openmp_gol $(INPUT_SMALL) > $(OUTPUT_SMALL_OPENMP)
	./bin/pthreads_gol $(INPUT_SMALL) > $(OUTPUT_SMALL_PTHREADS)
	mpirun -n 8 --oversubscribe ./bin/mpi_gol $(INPUT_SMALL) > $(OUTPUT_SMALL_MPI)

clean:
	rm -rf serial_gol pthreads_gol openmp_gol mpi_gol
