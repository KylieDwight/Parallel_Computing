Matrix Multiplication:
My task for this assignment was to convert the sequential matrix multiplication program to parallel using
OpenMP. I moved the functions from the header file into the c file's main function to make the program 
simpler. I left the allocation of memory alone as to not cause any issues. I implemented parallel
to initialize the matrices using a static schedule because OpenMP is very easy to use to split up work 
among threads and none of the threads would interfere with each other. I did the same thing with the 
function that multiplies the matrices together and again to free the memory. Converting this sequential
program to parallel was very simple using OpenMP as it was mostly just setting the number of threads and 
adding #pragma omp parallel for schedule (static) above the for loops.

Integrate:
My task for this assignment was to convert the sequential program integrate.c to parallel using OpenMP.
To do this, I only had to add 3 lines of code. The parallel version required the omp.h library, so that
was added in the top of the program. I set the number of threads in the main function. I then used 
#pragma omp parallel for reduction(+:sum) in the trapezoidalRule function above the for loop to split
the work among threads. Reduction had to be used so that the threads would not write over each other
for the final value of sum.