#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdlib.h>


typedef struct {
    long key;
    long payload;
} Tuple;


long generate_key() {
    return ((long)rand() << 32 | (long)rand()); // todo find better way
}


void tuple_initialize(Tuple* tuple) {
    tuple->key = generate_key();
    tuple->payload = tuple->key;
}


int tuple_hash(const Tuple* tuple, int b) {
    int mask = (1 << b) - 1; 
    return tuple->key & mask; 
}


void print_tuple(const Tuple *tuple) {
    printf("(%019lu, %4lu) ", tuple->key, tuple->payload);
}


#endif // PAYLOAD_H