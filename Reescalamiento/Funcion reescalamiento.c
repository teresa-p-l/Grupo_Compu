#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


int main(void)
{  double x,y,t,m;
    
   x= 4500000000;
   y= 4500000000;
   t=180000000000;
   m=1.024*pow(10, 26);
   
   reescalamiento(&x,&y,&t,&m);
   printf("x = %lf, y = %lf, t = %lf, m = %lf\n", x, y, t, m);
   reescalamientoinverso(&x,&y,&t,&m);
   printf("x = %lf, y = %lf, t = %lf, m = %lf\n", x, y, t, m);
}
