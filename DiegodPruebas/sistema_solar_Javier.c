#include <stdio.h>
#include <math.h>

#define N 9 // 1 Sol + 8 planetas
#define DIM 2 // Movimiento en plano 2D
#define G 39.47841760435743 // (AU³)/(año²·masa_solar)
#define h 0.001 // Paso temporal (en años)
#define PASOS 100000 // Número total de pasos (~100 años)

// Estructura para almacenar planetas
typedef struct {
    char nombre[10];
    double masa; // en masas solares
    double pos[DIM]; // posición x, y (AU)
    double vel[DIM]; // velocidad vx, vy (AU/año)
    double acel[DIM]; // aceleración ax, ay
} cuerpo;

// Inicializa planetas con datos reales aproximados
void inicializa_planetas(cuerpo cuerpos[N]){
    // Sol
    cuerpos[0] = (cuerpo){"Sol", 1.0, {0, 0}, {0, 0}, {0, 0}};

    // Mercurio
    cuerpos[1] = (cuerpo){"Mercurio", 1.651e-7, {0.387, 0}, {0, 10.05}, {0, 0}};

    // Venus
    cuerpos[2] = (cuerpo){"Venus", 2.447e-6, {0.723, 0}, {0, 7.38}, {0, 0}};

    // Tierra
    cuerpos[3] = (cuerpo){"Tierra", 3.003e-6, {1.0, 0}, {0, 6.28}, {0, 0}};

    // Marte
    cuerpos[4] = (cuerpo){"Marte", 3.213e-7, {1.524, 0}, {0, 5.09}, {0, 0}};

    // Júpiter
    cuerpos[5] = (cuerpo){"Jupiter", 9.545e-4, {5.203, 0}, {0, 2.76}, {0, 0}};

    // Saturno
    cuerpos[6] = (cuerpo){"Saturno", 2.857e-4, {9.537, 0}, {0, 2.04}, {0, 0}};

    // Urano
    cuerpos[7] = (cuerpo){"Urano", 4.366e-5, {19.191, 0}, {0, 1.43}, {0, 0}};

    // Neptuno
    cuerpos[8] = (cuerpo){"Neptuno", 5.151e-5, {30.069, 0}, {0, 1.14}, {0, 0}};
}

// Calcula aceleraciones gravitatorias
void calcula_aceleraciones(cuerpo cuerpos[N]){
    for(int i=0; i<N; i++){
        cuerpos[i].acel[0] = cuerpos[i].acel[1] = 0;
        for(int j=0; j<N; j++){
            if(i!=j){
                double dx = cuerpos[j].pos[0] - cuerpos[i].pos[0];
                double dy = cuerpos[j].pos[1] - cuerpos[i].pos[1];
                double dist3 = pow(dx*dx + dy*dy, 1.5) + 1e-10; // evitar división por cero
                cuerpos[i].acel[0] += G * cuerpos[j].masa * dx / dist3;
                cuerpos[i].acel[1] += G * cuerpos[j].masa * dy / dist3;
            }
        }
    }
}

// Algoritmo Verlet en velocidad
void verlet(cuerpo cuerpos[N]){
    for(int i=0; i<N; i++){
        // actualizar posiciones
        cuerpos[i].pos[0] += cuerpos[i].vel[0]*h + 0.5*cuerpos[i].acel[0]*h*h;
        cuerpos[i].pos[1] += cuerpos[i].vel[1]*h + 0.5*cuerpos[i].acel[1]*h*h;
    }

    // aceleración anterior
    double acel_ant[N][DIM];
    for(int i=0;i<N;i++){
        acel_ant[i][0] = cuerpos[i].acel[0];
        acel_ant[i][1] = cuerpos[i].acel[1];
    }

    // nuevas aceleraciones
    calcula_aceleraciones(cuerpos);

    // actualizar velocidades
    for(int i=0; i<N; i++){
        cuerpos[i].vel[0] += 0.5*(acel_ant[i][0] + cuerpos[i].acel[0])*h;
        cuerpos[i].vel[1] += 0.5*(acel_ant[i][1] + cuerpos[i].acel[1])*h;
    }
}

// programa principal
int main(){
    cuerpo cuerpos[N];
    inicializa_planetas(cuerpos);
    calcula_aceleraciones(cuerpos);

    FILE *f = fopen("orbitas.csv", "w");
    fprintf(f, "t,");
    for(int i=0; i<N; i++)
        fprintf(f, "%s_x,%s_y,", cuerpos[i].nombre, cuerpos[i].nombre);
    fprintf(f,"\n");

    // Integración numérica (verlet)
    for(int paso=0; paso<PASOS; paso++){
        verlet(cuerpos);
        if(paso%100==0){ // imprime cada 100 pasos
            fprintf(f, "%lf,", paso*h);
            for(int i=0; i<N; i++)
                fprintf(f, "%lf,%lf,", cuerpos[i].pos[0], cuerpos[i].pos[1]);
            fprintf(f, "\n");
        }
    }
    fclose(f);

    printf("Simulación finalizada. Datos guardados en orbitas.csv\n");
    return 0;
}
