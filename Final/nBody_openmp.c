#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>  // Include OpenMP header

#define G 6.67430e-11  // Gravitational constant 
#define NUM_BODIES 1000    // Number of bodies in the system
#define DT 60*60*24     // Time step (1 day in seconds)

// Position, velocity, and mass of each body
typedef struct {
    double x, y;      // Position (x, y)
    double vx, vy;    // Velocity (vx, vy)
    double mass;      // Mass
} Body;

Body bodies[NUM_BODIES];   // Array of bodies

// Compute the gravitational force between two bodies
void compute_gravitational_force(Body *b1, Body *b2, double *fx, double *fy) {
    // Calculate the distance between them
    double dx = b2->x - b1->x;
    double dy = b2->y - b1->y;
    double distance = sqrt(dx * dx + dy * dy);
    
    // Distance too small - no force calculation!
    if (distance == 0.0) {
        *fx = *fy = 0.0;
        return;
    }
    
    // Gravitational force magnitude
    double force_magnitude = G * b1->mass * b2->mass / (distance * distance);
    
    // Force components 
    *fx = force_magnitude * dx / distance;
    *fy = force_magnitude * dy / distance;
}

// Update positions and velocities of the bodies
void update_bodies(int num_bodies, double dt) {
    // Parallelize the outer loop using OpenMP
    #pragma omp parallel for shared(bodies)
    for (int i = 0; i < num_bodies; i++) {
        double fx = 0.0;
        double fy = 0.0;
        
        // Summation of all forces on that body
        for (int j = 0; j < num_bodies; j++) {
            if (i != j) {
                double fxi, fyi;
                compute_gravitational_force(&bodies[i], &bodies[j], &fxi, &fyi);
                fx += fxi;
                fy += fyi;
            }
        }
        
        // Update the velocity of body i
        #pragma omp atomic
        bodies[i].vx += fx / bodies[i].mass * dt;
        
        #pragma omp atomic
        bodies[i].vy += fy / bodies[i].mass * dt;
    }
    
    // Update the positions based on the velocities
    #pragma omp parallel for shared(bodies)
    for (int i = 0; i < num_bodies; i++) {
        bodies[i].x += bodies[i].vx * dt;
        bodies[i].y += bodies[i].vy * dt;
    }
}

// Just printing body positions here
void print_positions(Body bodies[], int num_bodies) {
    for (int i = 0; i < num_bodies; i++) {
        printf("Body %d: Position = (%.2f, %.2f), Velocity = (%.2f, %.2f)\n", 
               i, bodies[i].x, bodies[i].y, bodies[i].vx, bodies[i].vy);
    }
    printf("\n");
}

int main() {
    // Set the number of threads to 4
    omp_set_num_threads (4);

    // Initializing position, velocity, and mass for each body
    for (int i = 0; i < NUM_BODIES; i++) {
        bodies[i].x = rand() % 1000000000; 
        bodies[i].y = rand() % 1000000000;  
        bodies[i].vx = (rand() % 100 - 50) * 1e3; 
        bodies[i].vy = (rand() % 100 - 50) * 1e3; 
        bodies[i].mass = (rand() % 100 + 1) * 1e24; 
    }

    // Simulate for 1000 steps
    for (int step = 0; step < 1000; step++) {
        printf("Step %d:\n", step);
        print_positions(bodies, NUM_BODIES);
        update_bodies(NUM_BODIES, DT);
    }

    return 0;
}
