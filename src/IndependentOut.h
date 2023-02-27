#ifndef INDEPENDENT_OUT_H
#define INDEPENDENT_OUT_H

#include <stdlib.h>
#include <pthread.h>


typedef struct {
    int id;
    int start;
    int end; 
    Buffer** buffers;
    int buffer_start;
    int buffer_end;
    const Tuple* input;
    int num_hash_bits; 
} ThreadArgs;


void* thread_callback(void* args) {
    ThreadArgs *thread_args = (ThreadArgs*) args; 

    for(int i = thread_args->start; i < thread_args->end; i++) {
        const Tuple *tuple = thread_args->input + i;
        int hash_value = tuple_hash(tuple, thread_args->num_hash_bits);
        int index = thread_args->buffer_start + hash_value;
        insert(thread_args->buffers[index], tuple); 
    }
    return NULL;
}


void independent_out(const Tuple* tuples, int num_tuples, int num_threads, int num_hash_bits) {

    int partition_count = 1 << num_hash_bits;
    int buffer_count = num_threads * partition_count;
    int buffer_size = (num_tuples / buffer_count) * 1.5; // 50% extra capacity
    int tuples_per_thread = num_tuples / num_threads; 
    int buffers_per_thread = (buffer_count / num_threads); 
    Buffer **buffers = (Buffer**)malloc(sizeof(Buffer*) * buffer_count);
    pthread_t threads[num_threads];

    /* SETUP BUFFERS*/
    for(int i = 0; i < buffer_count; i++) {
        buffers[i] = create_buffer(buffer_size);
    }

    /* RUN THREADS */
    for( int i = 0; i < num_threads; i++ ) {
        ThreadArgs *args = (ThreadArgs*)malloc(sizeof(ThreadArgs)); 
        args->id = i;
        args->start = tuples_per_thread * args->id; 
        args->end = args->start + tuples_per_thread; // todo fix so it covers all (edge cases) 
        args->buffers = buffers;
        args->buffer_start = buffers_per_thread * i;
        args->buffer_end = args->buffer_start + buffers_per_thread;
        args->input = tuples;
        args->num_hash_bits = num_hash_bits; 
        pthread_create(&threads[i], NULL, thread_callback, (void *)args);
    }

    /* WAIT FOR THREADS TO FINISH*/
    for( int i = 0; i < num_threads; i++ ) {
        pthread_join(threads[i], NULL);
    }
}

#endif // INDEPENDENT_OUT_H