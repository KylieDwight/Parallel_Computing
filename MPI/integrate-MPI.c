#include <stdio.h>
#include <mpi.h>

#define N 1000000000 // intervals

double f(double x) {
    return 4.0 / (1.0 + x * x); // Function to integrate
}

double trapezoidalRule(int rank, int size) {
    double h = 1.0 / N;
    double local_sum = 0.0;

    // Each process handles a portion of the intervals
    int start = rank * (N / size) + 1; // start index for this process (excluding 0)
    int end = (rank + 1) * (N / size); // end index for this process
    if (rank == size - 1) {
        end = N - 1; // The last process handles the remaining intervals (excluding 1)
    }

    // Sum the local part, excluding the first and last elements
    for (int i = start; i <= end; ++i) {
        local_sum += f(i * h);
    }

    return local_sum;
}

int main(int argc, char *argv[]) {
    int rank, size;
    double global_sum, local_sum;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Perform the local summation
    local_sum = trapezoidalRule(rank, size);

    // Reduce the local sums to get the global sum (total sum)
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // The root process (rank 0) adds the first and last terms, and multiplies by the step size
    if (rank == 0) {
        double h = 1.0 / N;
        global_sum = 0.5 * (f(0) + f(1)) + global_sum; // Add the endpoints f(0) and f(1)
        global_sum *= h; // Multiply by the step size
        printf("Estimated value of π: %f\n", global_sum);
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}