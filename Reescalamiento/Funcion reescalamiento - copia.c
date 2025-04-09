#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define G 6.67e-11        // Constante de gravitación en N·m²/kg² (no usada directamente aquí)
#define MS 1.99e30        // Masa del Sol en kg
#define C 1.496e11 

typedef struct {
    double rx, ry;      // Posición 
    double e;           // excentricidad
    double m;          // Masa
    double t;          // Perioso
} Body;


void reescalamiento(*cuerpos[], int N)
{
    for i=0; i<N; i++){
    *cuerpos[i].rx = *cuerpos[i].rx/C;
    *cuerpos[i].ry = *cuerpos[i].ry/C;
    *cuerpos[i].t = *cuerpos[i].t*sqrt(G*MS/(C*C*C));
    *cuerpos[i].m= *cuerpos[i].m/MS; 
    }
}

void reescalamientoinverso(*cuerpos[], int N)
{
    for i=0; i<N; i++){
    *cuerpos[i].rx = *cuerpos[i].rx*C;
    *cuerpos[i].ry = *cuerpos[i].ry*C;
    *cuerpos[i].t = *cuerpos[i].t/sqrt(G*MS/(C*C*C));
    *cuerpos[i].m= *cuerpos[i].m*MS; 
    }
}

double Energia(cuerpos[], int N)
{   
    double E[N];
    double l;

    for(int i=0; i<N; i++){
    
    l=cuerpos[i].m*(cuerpos[i].rx*vy - cuerpos[i].ry*cuerpos[i].vx);

    E[]=((cuerpos[i].e*cuerpos[i].e)-1)*(G*G*MS*MS*cuerpos[i].m*cuerpos[i].m*cuerpos[i].m)/(2*l*l);
    }
    return E[];
}

double EnergiaAlternativa(Body cuerpos[], int N)
{   
    double E[N];
    double v;
    double r;
    
    for(int i=0; i<N; i++){
    v=sqrt(cuerpos[i].vx*cuerpos[i].vx+cuerpos[i]vy*cuerpos[i].vy);
    r=sqrt(cuerpos[i].rx*cuerpos[i].rx+cuerpos[i].ry*cuerpos[i].ry);

    E[i]=(cuerpos[i].m*v*v)/2 - (G*MS*cuerpos[i].m)/r;
    }
    return E[i];
}


void inicializarCuerpos(Body cuerpos[], int N, FILE *archivo) {
    int i; 

    for(i=0; i<N; i++){
        fscanf(archivo, "%lf %lf %lf %lf ", 
            &cuerpos[i].m, &cuerpos[i].rx, &cuerpos[i].ry, &cuerpos[i].e);
    }
}

int main(void)
{  double x,y,t,m,vx,vy,e;
   int N,i; 
   FILE *archivo;

   N=9;

   Body cuerpos[N];
  archivo = fopen("c:/Users/User/Documents/Fisica_compu/Compu/Grupo_Compu/SistemaSolar/initial.txt", "r");
  
  inicializarCuerpos(cuerpos, N, archivo);

  for(i=0; i<N; i++){
        printf( "m = %lf, rx = %lf, ry = %lf, e= %lf,\n", 
            cuerpos[i].m, cuerpos[i].rx, cuerpos[i].ry, cuerpos[i].e);
    }

    fclose(archivo);


   x= 4.495*pow(10, 12);
   y= 0;
   t=180000000000;
   e=0.009;
   vx=0;
   vy=5433;
   m=1.024*pow(10, 26);
   
   reescalamiento(&x,&y,&t,&m);
   printf("x = %lf, y = %lf, t = %lf, m = %lf\n", x, y, t, m);
   reescalamientoinverso(&x,&y,&t,&m);
   printf("x = %lf, y = %lf, t = %lf, m = %lf\n", x, y, t, m);

    printf("E = %lf\n", Energia(e, m, x, y, vx, vy));
    printf("E = %lf\n", EnergiaAlternativa(m, x, y, vx, vy));
}



