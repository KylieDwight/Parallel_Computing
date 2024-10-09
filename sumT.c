#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 13
#define NUM_THREADS 4

long long arr[SIZE];
long long totalSum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* sumPart(void* arg) {
    int thread_id = *(int*)arg;
    int chunk_size = SIZE / NUM_THREADS;
    int start = thread_id * chunk_size;
    int end = start + chunk_size;
    int remaining = SIZE % NUM_THREADS;
    if(thread_id == NUM_THREADS - 1){
        end = end + remaining;
    }

    long long temp;
    // Calculate partial sums
    for (int i = start; i < end; i++) {
        temp += arr[i];
    }

    pthread_mutex_lock(&mutex);
    totalSum += temp;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    // Initialize the array
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i + 1; 
    }

    // srand(time(NULL));
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    // Create threads to compute partial sums
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumPart, &thread_ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print the total sum;
    printf("Total Sum: %lld\n", totalSum);

    return 0;
}
