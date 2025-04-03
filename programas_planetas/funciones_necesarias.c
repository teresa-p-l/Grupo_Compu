
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


//Aquí hacemos las funciones que vayan a ir haciendo falta para el programa de los planetas

//Función que reescala mis constantes, para trabajar con datos menos pesados.
#include <math.h>

void ReescaladoSolar(double* r, double* t, double* v, double* m, int n, int d) {
    int i, k;
    double c = 1.496e11;
    double G = 6.67e-11;
    double M = 1.99e30;

    for (i = 0; i < n; i++) {
        m[i] = m[i] / M;
        for (k = 0; k < d; k++) {
            r[i * d + k] = r[i * d + k] / c;
            v[i * d + k] = v[i * d + k] * sqrt(c / (G * M));
        }
    }

    // Correctly update t
    *t = sqrt((G * M) / pow(c, 3)) * (*t);
}
