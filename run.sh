#!/bin/bash

# Compile program 
gcc Program.c -o program

# Iterate over threads (t) and hash bits (b)
for t in {1..32}
do
    for b in {0..18}    
    do
        # Run program with t and b
        echo "t: $t b: $b "
        ./program $t $b # todo specify algorithm 
    done
done


