#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 40                // Tamaño de la red (NxN)
#define STEPS 100000        // Pasos de Monte Carlo por temperatura
#define TEMP_MIN 1.5
#define TEMP_MAX 3.5
#define TEMP_STEP 0.5
#define MEDIDAS 1000        // Número de pasos para tomar medidas

int spins[N][N];

// Inicializa la red aleatoriamente con -1 o +1
void init_spins() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            spins[i][j] = (rand() % 2) * 2 - 1;
}

// Condiciones periódicas
int periodic(int i) {
    if (i >= N) return 0;
    if (i < 0) return N - 1;
    return i;
}

// Cambio de energía al voltear un espín
int delta_energy(int x, int y) {
    int s = spins[x][y];
    int sum_neighbors =
        spins[periodic(x+1)][y] +
        spins[periodic(x-1)][y] +
        spins[x][periodic(y+1)] +
        spins[x][periodic(y-1)];
    return 2 * s * sum_neighbors;
}

// Paso Monte Carlo
void monte_carlo_step(double T) {
    for (int i = 0; i < N * N; i++) {
        int x = rand() % N;
        int y = rand() % N;
        int dE = delta_energy(x, y);
        if (dE <= 0 || (rand() / (double)RAND_MAX) < exp(-dE / T))
            spins[x][y] *= -1;
    }
}

// Energía total
double total_energy() {
    double E = 0.0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            E += -spins[i][j] * (
                spins[periodic(i+1)][j] +
                spins[i][periodic(j+1)]
            );
    return E;
}

// Magnetización total
double magnetization() {
    double M = 0.0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M += spins[i][j];
    return fabs(M) / (N * N); // valor absoluto de magnetización por espín
}

void save_spins_to_file(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Error al guardar red");
        return;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(f, "%d ", spins[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main() {
    srand(time(NULL));

    FILE *data = fopen("ising_data.txt", "w");
    FILE *spins_all = fopen("spins_all_temps.txt", "w");

    if (!data || !spins_all) {
        perror("Error al abrir archivos");
        return 1;
    }

    fprintf(data, "#T\tEprom\tMprom\n");

    for (double T = TEMP_MIN; T <= TEMP_MAX + 1e-6; T += TEMP_STEP) {
        printf("Simulando T = %.2f...\n", T);
        init_spins();

        // Equilibrar el sistema
        for (int i = 0; i < 5000; i++)
            monte_carlo_step(T);

        // Medir
        double Etot = 0.0, Mtot = 0.0;
        for (int i = 0; i < MEDIDAS; i++) {
            monte_carlo_step(T);
            Etot += total_energy();
            Mtot += magnetization();
        }

        double Eprom = Etot / MEDIDAS / (N * N);
        double Mprom = Mtot / MEDIDAS;

        // Guardar medidas
        fprintf(data, "%.2f\t%.5f\t%.5f\n", T, Eprom, Mprom);
        printf("T=%.2f\tE=%.5f\tM=%.5f\n", T, Eprom, Mprom);

        // Guardar espines en el mismo archivo
        fprintf(spins_all, "# T = %.2f\n", T);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                fprintf(spins_all, "%d ", spins[i][j]);
            }
            fprintf(spins_all, "\n");
        }
        fprintf(spins_all, "\n"); // Separador entre temperaturas
    }

    fclose(data);
    fclose(spins_all);
    printf("¡Todo guardado en ising_data.txt y spins_all_temps.txt!\n");

    return 0;
}
