#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>
#include <unistd.h>

bool isSafe(int board[], int row, int col, int n) {
    for (int i = 0; i < col; i++) {
        if (board[i] == row || abs(board[i] - row) == abs(i - col)) {
            return false;
        }
    }
    return true;
}

void solveNQueensUtil(int board[], int col, int n) {
    if (col >= n) {
        #pragma omp critical
        {
            for (int i = 0; i < n; i++) {
                printf("%d ", board[i]);
            }
            printf("\n");
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        if (isSafe(board, i, col, n)) {
            board[col] = i;
            solveNQueensUtil(board, col + 1, n);
            board[col] = -1; // backtrack
        }
    }
}

int main() {
    int n = 15; // Size of the board
    omp_set_num_threads(7);
    int num_threads = 7;

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int chunk_size = n / num_threads;
        int start = thread_id * chunk_size;
        int end = start + chunk_size;
        if (thread_id == num_threads - 1) {
            end = n;
        }

        int* board = (int*)malloc(n * sizeof(int));

        // Initialize board
        for (int j = 0; j < n; j++) {
            board[j] = -1;
        }

        // Each thread works on its range of rows
        for (int i = start; i < end; i++) {
            board[0] = i;  // Place the queen in the first column
            solveNQueensUtil(board, 1, n);  // Solve for the rest of the board
            board[0] = -1;  // Backtrack
        }

        free(board);
    }

    return 0;
}