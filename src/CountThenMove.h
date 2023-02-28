#ifndef COUNT_THEN_MOVE_H
#define COUNT_THEN_MOVE_H

#include <pthread.h>
#include <unistd.h>

#include "Buffer.h"
#include "Payload.h"


typedef struct {
    int id;
    int start;
    int end;
    int** partition_counters;
    const Tuple *input;
    Buffer* output_buffer;
    int b;
    int partition_count;
    int* threads_still_counting;
    pthread_mutex_t* mutexCounting;
    pthread_cond_t* condCounting;
} CountArgs;


void* ctm_thread(void *args) {
    CountArgs *count_args = (CountArgs*) args;

    // First pass (count)
    for(int i = count_args->start; i < count_args->end; i++) {
        const Tuple *tuple = count_args->input + i;
        int hash_value = tuple_hash(tuple, count_args->b);
        count_args->partition_counters[count_args->id][hash_value]++;
    }

    pthread_mutex_lock(count_args->mutexCounting);
    (*count_args->threads_still_counting)--;
    pthread_mutex_unlock(count_args->mutexCounting);
    // signal that this thread is done counting
    pthread_cond_signal(count_args->condCounting);

    // synchronize with other threads
    pthread_mutex_lock(count_args->mutexCounting);
    while(*count_args->threads_still_counting > 0) {
        pthread_cond_wait(count_args->condCounting, count_args->mutexCounting);
    }
    pthread_mutex_unlock(count_args->mutexCounting);

    // calculate offsets
    int offsets[count_args->partition_count];

    for(int i = 0; i < count_args->partition_count; i++) {
        int offset = 0;

        // thread 0 has no offsets to add (therefore no iteration for id=0)
        // remaining threads' offsets are equal to the sum of the counts of the
        // previous threads for that specific partition
        for(int j = 0; j < count_args->id; j++) {
            offset += count_args->partition_counters[j][i];
        }
        offsets[i] = offset;
    }


    // Second pass (move)
    for(int i = count_args->start; i < count_args->end; i++) {
        const Tuple *tuple = count_args->input + i;
        int hash_value = tuple_hash(tuple, count_args->b);
        int offset = offsets[hash_value];

        // write to output buffer and increment offset
        count_args->output_buffer->data[offset] = tuple;
        offsets[hash_value]++;
    }
}

double count_then_move(const Tuple* input, int tuple_size, int thread_count, int hash_bits){
    int partition_count = 1 << hash_bits;

    int buffer_count = 1;
    // Computing range size of input to be assigned to each thread
    int buffer_size = (tuple_size / buffer_count) * 1.5; // 50% extra capacity
    int thread_range = tuple_size / thread_count;
    pthread_t threads[thread_count];
    Buffer* output_buffer = create_buffer(buffer_size);

    // 2^b counters per thread

    pthread_mutex_t mutexCounting;
    pthread_mutex_init(&mutexCounting, NULL);
    pthread_cond_t condCounting;
    pthread_cond_init(&condCounting, NULL);

    int** partition_counters = (int**)malloc(sizeof(int*) * thread_count);
    for (int i = 0; i < thread_count; i++) {
        partition_counters[i] = (int*)malloc(sizeof(int) * partition_count);
        for (int j = 0; j < partition_count; j++) {
            partition_counters[i][j] = 0;
        }
    }    

    CountArgs *threadArgs = (CountArgs*)malloc(sizeof(CountArgs) * thread_count);

    clock_t begin = clock();

    for(int i = 0; i < thread_count; i++) {
        CountArgs *args = threadArgs + i;
        args->id = i;
        args->start = thread_range * i;
        args->end = args->start + thread_range; // todo fix so it covers all (edge cases)   
        args->partition_counters = partition_counters;
        args->input = input; 
        args->output_buffer = output_buffer;
        args->b = hash_bits;
        args->partition_count = partition_count;
        args->threads_still_counting = &thread_count;
        args->mutexCounting = &mutexCounting;
        args->condCounting = &condCounting;

        pthread_create(&threads[i], NULL, ctm_thread, (void *)args);
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end = clock();
    double time_spent_ms = (double)(end - begin) / CLOCKS_PER_SEC * 1000.0;

    pthread_mutex_destroy(&mutexCounting);
    pthread_cond_destroy(&condCounting);
}

#endif // COUNT_THEN_MOVE_H