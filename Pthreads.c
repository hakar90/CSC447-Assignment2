#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUCKET_COUNT 10
#define THREAD_COUNT 5
#define DATA_SIZE 1000

int data[DATA_SIZE];
int segment_length = DATA_SIZE / THREAD_COUNT;

typedef struct {
    int* container;
    int size;
} Bucket;

Bucket buckets[BUCKET_COUNT];

void* sort_container(void* arg) {
    Bucket* bucket = (Bucket*)arg;
    int i, j, element;
    for (i = 1; i < bucket->size; i++) {
        element = bucket->container[i];
        j = i - 1;

        while (j >= 0 && bucket->container[j] > element) {
            bucket->container[j + 1] = bucket->container[j];
            j = j - 1;
        }
        bucket->container[j + 1] = element;
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int i, j;
    struct timespec start, end;
    double compute_time, comm_time, total_time;

    srand(time(NULL));
    for (i = 0; i < DATA_SIZE; i++) {
        data[i] = rand() % 1000;
    }

    for (i = 0; i < BUCKET_COUNT; i++) {
        buckets[i].container = (int*)malloc((DATA_SIZE / BUCKET_COUNT) * sizeof(int));
        buckets[i].size = 0;
    }

    for (i = 0; i < DATA_SIZE; i++) {
        int index = i % BUCKET_COUNT;
        buckets[index].container[buckets[index].size++] = data[i];
    }

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, sort_container, (void*)&buckets[i]);
    }

    for (i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    int idx = 0;
    for (i = 0; i < BUCKET_COUNT; i++) {
        for (j = 0; j < buckets[i].size; j++) {
            data[idx++] = buckets[i].container[j];
        }
        free(buckets[i].container);
    }

    compute_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    comm_time = compute_time;
    total_time = compute_time + comm_time;

    printf("Total Time: %.6f seconds\n", total_time);
    printf("Compute Time: %.6f seconds\n", compute_time);
    printf("Comm Time: %.6f seconds\n", comm_time);

    return 0;
}
