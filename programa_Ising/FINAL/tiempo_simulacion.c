/*

ESTE PROGRAMA SIRVE PARA VER EL TIEMPO QUE TARDA EN HACER MONTE CARLO PARA DISTINTOS N

NO HACE LOS PASOS DE MONTECARLO SOLO, TAMBIÉN LAS MEDIDAS

ASÍ QUE HACE #medidas de #steps de montecarlo para cada N

LO EXPORTA AL ARCHIVO time.txt

LOS DATOS SE PRESENTAN DE ESTA MANERA:

N_1 tiempo_1
N_2 tiempo_2
N_3 tiempo_3
...
*/




#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define STEPS 50        // Pasos de Monte Carlo por temperatura
#define MEDIDAS 50      // Número de pasos para tomar medidas
#define Temp 2.3        // Temperatura de la simulación solo para el caso de 1 temperatura
#define limit 40      // Límite superior para N

// Función periódica para índices
int periodic(int i, int N) {
    if (i >= N) return 0;
    if (i < 0) return N - 1;
    return i;
}

// Calcular el cambio de energía al voltear un spin
int delta_energia(int** spins, int x, int y, int N) {
    int s = spins[x][y];
    int suma_entorno = spins[periodic(x + 1, N)][y] +
                       spins[periodic(x - 1, N)][y] +
                       spins[x][periodic(y + 1, N)] +
                       spins[x][periodic(y - 1, N)];
    return 2 * s * suma_entorno;
}

// Calcular la probabilidad de voltear un spin
double probabilidad(int dE, double T) {
    double p = exp(-1.0 * dE / T);
    return (p <= 1) ? p : 1;
}

// Realizar un paso de Monte Carlo
void montecarlo_step(int** spins, int N, double T) {
    for (int i = 0; i < N * N; i++) {
        int x = rand() % N;
        int y = rand() % N;
        int dE = delta_energia(spins, x, y, N);
        if ((1.0 * rand() / RAND_MAX) < probabilidad(dE, T)) {
            spins[x][y] *= -1;
        }
    }
}


// Inicializar los spins aleatoriamente con -1 y +1
void start_spins_rand(int** spins, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            spins[i][j] = (rand() % 2) * 2 - 1;  // -1 o +1
        }
    }
}

// Crear la matriz de spins dinámicamente
int** create_spins(int N) {
    int** spins = malloc(N * sizeof(int*));
    for (int i = 0; i < N; i++) {
        spins[i] = malloc(N * sizeof(int));
    }
    return spins;
}

// Liberar la memoria de la matriz de spins
void free_spins(int** spins, int N) {
    for (int i = 0; i < N; i++) {
        free(spins[i]);
    }
    free(spins);
}


int main(void) {
    srand(time(NULL));  // Inicializar la semilla para números aleatorios

    FILE* timefile = fopen("time.txt", "w");
    if (!timefile) {
        perror("Error al abrir el archivo time.txt");
        return 1;
    }


    for (int N = 20; N <= limit; N++) {
        // Crear la matriz de spins dinámicamente
        int** spins = create_spins(N);

        // Inicializar los spins aleatoriamente
        start_spins_rand(spins, N);

        // Medir el tiempo de ejecución
        clock_t start = clock();
        for (int j = 0; j < MEDIDAS; j++) {
            for (int step = 0; step < STEPS; step++) {
                montecarlo_step(spins, N, Temp);
            }
        }
        clock_t end = clock();
        double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

        // Guardar el tiempo en el archivo
        fprintf(timefile, "%d %f\n", N, cpu_time_used);

        // Liberar la memoria de la matriz de spins
        free_spins(spins, N);
    }

    fclose(timefile);
    return 0;
}