#ifndef PAYLOAD
#define PAYLOAD

#include <stdlib.h>

typedef struct {
    long key;
    long payload;
} Tuple;

long generate_key() {
    return ((long)rand() << 32 | (long)rand()); // todo find better way
}

long generate_payload() {
    return rand() % 1000; // todo find better way
}

Tuple *create_tuple() {
    Tuple *tuple = (Tuple*)malloc(sizeof(Tuple));
    tuple->key = generate_key();
    tuple->payload = generate_payload();
    return tuple;
}

int hash(Tuple *tuple, int b) {
    int mask = (1 << b) - 1; 
    return tuple->key & mask; 
}

void print_tuple(Tuple *tuple) {
    printf("(%019lu, %4lu) ", tuple->key, tuple->payload);
}

#endif