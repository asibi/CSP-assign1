#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>

#include "Tuple.h"


typedef struct {
    int next; 
    int size;
    const Tuple** data;
} Buffer;


Buffer *create_buffer(int size) {
    Buffer *buffer = (Buffer*)malloc(sizeof(Buffer));
    buffer->next = 0;
    buffer->size = size; 
    buffer->data = (const Tuple**)malloc(size *sizeof(Tuple*));
    return buffer;
}


int count(Buffer *buffer) {
    return buffer->next;
}


int isFull(Buffer *buffer) {
    return buffer->next == buffer->size;
}


int isEmpty(Buffer *buffer) {
    return buffer->next == 0;
}


void insert(Buffer *buffer, const Tuple *item) {
    if (isFull(buffer)) {
        printf("Warning: Cannot insert - buffer is full! (size=%d)\n", buffer->size);
        return;
    }
    buffer->data[buffer->next++] = item;
}

#endif // #BUFFER_H