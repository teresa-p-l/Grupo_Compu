#include <stdio.h>
#include <math.h> //Necesario para la potencia

//Algoritmo de Verlet.

//Tenemos tres vectores: posición, velocidad y aceleración.
//Las vectores son bidimensionales y estarán el el formato:
//x1 y1 x2 y2 x3 y3 ... xn yn
//Lo mismo pasa con las velocidades y aceleraciones.

//Las masas estarán en un vector unidimensional.

//Definimos los vectores

//Los pasos son el número de pasos que queremos dar en la simulación.

#define pasos 1000
#define tiempo 10000
#define planetas 2 //Número de planetas POR AHORA, LUEGO SE PONE BIEN COMO LO DEL DOCUMENTO Y TAL.

double h=tiempo/pasos;

// Estructura para cada cuerpo
typedef struct {
    double rx, ry;      // Posición reescalada: r' = r / C
    double vx, vy;      // Velocidad reescalada
    double ax, ay;      // Aceleración reescalada
    double m;           // Masa reescalada: m' = m / MS
} Body;


//Hacemos el programa de las aceleraciones pero con las estructuras. Calculamos la aceleración con Newton.
//Partimos de que todo ya está reescalado y que los datos se leerán en otro archivo.

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


void verlet(Body cuerpos[])
{
    double acelvieja[planetas][2]; //Aceleración vieja de cada planeta.
    double omega[planetas][2]; //Vector auxiliar

    //Guardamos para el file PODEMOS GUARDAR TAMBIÉN POSICIÓN Y VELOCIDAD PARA EL FILE PERO ESO MEJOR EN UNA FUNCIÓN ANTERIOR A VERLET.
    for (int i=0; i<planetas; i++)
    {
        acelvieja[i][0]=cuerpos[i].rx;
        acelvieja[i][1]=cuerpos[i].ry;
    }
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
        cuerpos[i].vx = omega[i][0] + h/2*(cuerpos[i].ax+acelvieja[i][0]);
        cuerpos[i].vy = omega[i][1] + h/2*(cuerpos[i].ay+acelvieja[i][1]);
    }
    //Con esto ya tenemos r(t+h), v(t+h) y a(t+h). Dichos parámetros se han actualizado en el cuerpo.
}


int main(void)
{
printf("Helloworld");
return 0;
}


