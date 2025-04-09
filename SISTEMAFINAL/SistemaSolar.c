#include <stdio.h>
#include <string.h>
#include <math.h> //Necesario para la potencia

//Abrimos el archivo

archivo = fopen("c:/Users/User/Documents/Fisica_compu/Compu/Grupo_Compu/SistemaSolar/initial.txt", "r");

// Estructura para cada cuerpo
typedef struct {
    double rx, ry;      // Posición reescalada: r' = r / C
    double vx, vy;      // Velocidad reescalada
    double ax, ay;      // Aceleración reescalada
    double m;           // Masa reescalada: m' = m / MS
    double e;          // Excentricidad
} Body;


//DEFINIMOS AHORA LAS CONSTANTES

#define pasos 1000
#define tiempo 10000
#define planetas 2 //Número de planetas

//por si hay alguna inconsistencia en el número de planetas, ponemos una variable como N

int N = planetas; //Número de planetas, por si acaso.

double h=tiempo/pasos; //Paso de tiempo, que es el tiempo total dividido por el número de pasos.



//FUNCION DE REESCALAMIENTO: DEFINIMOS LAS CONSTANTES Y HACEMOS LA FUNCIÓN

#define G 6.67e-11        // Constante de gravitación en N·m²/kg² (no usada directamente aquí)
#define MS 1.99e30        // Masa del Sol en kg
#define C 1.496e11 

void reescalamiento(double *rx, double *ry, double *t, double *m)
{

    *rx = *rx/C;
    *ry = *ry/C;
    *t = *t*sqrt(G*MS/(C*C*C));
    *m= *m/MS; 
}

void reescalamientoinverso(double *rx, double *ry, double *t, double *m)
{

    *rx = *rx*C;
    *ry = *ry*C;
    *t = *t/sqrt(G*MS/(C*C*C));
    *m= *m*MS; 
}


//FUNCION DE INICIALIZACIÓN:




//FUNCION DE ACELERACIÓN:
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


//PROCEDIMIENTO DE VERLET:




void verlet(Body cuerpos[], Body historial[][planetas], int paso, FILE *file)
{
    for (int i=0; i<planetas; i++)
    {
        historial[paso][i].rx=cuerpos[i].rx;
        historial[paso][i].ry=cuerpos[i].ry;
        historial[paso][i].vx=cuerpos[i].vx;
        historial[paso][i].vy=cuerpos[i].vy;
        historial[paso][i].ax=cuerpos[i].ax;
        historial[paso][i].ay=cuerpos[i].ay;
    }
    double omega[planetas][2]; //Vector auxiliar

    //Guardamos para el file PODEMOS GUARDAR TAMBIÉN POSICIÓN Y VELOCIDAD PARA EL FILE PERO ESO MEJOR EN UNA FUNCIÓN ANTERIOR A VERLET.
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
        cuerpos[i].vx = omega[i][0] + h/2*(cuerpos[i].ax+historial[paso][i].ax);
        cuerpos[i].vy = omega[i][1] + h/2*(cuerpos[i].ay+historial[paso][i].ay);
    }
    //Con esto ya tenemos r(t+h), v(t+h) y a(t+h). Dichos parámetros se han actualizado en el cuerpo.


}



//Programa para calcular la energía de un cuerpo en el sistema solar. LO HACEMOS DE 2 FORMAS DISTINTAS


void Energia(Body cuerpos[], int N, FILE *archivo)
{   
    double E[N];
    double l;
    int i; 

    for(int i=0; i<N; i++){
    
    l=cuerpos[i].m*(cuerpos[i].rx*cuerpos[i].vy - cuerpos[i].ry*cuerpos[i].vx);

    E[i]=((cuerpos[i].e*cuerpos[i].e)-1)*(G*G*MS*MS*cuerpos[i].m*cuerpos[i].m*cuerpos[i].m)/(2*l*l);

    fprintf(archivo, "%e\n", E[i]);
    }

}

double EnergiaAlternativa(Body cuerpos[], int N)
{   
    double E[N];
    double v;
    double r;
    int i; 
    
    for(int i=0; i<N; i++){
    v=sqrt(cuerpos[i].vx*cuerpos[i].vx+cuerpos[i].vy*cuerpos[i].vy);
    r=sqrt(cuerpos[i].rx*cuerpos[i].rx+cuerpos[i].ry*cuerpos[i].ry);

    E[i]=(cuerpos[i].m*v*v)/2 - (G*MS*cuerpos[i].m)/r;
    
    fprintf(archivo, "%e\n", E[i]);
    }
}



void inicializarCuerpos(Body cuerpos[], int N, FILE *archivo) 
{
    int i; 

    for(i=0; i<N; i++){
        fscanf(archivo, "%lf %lf %lf %lf ", 
            &cuerpos[i].m, &cuerpos[i].rx, &cuerpos[i].ry, &cuerpos[i].e);
    }
}





//PROGRAMA PRINCIPAL:

int main(void)
{
    printf("Hello world\n");

    // Inicializar los cuerpos
    
    int N,i; 
    FILE *archivo;
 
    N=9;
 
    Body cuerpos[N];
   
   
   inicializarCuerpos(cuerpos, N, archivo);

   for(i=0; i<N; i++){
    printf( "m = %lf, rx = %lf, ry = %lf, e= %lf,\n", 
        cuerpos[i].m, cuerpos[i].rx, cuerpos[i].ry, cuerpos[i].e);
    }

    fclose(archivo);


    /*=================================
    
    FALTA LEER LOS DATOS E INICIALIZARLOS
    
    ==================================*/

    // Calculamos las aceleraciones iniciales
    //aceleracion(cuerpos);

    return 0;
}