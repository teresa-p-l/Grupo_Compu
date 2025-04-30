#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 40                
#define pasos 100000
#define temperatura_minima 0
#define temperatura_maxima 5
#define temperatura_paso 0.5
#define medidas 1000

int espines[N][N];

void inicializar_espines(int espines[N][N]) 
{
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            espines[i][j] = (rand() % 2)*2-1;
        }
    }
}

int condiciones_periodicas(int i)
{
    if (i>=N) return 0;
    if (i<0) return N-1;
    return i;
}

int delta_energia(int i, int j)
{
    int s = espines[i][j];
    int suma=
        espines[condiciones_periodicas(i+1)][j] +
        espines[condiciones_periodicas(i-1)][j] +
        espines[i][condiciones_periodicas(j+1)] +
        espines[i][condiciones_periodicas(j-1)];

    return 2*s*suma;   
}

double probabilidad(int dE, double T)
{
    if (exp(-dE/T >= 1)) return 1.0;
    else return exp(-dE/T);
}

void monte_carlo(double T)
{
    for(int i=0; i<N*N; i++)
    {
        int x = rand() % N;
        int y = rand() % N;
        int dE = delta_energia(x, y);
        if(rand()/(double)RAND_MAX < probabilidad(dE,T))
        {
            espines[x][y] *= -1;  
        }
    }
}

double energia_total()
{
    double E=0.0;
    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            E += espines[i][j] * (espines[i][condiciones_periodicas(j+1)] + 
            espines[i][condiciones_periodicas(j-1)]) +
            espines[condiciones_periodicas(i+1)][j] +
            espines[condiciones_periodicas(i-1)][j];
        }
    }
    return E;
}

void savefile(const char* file)
{
    FILE* f=fopen(file, "w");
    if(!f)
    {
        perror("Error al guardar red");
        return;
    }

    for(int i=0; i<N; i++)
    {
        for(int j=0; j<N; j++)
        {
            fprintf(f, "%d", espines[i][j]);
        }
    }
    fclose(f);
}

int main()
{
    srand(time(NULL));

    inicializar_espines(espines);
    savefile("espines.txt");
    printf("Red inicial guardada en espines.txt\n");

    FILE* resultados = fopen("resultados.txt", "w");
    if (!resultados)
    {
        perror("Error al abrir el archivo de resultados");
        return 1;
    }

    for (double T = temperatura_minima; T <= temperatura_maxima; T += temperatura_paso)
    {
        double energia_promedio = 0.0;

        for (int paso = 0; paso < pasos; paso++)
        {
            monte_carlo(T);

            if (paso % (pasos / medidas) == 0)
            {
                energia_promedio += energia_total();
            }
        }

        energia_promedio /= medidas;
        fprintf(resultados, "Temperatura: %.2f, Energia promedio: %.5f\n", T, energia_promedio);
        printf("Temperatura: %.2f, Energia promedio: %.5f\n", T, energia_promedio);
    }

    fclose(resultados);
    printf("Resultados guardados en resultados.txt\n");

    return 0;
}