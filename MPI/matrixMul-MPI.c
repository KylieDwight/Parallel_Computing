#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // Matrix size

void initializeMatrix(int* matrix, int n, int value) {
    for (int i = 0; i < n * n; ++i) {
        matrix[i] = value;
    }
}

void displayMatrix(int* matrix, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

void matrixMultiply(int* localA, int* B, int* localC, int rows, int n) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < n; ++j) {
            localC[i * n + j] = 0;
            for (int k = 0; k < n; ++k) {
                localC[i * n + j] += localA[i * n + k] * B[k * n + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            printf("Matrix is not evenly divisible by processes");
        }
        MPI_Finalize();
        return -1;
    }

    int rowsPerProcess = N / size;
    int* A = NULL;  
    int* B = (int*)malloc(N * N * sizeof(int));  
    int* C = NULL;  
    int* localA = (int*)malloc(rowsPerProcess * N * sizeof(int));  
    int* localC = (int*)malloc(rowsPerProcess * N * sizeof(int));  

    if (rank == 0) {
        A = (int*)malloc(N * N * sizeof(int));
        C = (int*)malloc(N * N * sizeof(int));
        initializeMatrix(A, N, 1);
        initializeMatrix(B, N, 1);
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, rowsPerProcess * N, MPI_INT, localA, rowsPerProcess * N, MPI_INT, 0, MPI_COMM_WORLD);

    matrixMultiply(localA, B, localC, rowsPerProcess, N);

    MPI_Gather(localC, rowsPerProcess * N, MPI_INT, C, rowsPerProcess * N, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Matrix multiplication complete!\n");
        printf("Resulting Matrix C:\n");
        //displayMatrix(C, N);

        free(A);
        free(C);
    }

    free(B);
    free(localA);
    free(localC);

    MPI_Finalize();
    return 0;
}
