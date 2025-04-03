#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Structure to represent a planet
typedef struct {
    double x, y;          // Position coordinates
    double vx, vy;        // Velocity components
    double ax, ay;        // Acceleration components
    double mass;          // Mass of the planet
    char name[20];        // Name of the planet
} Planet;

// Constants
#define G 6.67430e-11    // Gravitational constant
#define DT 3600           // Time step (1 hour)
#define NUM_PLANETS 9     // Number of planets + sun
#define MAX_STEPS 8760    // Maximum simulation steps (1 year)

// Function prototypes
double distance(Planet *p1, Planet *p2);
void calculate_forces(Planet *planets, int n);
void verlet_step(Planet *planets, int n);

// Calculate distance between two planets
double distance(Planet *p1, Planet *p2) {
    double dx = p2->x - p1->x;
    double dy = p2->y - p1->y;
    return sqrt(dx*dx + dy*dy);
}

// Calculate gravitational force between two planets
void calculate_force(Planet *p1, Planet *p2) {
    double r = distance(p1, p2);
    double F = G * p1->mass * p2->mass / (r*r);
    
    double dx = p2->x - p1->x;
    double dy = p2->y - p1->y;
    
    // Calculate acceleration components
    p1->ax += F * dx / (r * p1->mass);
    p1->ay += F * dy / (r * p1->mass);
    p2->ax -= F * dx / (r * p2->mass);
    p2->ay -= F * dy / (r * p2->mass);
}

// Calculate all forces between planets
void calculate_forces(Planet *planets, int n) {
    // Reset accelerations
    for(int i = 0; i < n; i++) {
        planets[i].ax = 0;
        planets[i].ay = 0;
    }
    
    // Calculate forces between all pairs of planets
    for(int i = 0; i < n; i++) {
        for(int j = i + 1; j < n; j++) {
            calculate_force(&planets[i], &planets[j]);
        }
    }
}

// Perform Verlet integration step
void verlet_step(Planet *planets, int n) {
    // First half-step velocity update
    for(int i = 0; i < n; i++) {
        planets[i].vx += planets[i].ax * DT * 0.5;
        planets[i].vy += planets[i].ay * DT * 0.5;
    }
    
    // Position update
    for(int i = 0; i < n; i++) {
        planets[i].x += planets[i].vx * DT;
        planets[i].y += planets[i].vy * DT;
    }
    
    // Recalculate forces at new positions
    calculate_forces(planets, n);
    
    // Second half-step velocity update
    for(int i = 0; i < n; i++) {
        planets[i].vx += planets[i].ax * DT * 0.5;
        planets[i].vy += planets[i].ay * DT * 0.5;
    }

    int read_initial_conditions(const char *filename, Planet *planets) {
        FILE *file = fopen(filename, "r");
        if (!file) return 0;
        
        int count = 0;
        while(count < NUM_PLANETS && 
              fscanf(file, "%s %lf %lf %lf %lf %lf", 
                     planets[count].name,
                     &planets[count].mass,
                     &planets[count].x,
                     &planets[count].y,
                     &planets[count].vx,
                     &planets[count].vy) == 6) {
            count++;
        }
        
        fclose(file);
        return count;
    }
    
    // Save frame to PPM file
    void save_frame(const char *filename, Planet *planets, int n, 
                    double scale_factor, int width, int height) {
        FILE *file = fopen(filename, "w");
        fprintf(file, "P3\n%d %d\n255\n", width, height);
        
        for(int y = 0; y < height; y++) {
            for(int x = 0; x < width; x++) {
                int r = 255, g = 255, b = 255;
                
                // Find closest planet
                double min_dist = scale_factor * width;
                Planet *closest = NULL;
                
                for(int i = 0; i < n; i++) {
                    double px = planets[i].x * scale_factor + width/2;
                    double py = planets[i].y * scale_factor + height/2;
                    
                    double dist = sqrt(pow(px - x, 2) + pow(py - y, 2));
                    if(dist < min_dist) {
                        min_dist = dist;
                        closest = &planets[i];
                    }
                }
                
                // Color based on distance to closest planet
                if(closest) {
                    double intensity = 255 * (1 - min_dist/(scale_factor * width));
                    r = g = b = (int)intensity;
                }
                
                fprintf(file, "%d %d %d ", r, g, b);
            }
            fprintf(file, "\n");
        }
        
        fclose(file);
    }
    
    int main() {
        Planet planets[NUM_PLANETS];
        int num_planets = read_initial_conditions("initial.txt", planets);
        
        // Find maximum distance for scaling
        double max_dist = 0;
        for(int i = 0; i < num_planets; i++) {
            double dist = sqrt(pow(planets[i].x, 2) + pow(planets[i].y, 2));
            if(dist > max_dist) max_dist = dist;
        }
        
        double scale_factor = 400.0 / max_dist;  // Scale factor for visualization
        
        // Simulation loop
        for(int step = 0; step < MAX_STEPS; step++) {
            verlet_step(planets, num_planets);
            
            // Save frame every 24 steps (one frame per day)
            if(step % 24 == 0) {
                char filename[50];
                sprintf(filename, "frame_%05d.ppm", step/24);
                save_frame(filename, planets, num_planets, scale_factor, 800, 800);
            }
        }
        
        return 0;
    }
}