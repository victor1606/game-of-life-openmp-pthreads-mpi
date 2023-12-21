#!/bin/bash

INPUT_LARGE="./in/input.txt"
INPUT_SMALL="./in/input_small.txt"
OUTPUT_LARGE_SERIAL="./out/serial_large.out"
OUTPUT_LARGE_OPENMP="./out/openmp_large.out"
OUTPUT_LARGE_PTHREADS="./out/pthreads_large.out"
OUTPUT_LARGE_MPI="./out/mpi_large.out"
OUTPUT_SMALL_SERIAL="./out/serial_small.out"
OUTPUT_SMALL_OPENMP="./out/openmp_small.out"
OUTPUT_SMALL_PTHREADS="./out/pthreads_small.out"
OUTPUT_SMALL_MPI="./out/mpi_small.out"

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <test_type>"
    echo "Available test types: large, small"
    exit 1
fi

INPUT=""
OUTPUT_SERIAL=""
OUTPUT_OPENMP=""
OUTPUT_PTHREADS=""
OUTPUT_MPI=""

test_type="$1"

if [ "$test_type" == "small" ]; then
    INPUT=$INPUT_SMALL
    OUTPUT_SERIAL=$OUTPUT_SMALL_SERIAL
    OUTPUT_OPENMP=$OUTPUT_SMALL_OPENMP
    OUTPUT_PTHREADS=$OUTPUT_SMALL_PTHREADS
    OUTPUT_MPI=$OUTPUT_SMALL_MPI

    echo "Running tests for small input..."    
elif [ "$test_type" == "large" ]; then
    INPUT=$INPUT_LARGE
    OUTPUT_SERIAL=$OUTPUT_LARGE_SERIAL
    OUTPUT_OPENMP=$OUTPUT_LARGE_OPENMP
    OUTPUT_PTHREADS=$OUTPUT_LARGE_PTHREADS
    OUTPUT_MPI=$OUTPUT_LARGE_MPI

    echo "Running tests for large input..."
fi


echo -n "serial time:"
time ./bin/serial_gol "$INPUT" > "$OUTPUT_SERIAL"
echo ""

echo -n "openmp time:"
time ./bin/openmp_gol "$INPUT" > "$OUTPUT_OPENMP"
echo ""

echo -n "pthreads time:"
time ./bin/pthreads_gol "$INPUT" > "$OUTPUT_PTHREADS"
echo ""

echo -n "mpi time:"
time mpirun -n 8 --oversubscribe ./bin/mpi_gol "$INPUT" > "$OUTPUT_MPI"
echo ""

echo "diff openmp:"
diff "$OUTPUT_SERIAL" "$OUTPUT_OPENMP"
echo ""

echo "diff pthreads:"
diff "$OUTPUT_SERIAL" "$OUTPUT_PTHREADS"
echo ""

echo "diff mpi:"
diff "$OUTPUT_SERIAL" "$OUTPUT_MPI"
