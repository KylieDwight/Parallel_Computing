For the neuromancer problem, I implemented mutex and a condition variable to make the game fair.
Mutex protects the shared variable current player so it cannot be altered. The condition variable
makes sure that only the current player can play. Each player checks to see if it is their turn. If not, 
they wait. Once the current player's turn is over, the current player is updated and the condition variable
tells the next player it can go. The goal of this part of the assignment was to make the game fair, so
I also added in a condition to make sure that if there is a tie, all players that got the highest score
are the winners and not just the player with the smallest id of the highest scores.

For the average problem, I simply added in a barrier that prevents thread 0 from computing the total
before all threads have finished computing their partial sums. By doing this, the average is also not
computed until then and the results will be correct constantly.
