#!/bin/bash

# Compile program 
gcc src/Program.c -o program

# Iterate over threads (t) and hash bits (b)
for t in {1..32}
do
    for b in {0..18}    
    do
        # Run program with t and b
        echo "Running: t=$t b=$b"
        algorithm=1
        ./program algorithm $t $b > results/output-$t-$b.txt # todo specify algorithm 
    done
done


