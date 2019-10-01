#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define COLS_A 5
#define ROWS_A 5

#define COLS_B 2
#define ROWS_B 5

// Declarations of functions
int** reserveMemory(int rows, int columns);
void freeMatrix(int** matrix, int rows);

int main(int argc, char *argv[]){
	// matrix declaration
	int** matrixA;
	int** matrixB;
	int** matrixC;

	// Convert matrix A to vector
	int sendVector[ROWS_A * COLS_A];

	int rows_c, cols_c, elems_per_proc;
	// int a[5][5]={{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5},{1,2,3,4,5}};
	// int b[5][2]={{2,2},{2,2},{2,2},{2,2},{2,2}};
	// int c[5][2]={{0,0},{0,0},{0,0},{0,0},{0,0}};

	int i,j,k, rank, size;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Each row will be assigned to one process
	int remainder = ROWS_A / size;
	printf("remainder: %d\n", remainder);

	if (rank == 0) {
		// Reserve memory for each matrix
		printf("rank: %d\n", rank);
		matrixA = reserveMemory(ROWS_A, COLS_A);
		matrixB = reserveMemory(ROWS_B, COLS_B);

		// A (2 x 3) * B (3 x 4) = C (2 x 4)
		rows_c = ROWS_A;
		cols_c = COLS_B;	
		matrixC = reserveMemory(rows_c, cols_c);

		// Filling matrix A
		for (int i = 0; i < ROWS_A; i++)
			for (int j = 0; j < COLS_A; j++)
				matrixA[i][j] = j + 1;

		// Filling matrix B
		for (int i = 0; i < ROWS_B; i++)
			for (int j = 0; j < COLS_B; j++)
				matrixB[i][j] = j + 1;

		printf("MATRICES: \n\n");
		printf("Matriz A: \n");
		for (int i = 0; i < ROWS_A; i++){
			for (int j = 0; j < COLS_A; j++)
				printf("%d ", matrixA[i][j]);
			printf("\n");
		}

		printf("Matriz B: \n");
		for (int i = 0; i < ROWS_B; i++){
			for (int j = 0; j < COLS_B; j++)
				printf("%d ", matrixB[i][j]);
			printf("\n");
		}

		// Filling vector from matrix A
		for (int i = 0; i < ROWS_A; i++)
			for (int j = 0; j < COLS_A; j++)
				sendVector[(i* COLS_A) + j] = matrixA[i][j];

		printf("Vector: \n");
		for (int i = 0; i < ROWS_A * COLS_A; i++)
			printf("%d ", sendVector[i]);
		printf("\n");
		
		printf("%s\n", "Sending B by broadcast...");
	} 

	MPI_Bcast(matrixB, COLS_B * ROWS_B, 
		MPI_INT, 0, MPI_COMM_WORLD);
	elems_per_proc = COLS_A;
	MPI_Bcast(&elems_per_proc, 1, 
		MPI_INT, 0, MPI_COMM_WORLD);

	//Number of elements in row
	int* local_row = (int* )malloc(sizeof(int) * elems_per_proc);

	// for (int i = 0; i < elems_per_proc; ++i)
	// {
	// 	local_row[i] = matrixA[rank][i];
	// }

  // Scatter each row from the root process to all processes in MPI world!
	
	MPI_Scatter(sendVector, elems_per_proc, MPI_INT, local_row,
		elems_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	// Initializing with zeros
	int* sub_column_result = (int* )calloc(COLS_B, sizeof(int));
	// int sub_column_result[2] = {0};

	// Performing sub multiplication
	for (int i = 0; i < COLS_B; i++) 
	{
		for (int k = 0; k < elems_per_proc; ++k)
		{
			sub_column_result[i] += local_row[k] * matrixB[k][i];
		}
	} 

	// Showing results of sub_multiplication
	for(i = 0; i < COLS_B; i++){
		printf("casilla[%d][%d]=%d\t", rank, i, sub_column_result[i]);
		printf("\n");
	}

	MPI_Gather(&sub_column_result, elems_per_proc, MPI_INT, 
		matrixC, elems_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	// MPI_Barrier(MPI_COMM_WORLD)

	if (rank == 0) {
		//Printing matrix C
		printf("%s\n", "Resultado: ");
		for (int i = 0; i < rows_c; ++i)
		{
			for (int j = 0; j < cols_c; ++j)
			{
				printf("c[%d][%d] = %d\n", i, j, matrixC[i][j]);
			}
		}
	}
	if (rank == 0) {
		freeMatrix(matrixA, ROWS_A);
		freeMatrix(matrixB, ROWS_B);
		freeMatrix(matrixC, rows_c);
	} 
	free(sub_column_result);
	MPI_Finalize();	

	return 0;
}

// Implementation of functions

int** reserveMemory(int rows, int columns){

	int **matrix;
	matrix = (int**) calloc(rows, sizeof(int *));

	for (int i = 0; i < rows; i++)
		matrix[i] = (int *) calloc(columns, sizeof(int));

	// Print matrix test
	printf("Matrix in reserve: \n");
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
	
	return matrix;
}

void freeMatrix(int** matrix, int rows){
	for (int i = 0; i < rows; i++)
		free(matrix[i]);
	
	free(matrix);
}
