#include <stdio.h>
#include <math.h>

#define MAX_BODIES 100
#define G 6.67430e-11 // Gravitational constant in m^3 kg^-1 s^-2
#define AU 1.496e11   // Astronomical Unit in meters
#define MASS_SUN 1.989e30 // Mass of the Sun in kilograms

// Rescaled constants
#define TIME_SCALE sqrt(G * MASS_SUN / (AU * AU * AU)) // Time scaling factor

typedef struct {
    double x, y, z; // Position
} Vector;

void read_vector_file(const char *filename, Vector data[], int *count, double scale) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    *count = 0;
    while (fscanf(file, "%lf %lf %lf", &data[*count].x, &data[*count].y, &data[*count].z) == 3) {
        // Rescale positions by dividing by 1 AU
        data[*count].x /= scale;
        data[*count].y /= scale;
        data[*count].z /= scale;
        (*count)++;
    }

    fclose(file);
}

void read_mass_file(const char *filename, double masses[], int *count, double scale) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    *count = 0;
    while (fscanf(file, "%lf", &masses[*count]) == 1) {
        // Rescale masses by dividing by the mass of the Sun
        masses[*count] /= scale;
        (*count)++;
    }

    fclose(file);
}

Vector compute_acceleration(Vector positions[], double masses[], int num_bodies, int target) {
    Vector acceleration = {0.0, 0.0, 0.0};

    for (int i = 0; i < num_bodies; i++) {
        if (i != target) {
            double dx = positions[i].x - positions[target].x;
            double dy = positions[i].y - positions[target].y;
            double dz = positions[i].z - positions[target].z;
            double distance = sqrt(dx * dx + dy * dy + dz * dz);
            double force = masses[i] / (distance * distance * distance);

            acceleration.x += force * dx;
            acceleration.y += force * dy;
            acceleration.z += force * dz;
        }
    }

    return acceleration;
}

int main() {
    Vector positions[MAX_BODIES], velocities[MAX_BODIES], prev_positions[MAX_BODIES];
    double masses[MAX_BODIES];
    int num_bodies;

    // Read and rescale initial data from files
    read_vector_file("pos.txt", positions, &num_bodies, AU);
    read_vector_file("velo.txt", velocities, &num_bodies, AU / TIME_SCALE);
    read_mass_file("mass.txt", masses, &num_bodies, MASS_SUN);

    // Initialize previous positions for Verlet integration
    for (int i = 0; i < num_bodies; i++) {
        Vector acceleration = compute_acceleration(positions, masses, num_bodies, i);
        prev_positions[i].x = positions[i].x - velocities[i].x * TIME_SCALE + 0.5 * acceleration.x * TIME_SCALE * TIME_SCALE;
        prev_positions[i].y = positions[i].y - velocities[i].y * TIME_SCALE + 0.5 * acceleration.y * TIME_SCALE * TIME_SCALE;
        prev_positions[i].z = positions[i].z - velocities[i].z * TIME_SCALE + 0.5 * acceleration.z * TIME_SCALE * TIME_SCALE;
    }

    // Simulation loop
    for (int step = 0; step < 1000; step++) { // 1000 steps
        Vector new_positions[MAX_BODIES];

        for (int i = 0; i < num_bodies; i++) {
            Vector acceleration = compute_acceleration(positions, masses, num_bodies, i);

            // Verlet integration
            new_positions[i].x = 2 * positions[i].x - prev_positions[i].x + acceleration.x * TIME_SCALE * TIME_SCALE;
            new_positions[i].y = 2 * positions[i].y - prev_positions[i].y + acceleration.y * TIME_SCALE * TIME_SCALE;
            new_positions[i].z = 2 * positions[i].z - prev_positions[i].z + acceleration.z * TIME_SCALE * TIME_SCALE;

            // Update velocity (optional, for output purposes)
            velocities[i].x = (new_positions[i].x - prev_positions[i].x) / (2 * TIME_SCALE);
            velocities[i].y = (new_positions[i].y - prev_positions[i].y) / (2 * TIME_SCALE);
            velocities[i].z = (new_positions[i].z - prev_positions[i].z) / (2 * TIME_SCALE);
        }

        // Update positions for the next step
        for (int i = 0; i < num_bodies; i++) {
            prev_positions[i] = positions[i];
            positions[i] = new_positions[i];
        }

        // Output positions for this step
        printf("Step %d:\n", step);
        for (int i = 0; i < num_bodies; i++) {
            printf("Body %d: Position (%lf, %lf, %lf)\n", i, positions[i].x, positions[i].y, positions[i].z);
        }
    }

    return 0;
}