/*
  Este programa calcula el valor de pi usando el método de Monte Carlo.
 */

//PREAMBULO
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

//PROGRAMA PRINCIPAL
int main(void)
{
  int i,j,N;
  double x,y,d,pi;
  double empezar, terminar;

  printf("Calculo de pi usando el metodo de Monte Carlo\n");
 
  srand48(time(NULL));

  j=0;
  N=100000000;

  empezar=omp_get_wtime();
  for(i=1;i<=N;i++){
    x=drand48();
    y=drand48();
    //printf("Punto: %f,%f\n",x,y);
    d=sqrt((x-0.5)*(x-0.5)+(y-0.5)*(y-0.5));
    if(d<0.5){
      j++;
      //printf("Cayo adentro\n");
    }
  }
  terminar = omp_get_wtime();

  printf("Numero de puntos que cayeron adentro: %d\n",j);
  pi=(j/(1.0*N))/(0.5*0.5);
  printf("Valor de pi estimado con Montecarlo: %f\n",pi);
  printf("TIEMPO=%lf\n",terminar-empezar);
  return 0;
}
