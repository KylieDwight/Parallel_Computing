For the Midterm Part 2, I was tasked with converting the N Queens problem from sequential to parallel
using pthreads and openMP. Based on my observation from the table, the sequential and 1 thread seem 
to be even. When increasing threads using pthreads, the time seems to be increasing. This could be due
to multiple different issues such as overhead from the creation and destroying of threads as well 
as the context switching. We can see that the time did decrease from 2 threads to 3 threads. I believe 
this is because 15 is divisible by 3, so the workload is much more balanced among the threads. OpenMP 
did very well in comparison to pthreads. Using the max threads that I can gives me 8 threads. Since 
15 is not divisible by 3, each thread took one row and the last thread took the rest of them. If I 
decrease the number of threads to 7, each thread will take 2 and the last thread will take 3, which 
actually gave me a smaller run time of 28.7 seconds. This shows that OpenMP is much faster when it 
comes to threads and also that the number of threads in comparison to N when splitting data is important 
for minimizing run time.