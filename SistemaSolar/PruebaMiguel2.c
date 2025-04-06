#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 6.67e-11        // Constante de gravitación en N·m²/kg² (no usada directamente aquí)
#define MS 1.99e30        // Masa del Sol en kg
#define C 1.496e11        // Unidad de distancia: distancia Tierra-Sol en metros

#define N 2               // Número de cuerpos (por ejemplo, Sol y Tierra)
#define T_TOTAL 1000.0    // Tiempo total de simulación (en unidades reescaladas)
#define DT 0.01           // Paso temporal

// Estructura para cada cuerpo
typedef struct {
    double rx, ry;      // Posición reescalada: r' = r / C
    double vx, vy;      // Velocidad reescalada
    double ax, ay;      // Aceleración reescalada
    double m;           // Masa reescalada: m' = m / MS
} Body;

// Prototipos de funciones
void inicializarCuerpos(Body cuerpos[]);
void calcularAceleraciones(Body cuerpos[]);
void verletStep(Body cuerpos[], FILE *file);

int main(void) {
    Body cuerpos[N];
    int i;
    double t;

    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    // Inicializar condiciones iniciales
    inicializarCuerpos(cuerpos);
    // Calcular aceleraciones iniciales
    calcularAceleraciones(cuerpos);

    // Bucle de simulación
    for (t = 0; t < T_TOTAL; t += DT) {
        verletStep(cuerpos, file);
        fprintf(file, "%lf %lf %lf\n", t, cuerpos[1].rx, cuerpos[1].ry);
    }

    fclose(file);
    return 0;
}

// Inicializa las condiciones iniciales de los cuerpos
void inicializarCuerpos(Body cuerpos[]) {
    // Cuerpo 0: Sol (centrado en el origen)
    cuerpos[0].rx = 0.0;
    cuerpos[0].ry = 0.0;
    cuerpos[0].vx = 0.0;
    cuerpos[0].vy = 0.0;
    cuerpos[0].m  = 1.0;   // m' = 1 (masa solar)

    // Cuerpo 1: Planeta (por ejemplo, la Tierra reescalada)
    cuerpos[1].rx = 1.0;   // 1 UA = 1 en unidades reescaladas
    cuerpos[1].ry = 0.0;
    cuerpos[1].vx = 0.0;
    cuerpos[1].vy = sqrt(1.0 / fabs(cuerpos[1].rx)); // velocidad orbital circular
    cuerpos[1].m  = 3.0e-6;  // Masa reescalada de la Tierra
}

// Calcula las aceleraciones para cada cuerpo usando la ley de la gravitación reescalada
void calcularAceleraciones(Body cuerpos[]) {
    int i, j;
    double dx, dy, distCubed;

    // Inicializa las aceleraciones a cero
    for (i = 0; i < N; i++) {
        cuerpos[i].ax = 0.0;
        cuerpos[i].ay = 0.0;
    }

    // Calcula la aceleración debido a la atracción gravitatoria entre cada par de cuerpos
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i != j) {
                dx = cuerpos[i].rx - cuerpos[j].rx;
                dy = cuerpos[i].ry - cuerpos[j].ry;
                distCubed = pow(dx * dx + dy * dy, 1.5);
                if (distCubed != 0.0) {
                    cuerpos[i].ax -= cuerpos[j].m * dx / distCubed;
                    cuerpos[i].ay -= cuerpos[j].m * dy / distCubed;
                }
            }
        }
    }
}

// Realiza un paso del algoritmo de Verlet en velocidad
void verletStep(Body cuerpos[], FILE *file) {
    int i;
    double old_ax[N], old_ay[N];

    // Almacena las aceleraciones actuales
    for (i = 0; i < N; i++) {
        old_ax[i] = cuerpos[i].ax;
        old_ay[i] = cuerpos[i].ay;
    }

    // Actualiza posiciones
    for (i = 0; i < N; i++) {
        cuerpos[i].rx += cuerpos[i].vx * DT + 0.5 * cuerpos[i].ax * DT * DT;
        cuerpos[i].ry += cuerpos[i].vy * DT + 0.5 * cuerpos[i].ay * DT * DT;
    }

    // Recalcula aceleraciones
    calcularAceleraciones(cuerpos);

    // Actualiza velocidades
    for (i = 0; i < N; i++) {
        cuerpos[i].vx += 0.5 * (old_ax[i] + cuerpos[i].ax) * DT;
        cuerpos[i].vy += 0.5 * (old_ay[i] + cuerpos[i].ay) * DT;
    }
}
