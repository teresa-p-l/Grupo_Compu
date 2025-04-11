#include<stdio.h>
#include<math.h>

#define N 9         // Número de cuerpos: Sol + 8 planetas
#define DIM 2       // Trabajamos en 2D: eje x y eje y
#define G 39.47841760435743 // Constante de gravitación universal en unidades AU³/año²/masa solar
#define h 0.001     // Paso temporal (años). h = 0.001 años = ~0.365 días
#define PASOS 100000 // Número total de pasos de simulación (~100 años)
#define MASA_SOL 1.989e30               // kg
#define DISTANCIA_UA 1.496e8            // km
#define SEGUNDOS_POR_ANIO 31557600.0    // s


typedef struct
{
    char nombre[10]; // Nombre del cuerpo celeste
    double masa;     // Masa del cuerpo celeste (en masas solares)
    double pos[DIM]; // Posición (x, y) en unidades astronómicas (AU)
    double vel[DIM]; // Velocidad (vx, vy) en AU/año
    double acel[DIM];// Aceleración (ax, ay) en AU/año²
} cuerpo;

void inicializar(cuerpo cuerpos[N])
{
    cuerpos[0] = (cuerpo){"Sol", 1.0, {0, 0}, {0, 0}, {0, 0}};
    cuerpos[1] = (cuerpo){"Mercurio", 1.651e-7, {0.387, 0}, {0, 10.05}, {0, 0}};
    cuerpos[2] = (cuerpo){"Venus", 2.447e-6, {0.723, 0}, {0, 7.38}, {0, 0}};
    cuerpos[3] = (cuerpo){"Tierra", 3.003e-6, {1.0, 0}, {0, 6.28}, {0, 0}};
    cuerpos[4] = (cuerpo){"Marte", 3.213e-7, {1.524, 0}, {0, 5.09}, {0, 0}};
    cuerpos[5] = (cuerpo){"Jupiter", 9.545e-4, {5.203, 0}, {0, 2.76}, {0, 0}};
    cuerpos[6] = (cuerpo){"Saturno", 2.857e-4, {9.537, 0}, {0, 2.04}, {0, 0}};
    cuerpos[7] = (cuerpo){"Urano", 4.366e-5, {19.191, 0}, {0, 1.43}, {0, 0}};
    cuerpos[8] = (cuerpo){"Neptuno", 5.151e-5, {30.069, 0}, {0, 1.14}, {0, 0}};
}


