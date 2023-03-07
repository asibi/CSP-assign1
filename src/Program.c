#include <assert.h>

#include "MTwister.h"
#include "Buffer.h"
#include "Payload.h"
#include "IndependentOut.h"
#include "CountThenMove.h"


const int NUM_TUPLES = 1 << 24;


int main(int argc, char** argv) {

    assert(argc == 4 && "Expected 3 arguments: <algorithm id> <num. threads> <num. hash bits>");
    
    int algorithm_id = atoi(argv[1]);
    assert((algorithm_id == 1 || algorithm_id == 2) && "Invalid algorithm id (1 = Independent out, 2 = Count-Then-Move)");
    
    int num_threads = atoi(argv[2]);
    assert(num_threads > 0);
    
    int num_hash_bits = atoi(argv[3]);
    assert(num_hash_bits > 0);

    // Consistent seed so we get the same random values every run
    MTRand mersenne_twister = seedRand(1337);

    Tuple* tuples = (Tuple*)malloc(sizeof(Tuple) * NUM_TUPLES);
    for (int i = 0; i < NUM_TUPLES; i++)
    {
        Tuple* tuple = tuples + i;
        tuple_initialize(tuple, &mersenne_twister);
    }

    double time_spent_ms = 0.0;

    switch (algorithm_id)
    {
        case 1:
            time_spent_ms = independent_out(tuples, NUM_TUPLES, num_threads, num_hash_bits);
            break;

        case 2:
            time_spent_ms = count_then_move(tuples, NUM_TUPLES, num_threads, num_hash_bits);
            break;

        default:       
            assert(0 && "Unknown algorithm id");
    }

    printf("%.3f\n", time_spent_ms);
    
    return 0;
}