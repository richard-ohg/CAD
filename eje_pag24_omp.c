/* 
    Programa de la página 24 de la presentación OpenMP-2018.pdf
    Ejercicio:
        Paralelizar
*/

#include <omp.h> 
#include <stdio.h> 
#include <stdlib.h>
#define SIZE 10

int main() {
    int i;
    int max; int a[SIZE];
    
    #pragma omp parallel for private(i)
    for(i=0;i<SIZE;i++) {
        a[i] = rand();
        printf("%d\n", a[i]); 
    }

    max = a[0];
    
    #pragma omp parallel for private(i)
    for(i=1;i<SIZE;i++) {
        #pragma omp critical
        {
            if (a[i] > max){ 
                max = a[i];
            } 
        }
    }
    printf("max = %d\n", max); 
}