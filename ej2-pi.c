#include <stdio.h>
#include <mpi.h>
#include <math.h>

double f(double a){
    return (4.0/(1.0+a*a));
}

int main(int argc, char *argv[]){
    
    int n, rank, size, i;
    double PI25DT = 3.141592653589793238462643;
    double myPi, pi, h, sum, x;
    int source, dest, tag = 100;

    MPI_Status status; // Estructura con información
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    printf("\nProceso %d de %d", rank, size);
    n = 100;
    h = 1.0/(double)n;
    sum = 0.0;
    
    for (i = rank+1; i <= n; i += size){
        x = h * ((double)i - 0.5);
        sum += f(x);
    }

    myPi = h * sum;
    printf("\n Proceso %d, mypi: %.16f\n", rank, myPi);

    if (rank == 0){
        pi = myPi;
        for (i = 1; i < size; i++){
            source = i;
            MPI_Recv(&myPi, 1, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
            pi += myPi;
        }
        printf("\n pi es aprox %.16f, el error es %.16f", pi, fabs(pi - PI25DT));
    }else{
        dest = 0;
        MPI_Send(&myPi, 1, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    
    return 0;
}
