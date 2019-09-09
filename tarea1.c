// Hernández González Ricardo Omar

// Compilar: $ mpicc tarea1.c -o tarea1
// Ejecutar: $ mpirun -np #_procesos ./tarea1

#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char const *argv[]){

    int rank, size, origen, destino, tag=1;
    int mensaje[1];

    MPI_Status status; // Estructura con información
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Si es el nodo 0
    if(rank == 0){
        printf("Total de procesos: %d\n\n", size);
        // Debe enviar el dato al nodo 1
        destino = 1;
        // Debe recibir el dato del último nodo
        origen = size-1;

        // Inicializamos mensaje a enviar al ciclo
        mensaje[0] = 0;

        // Enviamos ese mensaje
        MPI_Send(mensaje, 1, MPI_INT, destino, tag, MPI_COMM_WORLD);
        printf("Primer proceso (Proceso %d), enviando al proceso %d el valor i: %d\n\n", rank, destino, mensaje[0]);
        // Recibimos mensaje del último nodo 
        MPI_Recv(mensaje, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &status);
        printf("Primer proceso (Proceso %d), recibiendo del proceso %d el valor i: %d\n", rank, origen, mensaje[0]);

    // Si es el último nodo
    }else if(rank == size-1){
        // Debe enviar el dato al primer nodo
        destino = 0;
        // Debe recibir el dato del penúltimo nodo
        origen = size-2;

        // Recibimos el mensaje del penúltimo nodo
        MPI_Recv(mensaje, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &status);
        printf("Ultimo proceso (Proceso %d), recibiendo del proceso %d el valor i: %d\n", rank, origen, mensaje[0]);
        // Incrementamos el mensaje
        mensaje[0]++;
        // Enviamos el dato al nodo 0
        MPI_Send(mensaje, 1, MPI_INT, destino, tag, MPI_COMM_WORLD);
        printf("Ultimo proceso (Proceso %d), enviando al proceso %d el valor i: %d\n\n", rank, destino, mensaje[0]);
    
    // Cualquiera de los demás nodos
    }else{
        // Debe enviar el dato al siguiente nodo
        destino = rank+1;
        // Debe recibir el dato del nodo anterior
        origen = rank-1;

        // Recibimos mensaje del nodo anterior
        MPI_Recv(mensaje, 1, MPI_INT, origen, tag, MPI_COMM_WORLD, &status);
        printf("Proceso %d, recibiendo del proceso %d el valor i: %d\n", rank, origen, mensaje[0]);
        // Incrementamos el mensaje
        mensaje[0]++;
        // Mandamos mensaje al siguiente nodo
        MPI_Send(mensaje, 1, MPI_INT, destino, tag, MPI_COMM_WORLD);
        printf("Proceso %d, enviando al proceso %d el valor i: %d\n\n", rank, destino, mensaje[0]);
    }
    

    MPI_Finalize();
    
    return 0;
}
