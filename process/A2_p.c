#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Starting shield power level
int shield_power = 50;  

int main() {
    pid_t pid;

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);

    // Create 4 child processes - 4 different characters adjusting shield power

            // Luke increases the shield power by 25
            // Han increases the shield power by 20
            // Chewbacca increases the shield power by 30
            // Leia increases the shield power by 15
    for (int i = 0; i < 4; i++) {
        pid = fork();

        // Check if process creation failed
        if (pid < 0) {
            perror("Fork Failed");
            return 1;
        }

        // Child process
        if (pid == 0) {
            if (i == 0) {
                printf("Luke: Adjusting shields...\n");
                shield_power += 25; // Luke increases the shield power by 25
                printf("Luke: Shield power level now at %d\n", shield_power);
            } else if (i == 1) {
                printf("Han: Adjusting shields...\n");
                shield_power += 20; // Han increases the shield power by 20
                printf("Han: Shield power level now at %d\n", shield_power);
            } else if (i == 2) {
                printf("Chewbacca: Adjusting shields...\n");
                shield_power += 30; // Chewbacca increases the shield power by 30
                printf("Chewbacca: Shield power level now at %d\n", shield_power);
            } else if (i == 3) {
                printf("Leia: Adjusting shields...\n");
                shield_power += 15; // Leia increases the shield power by 15
                printf("Leia: Shield power level now at %d\n", shield_power);
            }
            return 0; // Exit the child process after adjusting the shield power
        }
    }

    // Make parent process wait for all child processes to complete
    while (wait(NULL) > 0);


    // Parent process reports final state
    printf("\nFinal shield power level on the Millennium Falcon: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return 0;
}
