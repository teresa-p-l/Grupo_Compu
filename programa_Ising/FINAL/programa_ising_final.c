#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Inicializamos los parámetros

#define N 100              // Tamaño de la red (NxN)
#define STEPS 5      // Pasos de Monte Carlo por temperatura
#define MEDIDAS 80 //Número de pasos para tomar medidas
#define TEMPERATURA 1.0   

// Definimos la red
int spins[N][N]; // Red de espines

// Generamos la red de spines, con valores 1 y -1 aleatorios y de igual probabilidad

void start_spins_rand(){
for (int i=0; i < N; i++)
    for (int j=0; j<N; j++)
        spins[i][j] = (rand() % 2) *2 -1;
}

// Otra opción para inicializar la red: la ponemos a 1 entera

void start_spins_1(){
for (int i=0; i < N; i++)
    for (int j=0; j<N; j++)
        spins[i][j] = 1;
}


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

// Función que hace los pasos Monte Carlo para cambiar los spins

void monte_carlo_step(double T) {
    for (int i = 0; i < N * N; i++) {
        int x = rand() % N;
        int y = rand() % N;
        int dE = delta_energia(spins[x][y], x, y);
        if ((rand() / ((double)RAND_MAX + 1 )) < probabilidad(dE, T)) // Si la probabilidad es menor que un número aleatorio entre 0 y 1, cambiamos el spin 
            spins[x][y] *= -1;
    }
}


// Función que calcula la magnetización total de la red

double magnetizacion() {
    double M = 0.0;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            M += spins[i][j];
    return fabs(M) / (N * N); 
}


// Función que calcula la energía total de la red

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

// Función que guarda los spins en un archivo

void save_spin(const char* filename) {
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

// Cuerpo del programa

int main(){
    srand(time(NULL)); // Inicializamos la semilla para los números aleatorios

    FILE *data = fopen("ising_data.txt", "w");
    FILE *spins_all = fopen("spins_all_temps.txt", "w");
    FILE *grid_data = fopen("datos_red.txt", "w");

    if (!data || !spins_all) {
        perror("Error al abrir archivos");
        return 1;
    }
    
    fprintf(grid_data, "%e %d %d\n", TEMPERATURA, STEPS, MEDIDAS); // Guardar datos de la red en un archivo

    double T = TEMPERATURA; // Temperatura a la que queremos calcular

    fprintf(data, "#T\tEprom\tMprom\n");

    
    printf("Calculando T = %.2f...\n", T);

    // Inicializar la red de espines aleatoriamente
    start_spins_rand(); // o start_spins_1() para todos a 1


    // Medir
    double E_T = 0.0, M_T = 0.0;

    for (int i = 1; i < MEDIDAS; i++) {
        for (int j = 0; j < STEPS; j++) {
            monte_carlo_step(T);
                    
            // Guardar espines en el mismo archivo
            fprintf(spins_all, "# T = %.2f\n", T);
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    fprintf(spins_all, "%d ", spins[i][j]);
                }
                fprintf(spins_all, "\n");
            }
            E_T += energia_total();
            M_T += magnetizacion();
        }

        double Eprom = E_T / (2*i*N); // Energía promedio
        double Mprom = M_T / i; // Magnetización promedio
        //double Cn=((E_T*E_T)/i-Eprom*Eprom)/(N*N*T); // Calor específico

        // Guardar medidas
        fprintf(data, "%.2f\t%.5f\t%.5f\n", T, Eprom, Mprom);
        //printf("%.5f\n",Cn); // Calor específico


            
       
        E_T = 0.0; // Reiniciar energía total
        M_T = 0.0; // Reiniciar magnetización total
        
       
     fprintf(spins_all, "\n"); // Separador entre temperaturas

    }
    salir:

    fclose(data);
    fclose(spins_all);
    fclose(grid_data);
    printf("¡Todo guardado en ising_data.txt y spins_all_temps.txt!\n");
    return 0;





}