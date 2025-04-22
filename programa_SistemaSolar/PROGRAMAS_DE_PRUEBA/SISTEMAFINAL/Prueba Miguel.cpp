#include <stdio.h>
#include <string.h>
#include <math.h> //Necesario para la potencia

//Abrimos el archivo

/*
 ==================================
    EL ARCHIVO TIENE EL FORMATO: 
    MASA, rx, vy, e
    rx y ry son las coordenadas del cuerpo en el sistema solar, vy la velocidad eje y, e es la excentricidad.
 ==================================
*/

// Estructura para cada cuerpo
typedef struct {
    double rx, ry;      // Posición reescalada: r' = r / C
    double vx, vy;      // Velocidad reescalada
    double ax, ay;      // Aceleración reescalada
    double m;           // Masa reescalada: m' = m / MS
    double e;          // Excentricidad
    double t;          // Período reescalado:
} Body;


//DEFINIMOS AHORA LAS CONSTANTES

#define pasos 10000
#define tiempo 100000
#define planetas 9 //Número de planetas

//por si hay alguna inconsistencia en el número de planetas, ponemos una variable como N

int N = planetas; //Número de planetas, por si acaso.

double h=tiempo/pasos; //Paso de tiempo, que es el tiempo total dividido por el número de pasos.


//FUNCION DE REESCALAMIENTO: DEFINIMOS LAS CONSTANTES Y HACEMOS LA FUNCIÓN

#define G 6.67e-11        // Constante de gravitación en N·m²/kg² (no usada directamente aquí)
#define MS 1.99e30        // Masa del Sol en kg
#define C 1.496e11 

void reescalamiento(Body cuerpos[], int N)
{   
    int i; 

    for (i=0; i<N; i++)
    {
    cuerpos[i].rx = cuerpos[i].rx/C;
    cuerpos[i].ry = cuerpos[i].ry/C;
    cuerpos[i].vx = cuerpos[i].vx / sqrt(G * MS / C); 
    cuerpos[i].vy = cuerpos[i].vy / sqrt(G * MS / C); 
    cuerpos[i].t = cuerpos[i].t*sqrt(G*MS/(C*C*C));
    cuerpos[i].m= cuerpos[i].m/MS; 
    }
}

void reescalamientoinverso(Body cuerpos[], int N)
{
    int i; 

    for (i=0; i<N; i++)
    {
    cuerpos[i].rx = cuerpos[i].rx*C;
    cuerpos[i].ry = cuerpos[i].ry*C;
    cuerpos[i].t = cuerpos[i].t/sqrt(G*MS/(C*C*C));
    cuerpos[i].m = cuerpos[i].m*MS; 
    }
}


//FUNCION DE ACELERACIÓN:
void aceleracion(Body cuerpos[]) 
{
    double distcubo;
    double dx, dy;

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
                cuerpos[i].ax -= G*cuerpos[j].m*dx/distcubo; //Aceleración en x
                cuerpos[i].ay -= G*cuerpos[j].m*dy/distcubo; //Aceleración en y
            }
        }
    }    
}


//PROCEDIMIENTO DE VERLET:

void verlet(Body cuerpos[], FILE *file)
{
    double old_ax[planetas], old_ay[planetas]; //Vectores auxiliares para guardar la aceleración anterior
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
        cuerpos[i].vx = omega[i][0] + h/2*cuerpos[i].ax;
        cuerpos[i].vy = omega[i][1] + h/2*cuerpos[i].ay;
    }
    //Con esto ya tenemos r(t+h), v(t+h) y a(t+h). Dichos parámetros se han actualizado en el cuerpo.

    //Vamos a guardar los datos en el archivo de salida.

    /*
    =====================================
    FORMATO DEL ARCHIVO DE SALIDA:
    1rx,1ry,1vx,1vy
    2rx,2ry,2vx,2vy
    .
    .
    .
    Nrx,Nry,Nvx,Nvy
    [salto de línea para separar los pasos]
    1rx,1ry,1vx,1vy
    ...

    =====================================
    
    */
    for (int i=0; i<planetas; i++)
    {   cuerpos[i].rx = cuerpos[i].rx/C;
        cuerpos[i].ry = cuerpos[i].ry/C;
        fprintf(file, "%lf, %lf\n", cuerpos[i].rx, cuerpos[i].ry,);
        cuerpos[i].rx = cuerpos[i].rx*C;
        cuerpos[i].ry = cuerpos[i].ry*C;
    }
    fprintf(file, "\n"); //Salto de línea para separar los pasos

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
    } //Reescalamos la energía para que esté en unidades de la energía del sistema solar.
    fprintf(archivo, "%e\n", E[i]);
    }

}

double EnergiaAlternativa(Body cuerpos[], int N, FILE *archivo)
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



//FUNCION PARA INICIALIZAR LOS CUERPOS:

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

    double aux[N];
    
    int N,i; N=9; Body cuerpos[N];

    //Abrimos los 3 files, el de entrada, salida y el de energía.
    FILE *archivo = fopen("c:/Users/User/Documents/Fisica_compu/Compu/Grupo_Compu/SISTEMAFINAL/initial.txt", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 1;
    }
    
    
    FILE *archivoSalida = fopen("c:/Users/User/Documents/Fisica_compu/Compu/Grupo_Compu/SISTEMAFINAL/output.txt", "w");
    if (archivoSalida == NULL) {
        printf("Error al abrir el archivo de salida.\n");
        fclose(archivo);
        return 1;
    }


    FILE *archivoEnergia = fopen("c:/Users/User/Documents/Fisica_compu/Compu//Grupo_Compu/SISTEMAFINAL/energia.txt", "w");
    if (archivoEnergia == NULL) {
        printf("Error al abrir el archivo de energía.\n");
        fclose(archivo);
        return 1;
    }
   
   inicializarCuerpos(cuerpos, N, archivo);

    //Reescalamos los cuerpos
    /*reescalamiento(cuerpos, N);
    for(i=0; i<N; i++){
        printf( "m = %lf, rx = %lf, vy = %lf, e= %lf,\n", 
            cuerpos[i].m, cuerpos[i].rx, cuerpos[i].vy, cuerpos[i].e);
        }

    */

    //Implementamos el algoritmo de Verlet
    // Inicializamos las posiciones y velocidades de los cuerpos
    //Calculamos la aceleración inicial
    //Inicializamos la aceleración a 0 SKIPEABLE. 
    //También ponemos la velocidad inicial de estos planetas para seguir órbitas "circulares"
    for (int i=0; i<planetas; i++) 
    {
        cuerpos[i].ax = 0.0;
        cuerpos[i].ay = 0.0;
        
        if(i!=0)
            cuerpos[i].vy= sqrt(G*MS/cuerpos[i].rx); // Velocidad inicial en y para órbita circular 
     }
    aceleracion(cuerpos);

    //Saco por pantalla la aceleración inicial de estos cuerpos
    for (int i=0; i<N; i++)
    {
        printf( "ax = %lf, ay = %lf\n", 
            cuerpos[i].ax, cuerpos[i].ay);
    }

    for (int i=0; i < pasos; i++)
    {
        verlet(cuerpos, archivoSalida);
        Energia(cuerpos, 9 , archivoEnergia);
        fprintf(archivoEnergia, "\n");
    }
    

    fclose(archivo);
    fclose(archivoSalida);
    fclose(archivoEnergia);


    /*=================================
    
    FALTA LEER LOS DATOS E INICIALIZARLOS
    
    ==================================*/

    // Calculamos las aceleraciones iniciales
    //aceleracion(cuerpos);

    return 0;
}