#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000  // Size of the matrix
#define NUM_THREADS 4  // Number of threads

int **A, **B, **C;  // Global matrices

// Information holder for each thread
typedef struct {
    int thread_id;
    int num_rows;  
} thread_data_t;

// Entry function for each thread
void* matrixMultiplyThread(void* arg) {
    // Extract thread info from the passes argument
    thread_data_t* data = (thread_data_t*)arg;
    int thread_id = data->thread_id;
    int num_rows = data->num_rows;


    // Calculate the start and ending row chunck for each thread to handle
    int start_row = thread_id * num_rows;
    int end_row = start_row + num_rows;

    // Loop through the start and end row assigned to the thread and compute matrix multiplication
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0; // Initialize the result cell
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j]; // Multiply and accumulate
            }
        }
    }

    pthread_exit(NULL);
}

