#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 100                
#define pasos 1000
#define temperatura_minima 0
#define temperatura_maxima 5
#define temperatura_paso 0.5
#define medidas 100000
#define temperatura 1.1 // Temperatura fija

int espines[N][N];

void inicializar_espines(int espines[N][N]) 
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            espines[i][j] = (rand() % 2) * 2 - 1;
        }
    }
}

int condiciones_periodicas(int i)
{
    if (i >= N) return 0;
    if (i < 0) return N - 1;
    return i;
}

int delta_energia(int i, int j)
{
    int s = espines[i][j];
    int suma =
        espines[condiciones_periodicas(i + 1)][j] +
        espines[condiciones_periodicas(i - 1)][j] +
        espines[i][condiciones_periodicas(j + 1)] +
        espines[i][condiciones_periodicas(j - 1)];

    return 2 * s * suma;   
}

double probabilidad(int dE, double T) {
    double p = exp(-1.0 * dE / T);
    return (p <= 1) ? p : 1;
}

void monte_carlo(double T)
{
    for (int i = 0; i < N * N; i++)
    {
        int x = rand() % N;
        int y = rand() % N;
        int dE = delta_energia(x, y);
        if (rand() / (double)RAND_MAX < probabilidad(dE, T))
        {
            espines[x][y] *= -1;  
        }
    }
}

double energia_total()
{
    double E = 0.0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            E += espines[i][j] * (espines[i][condiciones_periodicas(j + 1)] + 
                                  espines[i][condiciones_periodicas(j - 1)]) +
                 espines[condiciones_periodicas(i + 1)][j] +
                 espines[condiciones_periodicas(i - 1)][j];
        }
    }
    return E;
}

void savefile(const char* file)
{
    FILE* f = fopen(file, "w");
    if (!f)
    {
        perror("Error al guardar red");
        return;
    }

    // Escribir la temperatura en la primera línea
    fprintf(f, "# T = %.2f\n", temperatura);

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fprintf(f, "%d ", espines[i][j]); // Agregar un espacio entre los valores
        }
        fprintf(f, "\n"); // Agregar un salto de línea al final de cada fila
    }

    fclose(f);
}

int main()
{
    clock_t start_time = clock();
    srand(time(NULL));

    inicializar_espines(espines);
    savefile("espines_iniciales.txt");
    printf("Red inicial guardada en espines_iniciales.txt\n");

    FILE* resultados = fopen("resultados.txt", "w");
    if (!resultados)
    {
        perror("Error al abrir el archivo de resultados");
        return 1;
    }

    FILE* espines_file = fopen("espines.txt", "w");
    if (!espines_file)
    {
        perror("Error al abrir el archivo espines.txt");
        fclose(resultados);
        return 1;
    }

    // Escribir la temperatura en la primera línea del archivo espines.txt
    fprintf(espines_file, "# T = %.2f\n", temperatura);

    for (int paso = 0; paso < pasos; paso++)
    {
        monte_carlo(temperatura);

        // Guardar los espines en el archivo espines.txt
        fprintf(espines_file, "# Paso = %d\n", paso);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                fprintf(espines_file, "%d ", espines[i][j]);
            }
            fprintf(espines_file, "\n");
        }
        fprintf(espines_file, "\n"); // Línea en blanco para separar configuraciones
    }

    fclose(espines_file);
    fclose(resultados);
    printf("Resultados guardados en resultados.txt\n");
    printf("Espines guardados en espines.txt\n");

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %.2f segundos\n", elapsed_time);

    return 0;
}