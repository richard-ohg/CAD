#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]) {
    double *x, *y,dot;
    int   n,i;

    printf("Tamaño del vector: ");
    scanf("%d", &n);
    printf("Tamaño de n en bytes: %d\n",sizeof(n));
    printf("Tamaño del vector: %d\n",sizeof(x));
    x = (double *) calloc(n, sizeof(double));
    y = (double *) calloc(n, sizeof(double));
    
    #pragma omp parallel for private(i)
    for (i=0; i<n; i++){
        x[i] = 1.0;  
        y[i] = 1.0;
    }

    #pragma omp parallel for
    for (i = 0; i < n; i++){
        #pragma omp critical
        dot += x[i] * y[i];
    }
   
    printf("Tamaño del vector: %d\n",sizeof(*x));

    free(x);
    free(y);
    printf("El producto punto es %.2f \n", dot);

    return 0;
}


