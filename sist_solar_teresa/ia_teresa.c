#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N_BODIES 9      // Sol + 8 planetas (se escriben solo los 8 planetas)
#define T_TOTAL 200.0  // Tiempo total de simulación en unidades reescaladas (1 unidad ~ 58.1 días)
#define DT 0.1       // Paso temporal

typedef struct {
    double rx, ry;    // Posición reescalada: r' = r / UA (1 UA)
    double vx, vy;    // Velocidad reescalada
    double ax, ay;    // Aceleración reescalada
    double m;         // Masa reescalada: m' = m / M_Solar
} Body;

// Prototipos
void initializeBodies(Body bodies[]);
void computeAccelerations(Body bodies[]);
void velocityVerletStep(Body bodies[]);

int main(void) {
    Body bodies[N_BODIES];
    int i, step, numSteps;
    FILE *fout;

    // Abrir el fichero "planetas" para escritura
    fout = fopen("planetas", "w");
    if (fout == NULL) {
        perror("Error al abrir el fichero de salida");
        return EXIT_FAILURE;
    }

    initializeBodies(bodies);
    computeAccelerations(bodies);
    numSteps = (int)(T_TOTAL / DT);

    // Bucle de simulación: en cada instante se guardan las posiciones de los planetas (excluyendo el Sol)
    for(step = 0; step <= numSteps; step++) {
        // Para cada planeta (índices 1..N_BODIES-1)
        for(i = 1; i < N_BODIES; i++) {
            // Escribe "xi_j, yi_j" seguido de salto de línea
            fprintf(fout, "%lf, %lf\n", bodies[i].rx, bodies[i].ry);
        }
        // Línea en blanco para separar los datos de cada instante
        fprintf(fout, "\n");
        
        // Actualiza usando el algoritmo de Verlet en velocidad
        velocityVerletStep(bodies);
    }

    fclose(fout);
    printf("Simulación finalizada. Los datos se han guardado en el fichero 'planetas'.\n");
    
    return 0;
}

void initializeBodies(Body bodies[]) {
    // Inicialización de condiciones reescaladas
    // Cuerpo 0: Sol (no se escribe en el fichero)
    bodies[0].rx = 0.0;
    bodies[0].ry = 0.0;
    bodies[0].vx = 0.0;
    bodies[0].vy = 0.0;
    bodies[0].m  = 1.0;
    
    // Cuerpo 1: Mercurio (r ≈ 0.39 UA)
    bodies[1].rx = 0.39;
    bodies[1].ry = 0.0;
    bodies[1].vx = 0.0;
    bodies[1].vy = sqrt(1.0 / 0.39);
    bodies[1].m  = 1.66e-7;
    
    // Cuerpo 2: Venus (r ≈ 0.72 UA)
    bodies[2].rx = 0.72;
    bodies[2].ry = 0.0;
    bodies[2].vx = 0.0;
    bodies[2].vy = sqrt(1.0 / 0.72);
    bodies[2].m  = 2.45e-6;
    
    // Cuerpo 3: Tierra (r = 1.0 UA)
    bodies[3].rx = 1.0;
    bodies[3].ry = 0.0;
    bodies[3].vx = 0.0;
    bodies[3].vy = sqrt(1.0 / 1.0);
    bodies[3].m  = 3.0e-6;
    
    // Cuerpo 4: Marte (r ≈ 1.52 UA)
    bodies[4].rx = 1.52;
    bodies[4].ry = 0.0;
    bodies[4].vx = 0.0;
    bodies[4].vy = sqrt(1.0 / 1.52);
    bodies[4].m  = 3.22e-7;
    
    // Cuerpo 5: Júpiter (r ≈ 5.2 UA)
    bodies[5].rx = 5.2;
    bodies[5].ry = 0.0;
    bodies[5].vx = 0.0;
    bodies[5].vy = sqrt(1.0 / 5.2);
    bodies[5].m  = 9.55e-4;
    
    // Cuerpo 6: Saturno (r ≈ 9.54 UA)
    bodies[6].rx = 9.54;
    bodies[6].ry = 0.0;
    bodies[6].vx = 0.0;
    bodies[6].vy = sqrt(1.0 / 9.54);
    bodies[6].m  = 2.85e-4;
    
    // Cuerpo 7: Urano (r ≈ 19.2 UA)
    bodies[7].rx = 19.2;
    bodies[7].ry = 0.0;
    bodies[7].vx = 0.0;
    bodies[7].vy = sqrt(1.0 / 19.2);
    bodies[7].m  = 4.37e-5;
    
    // Cuerpo 8: Neptuno (r ≈ 30.1 UA)
    bodies[8].rx = 30.1;
    bodies[8].ry = 0.0;
    bodies[8].vx = 0.0;
    bodies[8].vy = sqrt(1.0 / 30.1);
    bodies[8].m  = 5.15e-5;
}

void computeAccelerations(Body bodies[]) {
    int i, j;
    double dx, dy, r2, r3;
    
    // Inicializa las aceleraciones a cero
    for(i = 0; i < N_BODIES; i++) {
        bodies[i].ax = 0.0;
        bodies[i].ay = 0.0;
    }
    
    for(i = 0; i < N_BODIES; i++) {
        for(j = 0; j < N_BODIES; j++) {
            if(i != j) {
                dx = bodies[i].rx - bodies[j].rx;
                dy = bodies[i].ry - bodies[j].ry;
                r2 = dx * dx + dy * dy;
                r3 = pow(r2, 1.5);
                if(r3 > 1e-10) { // Evitar división por cero
                    bodies[i].ax -= bodies[j].m * dx / r3;
                    bodies[i].ay -= bodies[j].m * dy / r3;
                }
            }
        }
    }
}

void velocityVerletStep(Body bodies[]) {
    int i;
    double old_ax[N_BODIES], old_ay[N_BODIES];
    
    for(i = 0; i < N_BODIES; i++) {
        old_ax[i] = bodies[i].ax;
        old_ay[i] = bodies[i].ay;
    }
    
    // Actualiza posiciones: r(t+DT) = r(t) + v(t)*DT + 0.5*a(t)*DT²
    for(i = 0; i < N_BODIES; i++) {
        bodies[i].rx += bodies[i].vx * DT + 0.5 * bodies[i].ax * DT * DT;
        bodies[i].ry += bodies[i].vy * DT + 0.5 * bodies[i].ay * DT * DT;
    }
    
    // Recalcula aceleraciones con las nuevas posiciones
    computeAccelerations(bodies);
    
    // Actualiza velocidades: v(t+DT) = v(t) + 0.5*[a(t) + a(t+DT)]*DT
    for(i = 0; i < N_BODIES; i++) {
        bodies[i].vx += 0.5 * (old_ax[i] + bodies[i].ax) * DT;
        bodies[i].vy += 0.5 * (old_ay[i] + bodies[i].ay) * DT;
    }
}
