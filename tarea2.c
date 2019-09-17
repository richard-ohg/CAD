/* Hernández González Ricardo Omar
Tarea 2: Cálculo de pi 

Observaciones:
- El proceso 0 solo sumará el resultado de todos los demás procesos
- El offset tiene que ser calculado con size-1, dada la razón de arriba
- El último proceso tiene que llegar al valor de "n"
 */

#include <stdio.h>
#include <mpi.h>
#include <math.h>

double f(double a){
    return (4.0/(1.0+a*a));
}

int main(int argc, char *argv[]){
    
    int n, rank, size, i, offset, startFor, endFor;
    double PI25DT = 3.141592653589793238462643; // Valor real de PI
    double myPi, pi, h, sum, x;
    int source, dest, tag = 100; // Valores para enviar y recibir mensajes

    MPI_Status status;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    n = 100;
    h = 1.0/(double)n;
    sum = 0.0;
    offset = n/(size-1);
    // printf("Offset: %d", offset);

    if (rank == 0){
        pi = 0;

        for (i = 1; i < size; i++){
            source = i;
            MPI_Recv(&myPi, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
            pi += myPi;
        }

        printf("\npi es aprox %.16f, el error es %.16f", pi, fabs(pi - PI25DT));
    }else{
        dest = 0;
        startFor = ((rank - 1) * offset) + 1; 
        endFor = (rank == size-1) ? n : rank*offset; // Si es el último proceso lo obligamos a llegar a n
    
        for (i = startFor; i <= endFor; i++){
            x = h * ((double)i - 0.5);
            sum += f(x);
        }
        myPi = h * sum;

        printf("\nProceso %d, con rango en for: %d - %d, mypi: %.16f\n", rank, startFor, endFor ,myPi);
        MPI_Send(&myPi, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    
    return 0;
}
