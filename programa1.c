// Compilar: $ mpicc programa.c -o nombre_ejecutable
// Ejecutar: $ mpirun -np #_procesos ./ejecutable

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]){

    int rank, size, origen, destino, tag=1;
    int mensaje[100];

    MPI_Status status; // Estructura con información
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank != 0){
        printf("Proceso %d de %d\n",rank, size);
        sprintf(mensaje, "Saludos desde el proceso %d", rank);
        destino = 0;
        MPI_Send(mensaje, strlen(mensaje)+1, MPI_CHAR, destino, tag, MPI_COMM_WORLD);
    }else{
        printf("Proceso 0, total de procesos: %d\n", size);
        for (origen = 1; origen < size; origen++){
            MPI_Recv(mensaje, 100, MPI_CHAR, origen, tag, MPI_COMM_WORLD, &status);
            printf("%s\n", mensaje);
        }
    }

    MPI_Finalize();
    
    return 0;
}
