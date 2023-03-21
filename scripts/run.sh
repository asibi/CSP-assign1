#!/bin/bash

set -e

# Compile program 
gcc -pthread src/Program.c src/MTwister.c -O3 -o program

rm -rf results
mkdir -p results

# Setup output file
echo "algorithm, thread, bits, time, cache-misses, itlb-misses, dtlb-misses, cycles, instructions, L1-icache-load-misses, L1-dcache-load-misses, stalled-cycles-frontend, stalled-cycles-backend" > results/data.txt

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
                time=$(perf stat -e cycles,instructions,L1-icache-load-misses,L1-dcache-load-misses,cache-misses,iTLB-load-misses,dTLB-load-misses,stalled-cycles-frontend,stalled-cycles-backend ./program $algorithm $t $b 2> perf_tmp.txt) 
                cycles=$(grep '  cycles' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                instructions=$(grep '  instructions' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                l1i_misses=$(grep '  L1-icache-load-misses' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                l1d_misses=$(grep '  L1-dcache-load-misses' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                cache_misses=$(grep '  cache-misses' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                itlb_misses=$(grep '  iTLB-load-misses' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                dtlb_misses=$(grep '  dTLB-load-misses' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                stalled_frontend_cycles=$(grep '  stalled-cycles-frontend' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                stalled_backend_cycles=$(grep '  stalled-cycles-backend' perf_tmp.txt | sed -E 's/\s+([0-9,]+).*$/\1/g' | sed -E 's/,//g')
                echo "$algorithm, $t, $b, $time, $cache_misses, $itlb_misses, $dtlb_misses, $cycles, $instructions, $l1i_misses, $l1d_misses, $stalled_frontend_cycles, $stalled_backend_cycles" >> results/data.txt
                rm -f perf_tmp.txt
            done
        done
    done
done


