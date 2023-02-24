#include <assert.h>
#include <pthread.h>

#include "Buffer.c"
#include "Payload.c"


typedef struct {
    int id;
    int start;
    int end; 
    Buffer **buffers;
    int buffer_start;
    int buffer_end;
    Tuple **input;
    int b; 
} Args;


void *process(void *args) {
    Args *process_args = (Args*) args; 

    for(int i = process_args->start; i < process_args->end; i++) {
        Tuple *tuple = process_args->input[i];
        int hash_value = hash(tuple, process_args->b);
        int index = process_args->buffer_start + hash_value;
        insert(process_args->buffers[index], tuple); 
    }
    return NULL;
}


int main(int argc, char** argv) {

    assert(argc == 4 && "Expected 3 arguments: <algorithm id> <num. threads> <num. hash bits>");
    
    int algorithmId = atoi(argv[1]);
    assert((algorithmId == 1 || algorithmId == 2) && "Invalid algorithm id (1 = Independent out, 2 = Count-Then-Move)")
    
    int numThreads = atoi(argv[2]);
    assert(numThreads > 0);
    
    int numHashBits = atoi(argv[3]);
    assert(numHashBits > 1);

    /* VARIABLES */ 
    int N = 1 << 24;
    int t = 8;
    int b = 4;
    int partition_count = 1 << b;
    int buffer_count = t * partition_count;
    int buffer_size = (N / buffer_count) * 1.5; // 50% extra capacity
    int thread_range = N / t; 
    int buffer_range = (buffer_count / t); 
    Buffer **buffers = (Buffer**)malloc(sizeof(Buffer*) * buffer_count);
    Tuple **input = (Tuple**)malloc(sizeof(Tuple*) * N);
    pthread_t threads[t];

    /* SETUP BUFFERS*/
    for(int i = 0; i < buffer_count; i++) {
        buffers[i] = create_buffer(buffer_size);
    }

    /* GENERARTE INPUT */
    for(int i = 0;  i< N; i++) {
        input[i] = create_tuple(); 
    }

    // todo start time

    /* RUN THREADS */
    for(int i = 0; i < t; i++) {
        Args *args = (Args*)malloc(sizeof(Args)); 
        args->id = i;
        args->start = thread_range * args->id; 
        args->end = args->start + thread_range; // todo fix so it covers all (edge cases) 
        args->buffers = buffers;
        args->buffer_start = buffer_range * i;
        args->buffer_end = args->buffer_start + buffer_range;
        args->input = input;
        args->b = b; 
        pthread_create(&threads[i], NULL, process, (void *)args);
    }

    /* WAIT FOR THREADS TO FINISH*/
    for(int i = 0; i<t; i++) {
        pthread_join(threads[i], NULL);
    }

    // todo stop time

    /* Inspect buffers */
    printf("Check buffers\n");
    for(int i =0; i < buffer_count; i++) {
        printf("Buffer-%03d: ", i);
        print_buffer_range(buffers[i], 3);
    }

    return 0;
}