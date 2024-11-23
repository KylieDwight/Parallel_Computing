For this assignment, I transformed the sequential prime, integrate, and matrix multiplication programs 
into parallel using MPI. For the matrix multiplication problem, I split up the rows and columns each 
process would be working with in A, but B remained fully visible to each process. In prime, the first
process computes all of the primes up to the square root of n. It then broadcasts that to the rest
of the processes that use that to sieve out the rest of the non-prime numbers in their section. In the 
integrate problem, the work is divided among processes, but f(0) and f(N) are handled separately.
Using the reduce MPI function allows everything to come together at the end and compute the final
value.