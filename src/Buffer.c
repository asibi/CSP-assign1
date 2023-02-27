#include <stdio.h>
#include "Payload.c"

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

void print_buffer_range(Buffer *buffer, int n) {
    if(count(buffer) == 0) {
        printf("Empty buffer\n");
        return;
    }
    for(int i = 0; i < n && i < count(buffer); i++) {
        print_tuple(buffer->data[i]);
    }
    printf("\n");
}

void print_buffer(Buffer *buffer) {
    print_buffer_range(buffer, buffer->size);
}

void insert(Buffer *buffer, const Tuple *item) {
    if (isFull(buffer)) {
        printf("Warning: Can not insert. Buffer is full\n");
        // print_buffer(buffer);
        return;
    }
    buffer->data[buffer->next++] = item;
}

