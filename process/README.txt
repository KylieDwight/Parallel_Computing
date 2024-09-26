For assignment 2 part 1, I created a program with 4 child processes that print out their names, adjust the 
shield power, and then print the new shield power.
I used a for loop and the fork() function to create all 4 children and added in a check to make sure it did not fail
From there, I used the incrementor in the for loop to determine which child was which, printed their correct outputs,
and adjusted the shield accordingly
I used wait() to ensure the parent function waited until all of the children were done
Then the final output printed.