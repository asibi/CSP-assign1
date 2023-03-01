#!/bin/bash

set -e

# Compile program 
gcc src/Program.c -pthread -o program

mkdir -p results

# Iterate over threads (t) and hash bits (b)
for t in {1..32}
do
    for b in {1..18}    
    do
        # Run program with t and b
        echo "Running: t=$t b=$b"
        algorithm=2
        ./program $algorithm $t $b > results/output-$t-$b.txt # todo specify algorithm 
    done
done


