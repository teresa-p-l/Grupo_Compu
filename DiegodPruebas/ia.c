#include <stdio.h>
#include <math.h>

#define MAX_BODIES 100
#define G 6.67430e-11 // Gravitational constant in m^3 kg^-1 s^-2
#define TIME_STEP 3600 // Time step in seconds (1 hour)
#define SIMULATION_STEPS 100000 // Number of simulation steps

typedef struct {
    double x, y; // Position in 2D
} Vector;

void read_vector_file(const char *filename, Vector data[], int *count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    *count = 0;
    while (fscanf(file, "%lf %lf", &data[*count].x, &data[*count].y) == 2) {
        (*count)++;
    }

    fclose(file);
}

void read_mass_file(const char *filename, double masses[], int *count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    *count = 0;
    while (fscanf(file, "%lf", &masses[*count]) == 1) {
        (*count)++;
    }

    fclose(file);
}

Vector compute_acceleration(Vector positions[], double masses[], int num_bodies, int target) {
    Vector acceleration = {0.0, 0.0};

    for (int i = 0; i < num_bodies; i++) {
        if (i != target) {
            double dx = positions[i].x - positions[target].x;
            double dy = positions[i].y - positions[target].y;
            double distance = sqrt(dx * dx + dy * dy);
            double force = G * masses[i] / (distance * distance * distance);

            acceleration.x += force * dx;
            acceleration.y += force * dy;
        }
    }

    return acceleration;
}

double compute_kinetic_energy(Vector velocities[], double masses[], int num_bodies) {
    double kinetic_energy = 0.0;
    for (int i = 0; i < num_bodies; i++) {
        double speed_squared = velocities[i].x * velocities[i].x + velocities[i].y * velocities[i].y;
        kinetic_energy += 0.5 * masses[i] * speed_squared;
    }
    return kinetic_energy;
}

double compute_potential_energy(Vector positions[], double masses[], int num_bodies) {
    double potential_energy = 0.0;
    for (int i = 0; i < num_bodies; i++) {
        for (int j = i + 1; j < num_bodies; j++) {
            double dx = positions[j].x - positions[i].x;
            double dy = positions[j].y - positions[i].y;
            double distance = sqrt(dx * dx + dy * dy);
            potential_energy -= G * masses[i] * masses[j] / distance;
        }
    }
    return potential_energy;
}

int main() {
    Vector positions[MAX_BODIES], velocities[MAX_BODIES], prev_positions[MAX_BODIES];
    double masses[MAX_BODIES];
    int num_bodies;

    // Open output files
    FILE *output_file = fopen("output.txt", "w");
    FILE *energy_file = fopen("energy.txt", "w");
    if (output_file == NULL || energy_file == NULL) {
        printf("Error opening output files\n");
        return 1;
    }

    // Read initial data from files
    read_vector_file("pos.txt", positions, &num_bodies);
    read_vector_file("velo.txt", velocities, &num_bodies);
    read_mass_file("mass.txt", masses, &num_bodies);

    // Initialize previous positions for Verlet integration
    for (int i = 0; i < num_bodies; i++) {
        Vector acceleration = compute_acceleration(positions, masses, num_bodies, i);
        prev_positions[i].x = positions[i].x - velocities[i].x * TIME_STEP + 0.5 * acceleration.x * TIME_STEP * TIME_STEP;
        prev_positions[i].y = positions[i].y - velocities[i].y * TIME_STEP + 0.5 * acceleration.y * TIME_STEP * TIME_STEP;
    }

    // Simulation loop
    for (int step = 0; step < SIMULATION_STEPS; step++) {
        Vector new_positions[MAX_BODIES];

        for (int i = 0; i < num_bodies; i++) {
            Vector acceleration = compute_acceleration(positions, masses, num_bodies, i);

            // Verlet integration
            new_positions[i].x = 2 * positions[i].x - prev_positions[i].x + acceleration.x * TIME_STEP * TIME_STEP;
            new_positions[i].y = 2 * positions[i].y - prev_positions[i].y + acceleration.y * TIME_STEP * TIME_STEP;

            // Update velocity (optional, for output purposes)
            velocities[i].x = (new_positions[i].x - prev_positions[i].x) / (2 * TIME_STEP);
            velocities[i].y = (new_positions[i].y - prev_positions[i].y) / (2 * TIME_STEP);
        }

        // Update positions for the next step
        for (int i = 0; i < num_bodies; i++) {
            prev_positions[i] = positions[i];
            positions[i] = new_positions[i];
        }

        // Write positions to the output file
        fprintf(output_file, "Step %d:\n", step);
        for (int i = 0; i < num_bodies; i++) {
            fprintf(output_file, "Body %d: %lf %lf\n", i, positions[i].x, positions[i].y);
        }

        // Calculate and write energy to the energy file
        double kinetic_energy = compute_kinetic_energy(velocities, masses, num_bodies);
        double potential_energy = compute_potential_energy(positions, masses, num_bodies);
        double total_energy = kinetic_energy + potential_energy;
        fprintf(energy_file, "%d %lf\n", step, total_energy);
    }

    fclose(output_file);
    fclose(energy_file);
    return 0;
}