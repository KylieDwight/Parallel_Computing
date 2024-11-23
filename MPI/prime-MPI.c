#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <mpi.h>


void sieveSegment(int start, int end, bool *is_prime, int sqrt_n, bool *base_primes) {
    for (int p = 2; p <= sqrt_n; p++) {
        if (base_primes[p]) {
            int first = (start + p - 1) / p * p;
            if (first < p * p) first = p * p;
            for (int i = first; i <= end; i += p) {
                is_prime[i - start] = false;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 10000000;

    int sqrt_n = (int)sqrt(n);
    bool *base_primes = NULL;


    if (rank == 0) {
        base_primes = (bool *)malloc((sqrt_n + 1) * sizeof(bool));
        for (int i = 0; i <= sqrt_n; i++) base_primes[i] = true;

        base_primes[0] = base_primes[1] = false;
        for (int p = 2; p * p <= sqrt_n; p++) {
            if (base_primes[p]) {
                for (int i = p * p; i <= sqrt_n; i += p) {
                    base_primes[i] = false;
                }
            }
        }
    }


    if (rank != 0) {
        base_primes = (bool *)malloc((sqrt_n + 1) * sizeof(bool));
    }
    MPI_Bcast(base_primes, sqrt_n + 1, MPI_C_BOOL, 0, MPI_COMM_WORLD);


    int low = 2 + rank * ((n - 1) / size + 1);
    int high = 1 + (rank + 1) * ((n - 1) / size + 1);
    if (high > n) high = n;

    int range_size = high - low + 1;
    bool *is_prime = (bool *)malloc(range_size * sizeof(bool));
    for (int i = 0; i < range_size; i++) is_prime[i] = true;


    sieveSegment(low, high, is_prime, sqrt_n, base_primes);

    if (rank == 0) {
        printf("Prime numbers up to %d are:\n", n);
    }

    for (int i = 0; i < range_size; i++) {
        if (is_prime[i]) {
            printf("%d ", low + i);
        }
    }

    free(is_prime);
    free(base_primes);
    MPI_Finalize();
    return 0;
}