#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

int main() {

    omp_set_num_threads(5);
    int N = 1003;

    // Dynamic allocation of matrices
    int **A = (int **)malloc(N * sizeof(int *));
    int **B = (int **)malloc(N * sizeof(int *));
    int **C = (int **)malloc(N * sizeof(int *));

    if (!A || !B || !C) {
        printf("Memory allocation failed!\n");
        return -1;
    }

    for (int i = 0; i < N; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
        C[i] = (int *)malloc(N * sizeof(int));

        if (!A[i] || !B[i] || !C[i]) {
            printf("Memory allocation failed!\n");
            return -1;
        }
    }

    printf("Matrices allocated successfully.\n");

    // Initialize matrices A and B
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    printf("Matrices initialized successfully.\n");

    // Perform matrix multiplication
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0; // Initialize the result cell
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j]; // Multiply and accumulate
            }
        }
    }

    printf("Matrix multiplication complete!\n");

    /* Display the resulting matrix C (optional)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }*/

    // Free allocated memory
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < N; ++i) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }

    free(A);
    free(B);
    free(C);

    return 0;
}