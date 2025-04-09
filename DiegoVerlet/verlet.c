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
#define planetas 2 //Número de planetas POR AHORA, LUEGO SE PONE BIEN COMO LO DEL DOCUMENTO Y TAL.


//IMPORTANTE, TODO YA ESTÁ REESCALADO, NO HACE FALTA HACERLO AQUÍ.
double pos[planetas][2];
double vel[planetas][2];
float masa[planetas]; 
double acel[planetas][2]; //Aceleración de cada planeta en cada paso.





//Primero hago la 2ª ley de Newton, calculamos a(t) a partir de r(t) y m(t).




void aceleracion(double pos[planetas][2], float masa[planetas]) 
{
    double distcubo;
    double dx, dy;
    //Inicializamos la aceleración a 0 SKIPEABLE.
    for (int i=0; i<planetas; i++) 
    {
        acel[i][0] = 0.0; 
        acel[i][1] = 0.0; 
    }

    //Calculamos la aceleración de cada planeta.

    for (int i=0; i<planetas; i++)
    {
        for (int j=0; j<planetas; j++)
        {
            if (i != j) //No calculamos la aceleración de un planeta sobre sí mismo.
            {
                dx = pos[i][0] - pos[j][0]; //Diferencia en x
                dy = pos[i][1] - pos[j][1]; //Diferencia en y
                distcubo = pow(dx*dx + dy*dy, 1.5); //Distancia al cubo
                acel[i][0] -= masa[j]*dx/distcubo; //Aceleración en x
                acel[i][1] -= masa[j]*dy/distcubo; //Aceleración en y
            }
        }
    }
    return;
    
}

void verlet(pos[planetas][2], vel[planetas][2], acel[planetas][2])
{
    double posnueva[planetas][2]; //Posición nueva de cada planeta.
    double velnueva[planetas][2]; //Velocidad nueva de cada planeta.
    double acelnueva[planetas][2]; //Aceleración nueva de cada planeta.



    for (int i=0; i<planetas; i++)
    {
        pos[i][0] += vel[i][0] + 0.5*acel[i][0]; //Posición en x
        pos[i][1] += vel[i][1] + 0.5*acel[i][1]; //Posición en y

        vel[i][0] += acel[i][0]; //Velocidad en x
        vel[i][1] += acel[i][1]; //Velocidad en y

        pos[i][0] += 0.5*acel[i][0]; //Posición en x
        pos[i][1] += 0.5*acel[i][1]; //Posición en y

        vel[i][0] += acel[i][0]; //Velocidad en x
        vel[i][1] += acel[i][1]; //Velocidad en y
    }
}


int main(void)
{
printf("Helloworld");
return 0;
}


