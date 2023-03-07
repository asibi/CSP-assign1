#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdlib.h>

#include "MTwister.h"


typedef struct {
    unsigned long key;
    long payload;
} Tuple;


void tuple_initialize(Tuple* tuple, MTRand* mersenne_twister) {
    tuple->key = genRandLong(mersenne_twister);
    
    tuple->payload = tuple->key;
}


int tuple_hash(const Tuple* tuple, int b) {
    unsigned long mask = (1 << b) - 1; 
    return (int)(tuple->key & mask); 
}


#endif // PAYLOAD_H