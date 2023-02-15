#include <stdio.h> // todo delete

#include "Payload.c"

typedef struct {
    int next; 
    int size;
    Tuple **data;
} Buffer;

Buffer *create_buffer(int size) {
    Buffer *buffer = (Buffer*)malloc(sizeof(Buffer));
    buffer->next = 0;
    buffer->size = size; 
    buffer->data = (Tuple**)malloc(size *sizeof(Tuple*));
    return buffer;
}

int count(Buffer *buffer) {
    return buffer->next;
}

int isFull(Buffer *buffer) {
    return buffer->next == buffer->size - 1;
}

int isEmpty(Buffer *buffer) {
    return buffer->next == 0;
}

void insert(Buffer *buffer, Tuple *item) {
    if (isFull(buffer)) {
        printf("Buffer is full\n");
        return;
    }
    buffer->data[buffer->next++] = item;
}

void print_buffer(Buffer *buffer) {
    for(int i = 0; i < count(buffer); i++) {
        print_tuple(buffer->data[i]);
    }
    printf("\n");
}


// todo delete
int main() {
    Buffer *mybuffer = create_buffer(100);
    printf("%d\n", isEmpty(mybuffer));
    printf("%d\n", isFull(mybuffer));

    for(int i = 0; i < 100; i++) {
        insert(mybuffer, create_tuple());
    }
    printf("%d\n", isEmpty(mybuffer));
    printf("%d\n", isFull(mybuffer));
    // print_buffer(mybuffer);
    return 0;
}