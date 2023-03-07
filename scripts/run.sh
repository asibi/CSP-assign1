#!/bin/bash

set -e

# Compile program 
gcc -pthread src/Program.c src/MTwister.c -O3 -o program

rm -rf results
mkdir -p results

# Setup output file
echo "algorithm, thread, bits, time, cache-misses, itlb-misses, dtlb-misses" > results/data.txt

# Iterate over threads (t) and hash bits (b)
for t in 1 2 4 8 16 32
do
    for b in {1..18}    
    do
        for runs in {1..8} # Run each configuration 8 times
        do
            # 1 = independen out, 2 = count-and-move 
            for algorithm in 1 2
            do
                echo "Running: a=$algorithm t=$t b=$b"
                time=$(perf stat -e cache-misses,iTLB-load-misses,dTLB-load-misses ./program $algorithm $t $b 2> perf_tmp.txt) 
                cache_misses=$(grep cache-misses perf_tmp.txt | sed -E 's/\s+([0-9]+).*$/\1/g')
                itlb_misses=$(grep iTLB-load-misses perf_tmp.txt | sed -E 's/\s+([0-9]+).*$/\1/g')
                dtlb_misses=$(grep dTLB-load-misses perf_tmp.txt | sed -E 's/\s+([0-9]+).*$/\1/g')
                echo "$algorithm, $t, $b, $time, $cache_misses, $itlb_misses, $dtlb_misses" >> results/data.txt
                rm -f perf_tmp.txt
            done
        done
    done
done


