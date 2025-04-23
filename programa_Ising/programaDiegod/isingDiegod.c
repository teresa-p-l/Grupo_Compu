/*
Programa para hacer el algoritmo de ising en C
El programa se basa en el algoritmo de Metropolis para simular el modelo de Ising en 2D.

*/

#include <stdio.h>
#include <stdlib.h>

#define N 20                // Tamaño de la red (NxN)
#define STEPS 100        // Pasos de Monte Carlo por temperatura
#define TEMP_MIN 0        // Temperatura mínima
#define TEMP_MAX 5        // Temperatura máxima
#define TEMP_STEP 0.5     // Paso de temperatura
#define MEDIDAS 10000        // Número de pasos para tomar medidas

// Definimos la red
int spins[N][N]; // Red de espines

//First we initialize the spins randomly with -1 and +1
void start_spins_rand(){
    for (int i=0; i < N; i++)
        for (int j=0; j<N; j++)
            spins[i][j] = (rand() % 2) *2 -1;
            //This makes a random number between 0 and 1, and then we multiply it by 2 and subtract 1, so we get -1 or +1 with equal probability.
}

//Another option for initializing the network: we put it all to 1
void start_spins_1(){
    for (int i=0; i < N; i++)
        for (int j=0; j<N; j++)
            spins[i][j] = 1;
}
//This will be important later when we want to see the effect of the temperature on the system.

//Now we define the function that gives us the periodic integer, so we can use it in the rest of the program.
int periodic(int i) {
    if (i >= N) return 0;
    if (i < 0) return N - 1;
    return i;
}

//This function gives us the periodic integer, so we can use it in the rest of the program.
//Now we get the energy change when we flip a spin.

int delta_energia(int spin, int x, int y) 
{
    int s = spins[x][y];
    //We get the sum of the spins in the neighborhood of the spin we are going to flip.
    //We use the periodic function to get the periodic integer.
    int suma_entorno =  spins[periodic(x+1)][y] +
        spins[periodic(x-1)][y] +
        spins[x][periodic(y+1)] +
        spins[x][periodic(y-1)];
    return 2 * s * suma_entorno;
}

//Now we generate a random number from 0 to 1. If the random number is less than the probability, we flip the spin.
//If the random number is greater than the probability, we don't flip the spin.

double probabilidad(int dE, double T) {
    double p = exp(-dE / T);
    if(p <= 1){return p;} 
    else{return 1;}
}

//We will the montecarlo step function to flip the spins. We will use the delta_energia function to get the energy change when we flip a spin.
//We will use the probabilidad function to get the probability of flipping the spin.

void montecarlo_step(double T) {
    for (int i = 0; i < N * N; i++) {
        int x = rand() % N;
        int y = rand() % N;
        int dE = delta_energia(spins[x][y], x, y);
        if (( rand() / RAND_MAX) < probabilidad(dE, T)) // If the probability is less than a random number between 0 and 1, we flip the spin 
            spins[x][y] *= -1;
    }
}

//Now we define the function that calculates the total energy

double energia_total() {
    double E = 0.0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            E += spins[i][j] * (
                spins[i][periodic(j+1)] +
                spins[i][periodic(j-1)] +
                spins[periodic(i+1)][j] +
                spins[periodic(i-1)][j]);
    return -E * 0.5;
}



//Now let us save the data. We will save the data in a file called ising_data.txt
//We can also open a file that is called spins_all_temps.txt, where we will save the spins for each temperature.

//Now for the program itself.

void savespin(char* file)
{
    FILE* f = fopen(file, "w");
    //We check if it opens correctly
    if (!f) {
        perror("Error al guardar red");
        return;
    }

    //We write the spins in the file
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(f, "%d ", spins[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

int main(void)
{
    srand(time(NULL)); // Inicializamos la semilla para los números aleatorios

    FILE *data = fopen("ising_data.txt", "w");
    FILE *spins_all = fopen("spins_all_temps.txt", "w");

    if (!data || !spins_all) {
        perror("Error al abrir archivos");
        return 1;
    }

    //We initialize the spins randomly
    start_spins_rand();

    //We loop over the temperatures
    for (double T = TEMP_MIN; T <= TEMP_MAX; T += TEMP_STEP) {
        //We loop over the steps
        for (int step = 0; step < STEPS; step++) {
            montecarlo_step(T);
        }
        //We stabilize the system
        

        //We calculate the energy
        double E = energia_total();
        fprintf(data, "%f %f\n", T, E);
        savespin(spins_all);
    }
    fclose(data);
    fclose(spins_all);

    return 0;
}



