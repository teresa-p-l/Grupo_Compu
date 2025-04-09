#include <stdio.h>
#include <string.h>
#include <math.h> //Necesario para la potencia

typedef struct {
    double rx, ry;      // Posición reescalada: r' = r / C
    double vx, vy;      // Velocidad reescalada
    double ax, ay;      // Aceleración reescalada
    double m;           // Masa reescalada: m' = m / MS
    double e;          // Excentricidad
    double t;          // Período reescalado:
} Body;

#define G 6.67e-11        // Constante de gravitación en N·m²/kg² (no usada directamente aquí)
#define MS 1.99e30        // Masa del Sol en kg
#define C 1.496e11        // Unidad de distancia: distancia Tierra-Sol en metros

#define planetas 2               // Número de cuerpos (por ejemplo, Sol y Tierra)
#define T_TOTAL 10.0    // Tiempo total de simulación (en unidades reescaladas)
#define h 0.01           // Paso temporal

int N = planetas; //Número de planetas, por si acaso.
void aceleracion(Body cuerpos[]) 
{
    double distcubo;
    double dx, dy;
    //Inicializamos la aceleración a 0 SKIPEABLE.
    for (int i=0; i<planetas; i++) 
    {
        cuerpos[i].ax = 0.0;
        cuerpos[i].ay = 0.0; 
    }

    //Calculamos la aceleración de cada planeta.

    for (int i=0; i<planetas; i++)
    {
        for (int j=0; j<planetas; j++)
        {
            if (i != j) //No calculamos la aceleración de un planeta sobre sí mismo.
            {
                dx = cuerpos[i].rx-cuerpos[j].rx; //Diferencia en x
                dy = cuerpos[i].ry-cuerpos[j].ry; //Diferencia en y
                distcubo = pow(dx*dx + dy*dy, 1.5); //Distancia al cubo
                cuerpos[i].ax -= cuerpos[j].m*dx/distcubo; //Aceleración en x
                cuerpos[i].ay -= cuerpos[j].m*dy/distcubo; //Aceleración en y
            }
        }
    }
    return;
    
}


void verlet(Body cuerpos[], FILE *file)
{
    double acelvieja[planetas][2]; //Aceleración vieja de cada planeta.
    double omega[planetas][2]; //Vector auxiliar
    //Actualizamos posiciones y calculamos vector auxiliar omega
    for (int i=0; i<planetas; i++)
    {
        cuerpos[i].rx += cuerpos[i].vx * h + 0.5 * cuerpos[i].ax * h * h;
        cuerpos[i].ry += cuerpos[i].vy * h + 0.5 * cuerpos[i].ay * h * h;
        omega[i][0]=cuerpos[i].vx + h/2*cuerpos[i].ax;
        omega[i][1]=cuerpos[i].vy + h/2*cuerpos[i].ay; 
    }

    //Hemos obtenido r(t+h) y omega(t+h).

    //Actualizamos las aceleraciones (a(t+h)).
    aceleracion(cuerpos);

    //Por último actualizamos las velocidades.
    for (int i=0; i<planetas; i++)
    {
        cuerpos[i].vx = omega[i][0] + h/2*cuerpos[i].ax;
        cuerpos[i].vy = omega[i][1] + h/2*cuerpos[i].ay;
    }
    //Con esto ya tenemos r(t+h), v(t+h) y a(t+h). Dichos parámetros se han actualizado en el cuerpo.
    for (int i=0; i<planetas; i++)
    {
        fprintf(file, "%lf, %lf, %lf, %lf\n", cuerpos[i].rx, cuerpos[i].ry, cuerpos[i].vx, cuerpos[i].vy);

    }
    fprintf(file, "\n"); //Salto de línea para separar los pasos
}

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


int main (void)
{
    FILE *archivo = fopen("initial.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }

    FILE *file = fopen("SALIDA.txt", "w");
    if (file == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    Body cuerpos[N]; // Arreglo de cuerpos
    inicializarCuerpos(cuerpos);
    aceleracion(cuerpos);
    for(float i=0; i<T_TOTAL; i=i+h)
    {   
        verlet(cuerpos, file);
    }
    fclose(file);

    return 0;
}


