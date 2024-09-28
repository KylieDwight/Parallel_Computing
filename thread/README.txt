For this assignment I adjusted the array sum and matrix multiplication sequential code into parallel.
In the array sum parallel code I split the work into the number of threads so each thread would have 
a different section of numbers to sum up and store in the respective spot in the partial sums array.
At the end the partial sums are added together to get a total sum of the entire array. 
In the matrix multiplication parallel code the work is split into rows. They all adjust their respective
rows in the matrix C. 