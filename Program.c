#include "Buffer.c"
#include "Payload.c"
#define N 1000 // 1 << 24 todo use this number



int main() {

    // Variables
    int t = 2;
    int b = 2;
    int partition_count = 1 << b;
    int buffer_count = t * partition_count;
    int buffer_size = (N / buffer_count) * 1.5; // 50% extra capacity
    Buffer **buffers = (Buffer**)malloc(sizeof(Buffer*) * buffer_count);

    for(int i = 0; i < buffer_count; i++) {
        buffers[i] = 
    }

    return 0;
}