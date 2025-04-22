#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Inicializamos los parámetros

#define N 40                // Tamaño de la red (NxN)
#define STEPS 100000        // Pasos de Monte Carlo por temperatura
#define TEMP_MIN 0        // Temperatura mínima
#define TEMP_MAX 5        // Temperatura máxima
#define TEMP_STEP 0.5     // Paso de temperatura
#define MONTECARLO_STEPS 1000 // Pasos de Monte Carlo para medir
#define MEDIDAS 1000        // Número de pasos para tomar medidas

// Definimos la red
int spins[N][N]; // Red de espines

// Generamos la red de spines, con valores 1 y -1 aleatorios y de igual probabilidad
void start_spins_rand(){
for (int i=0; i < N; i++)
    for (int j=0; j<N; j++)
        spins[i][j] = (rand() % 2) *2 -1;
}

// Otra opción para inicializar la red: la ponemos a 1 entera

/* 
void start_spins_1(){
for (int i=0; i < N; i++)
    for (int j=0; j<N; j++)
        spins[i][j] = 1;
}
*/

// Función que nos de el entero de forma periódica

int periodic(int i) {
    if (i >= N) return 0;
    if (i < 0) return N - 1;
    return i;
}

// Función que calcula el cambio de energía al cambiar el spin

int delta_energia(int spin, int x, int y) {
    int s = spins[x][y];
    int suma_entorno =  spins[periodic(x+1)][y] +
        spins[periodic(x-1)][y] +
        spins[x][periodic(y+1)] +
        spins[x][periodic(y-1)];

    return 2 * s * suma_entorno;
}

// Función que evalua la probabilidad de que el spin cambie de signo
double probabilidad(int dE, double T) {
    double p = exp(-dE / T);

    if(p <= 1){return p;} 
    else{return 1;}
}


