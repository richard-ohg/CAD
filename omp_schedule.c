// Compilar omp en Mac -> gcc-9 -fopenmp archivo.c -o ejecutable
// Ejecutar -> ./ejecutable

/* Observaciones de los diferentes schedule:

* static -> intenta dividir entre el número de hilos el "crunk" que se le haya puesto, en este ejemplo pusimos un chunk de 5, entonces tratará de darle 5 iteraciones a cada hilo
* dynamic -> a cada hilo le trata de dar el número de iteraciones que indique "chunk", pero no en partes iguales, si no al hilo que no este trabajando, en este caso al correrlo con 5 hilos, pasó que un hilo hizo más iteraciones y 2 hilos no hicieron ninguna
* guided -> un thread toma iteraciones dinamicamente pero van disminuyendo el número de iteraciones progresivamente, en este caso observé que un hilo tomó varias iteraciones y los demás hilos tomaron menos, a tal grado que algunos hilos no hicieron ni una iteración

*/

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define CHUNKSIZE 5
#define N 20

int main(int argc, char const *argv[])
{
    int nthreads, tid, i, chunk;
    float a[N], b[N], c[N];

    for (int i = 0; i < N; i++)
        a[i] = b[i] = i * 1.0;
    
    #pragma omp parallel shared(a, b, c, chunk) private(tid, i, nthreads)
    {
        tid = omp_get_thread_num();

        if (tid == 0){
            nthreads = omp_get_num_threads();
            printf("Numero de hilos = %d\n", nthreads);
        }

        printf("Iniciando Hilo - %d\n", tid);

        // #pragma omp for schedule(static, CHUNKSIZE)
        // #pragma omp for schedule(dynamic, CHUNKSIZE)
        #pragma omp for schedule(guided, CHUNKSIZE)
        for (int i = 0; i < N; i++){
            c[i] = a[i] + b[i];
            printf("Hilo %d: c[%d] = %f\n", tid, i, c[i]);
        }        
    }

    return 0;
}
