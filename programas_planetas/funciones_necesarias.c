
#include <stdio.h>
#include <math.h>
#include <stdlib.h>


//Aquí hacemos las funciones que vayan a ir haciendo falta para el programa de los planetas

#define G 6.67e-11      // Constante de gravitación universal en N·m²/kg²
#define Ms 1.99e30      // Masa del Sol en kg
#define c 1.496e11      // Distancia Tierra-Sol en metros

// Función para reescalar las variables
void reescalar(double r, double t, double m, double *r_scaled, double *t_scaled, double *m_scaled) {
    *r_scaled = r / c;
    *t_scaled = t * sqrt(G * Ms / (c * c * c));
    *m_scaled = m / Ms;
}

int main() {
    double r = 4.5e12;  // Ejemplo: distancia en metros
    double t = 3.154e7; // Ejemplo: tiempo en segundos (aproximadamente un año)
    double m = 5.97e24; // Ejemplo: masa en kg (aproximadamente la masa de la Tierra)

    double r_scaled, t_scaled, m_scaled;
    reescalar(r, t, m, &r_scaled, &t_scaled, &m_scaled);

    printf("Valores reescalados:\n");
    printf("r' = %e\n", r_scaled);
    printf("t' = %e\n", t_scaled);
    printf("m' = %e\n", m_scaled);

    return 0;
}