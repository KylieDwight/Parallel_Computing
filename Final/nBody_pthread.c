#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define G 6.67430e-11  // Gravitational constant 
#define NUM_BODIES 1000    // Number of bodies in the system
#define DT 60*60*24     // Time step (1 day in seconds)
#define NUM_THREADS 4   // Number of threads to use

// Position, velocity, and mass of each body
typedef struct {
    double x, y;      // Position (x, y)
    double vx, vy;    // Velocity (vx, vy)
    double mass;      // Mass
} Body;

Body bodies[NUM_BODIES];   // Array of bodies
pthread_barrier_t barrier; // Barrier for synchronizing threads

// Parameters for each thread
typedef struct {
    int start;
    int end;
} ThreadParams;

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
void* update_bodies(void* params) {
    ThreadParams* p = (ThreadParams*) params;
    int start = p->start;
    int end = p->end;
    
    double fx, fy;
    
    for (int i = start; i < end; i++) {
        fx = 0.0;
        fy = 0.0;
        
        // Summation of all forces on that body
        for (int j = 0; j < NUM_BODIES; j++) {
            if (i != j) {
                double fxi, fyi;
                compute_gravitational_force(&bodies[i], &bodies[j], &fxi, &fyi);
                fx += fxi;
                fy += fyi;
            }
        }
        
        // Update the velocity of body i
        bodies[i].vx += fx / bodies[i].mass * DT;
        bodies[i].vy += fy / bodies[i].mass * DT;
    }
    
    // Wait for all threads to complete velocity updates
    pthread_barrier_wait(&barrier);
    
    // Update the positions based on the velocities
    for (int i = start; i < end; i++) {
        bodies[i].x += bodies[i].vx * DT;
        bodies[i].y += bodies[i].vy * DT;
    }
    
    return NULL;
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
    // Initializing position, velocity, and mass for each body
    for (int i = 0; i < NUM_BODIES; i++) {
        bodies[i].x = rand() % 1000000000; 
        bodies[i].y = rand() % 1000000000;  
        bodies[i].vx = (rand() % 100 - 50) * 1e3; 
        bodies[i].vy = (rand() % 100 - 50) * 1e3; 
        bodies[i].mass = (rand() % 100 + 1) * 1e24; 
    }
    
    pthread_t threads[NUM_THREADS];
    ThreadParams params[NUM_THREADS];
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    
    // Simulate for 1000 steps
    for (int step = 0; step < 1000; step++) {
        printf("Step %d:\n", step);
        print_positions(bodies, NUM_BODIES);
        
        // Create threads
        for (int t = 0; t < NUM_THREADS; t++) {
            params[t].start = t * (NUM_BODIES / NUM_THREADS);
            params[t].end = (t + 1) * (NUM_BODIES / NUM_THREADS);
            if (t == NUM_THREADS - 1) {
                params[t].end = NUM_BODIES; // Ensure last thread covers any remaining bodies
            }
            pthread_create(&threads[t], NULL, update_bodies, &params[t]);
        }
        
        // Join threads
        for (int t = 0; t < NUM_THREADS; t++) {
            pthread_join(threads[t], NULL);
        }
    }
    
    pthread_barrier_destroy(&barrier);
    return 0;
}
