#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define COLS_A 5
#define ROWS_A 5

#define COLS_B 2
#define ROWS_B 5


int allocateMemory(int *** ptrToMatrix, int rows, int columns);

int main(int argc, char *argv[])
{
	int** matrix_a;
	int** matrix_b;
	int** matrix_c;

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

	if (rank == 0) {
		//allocating memory 

		allocateMemory(&matrix_a, ROWS_A, COLS_A);
		allocateMemory(&matrix_b, ROWS_B, COLS_B);
		// A (2 x 3) * B (3 x 4) = C (2 x 4)
		rows_c = ROWS_A;
		cols_c = COLS_B;
		
		allocateMemory(&matrix_c, rows_c, cols_c);

		// Filling matrix A
		for (int i = 0; i < ROWS_A; ++i)
		{
			for (int j = 0; j < COLS_A; ++j)
			{
				matrix_a[i][j] = j + 1;
			}
		}
		// Filling matrix B
		for (int i = 0; i < ROWS_B; ++i)
		{
			for (int j = 0; j < COLS_B; ++j)
			{
				matrix_b[i][j] = j + 1;
			}
		}
		printf("MATRICES: \n");

		for (int i = 0; i < ROWS_A; ++i)
		{
			for (int j = 0; j < COLS_A; ++j)
			{
				printf("%d ", matrix_a[i][j]);
			}
			printf("\n");
		}

		for (int i = 0; i < ROWS_A; ++i)
		{
			for (int j = 0; j < COLS_A; ++j)
			{
				sendVector[(i* COLS_A) + j] = matrix_a[i][j];
			}
		}
		// Filling matrix B
		printf("\n");
		for (int i = 0; i < ROWS_B; ++i)
		{
			for (int j = 0; j < COLS_B; ++j)
			{
				printf("%d ", matrix_b[i][j]);
			}
			printf("\n");
		}
		printf("%s\n", "Sending B by broadcast...");
	} 

	MPI_Bcast(matrix_b, COLS_B * ROWS_B, 
		MPI_INT, 0, MPI_COMM_WORLD);
	elems_per_proc = COLS_A;
	MPI_Bcast(&elems_per_proc, 1, 
		MPI_INT, 0, MPI_COMM_WORLD);

	//Number of elements in row
	int* local_row = (int* )malloc(sizeof(int) * elems_per_proc);

	// for (int i = 0; i < elems_per_proc; ++i)
	// {
	// 	local_row[i] = matrix_a[rank][i];
	// }

  // Scatter each row from the root process to all processes in MPI world!
	
	MPI_Scatter(sendVector, elems_per_proc, MPI_INT, local_row,
		elems_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	// Initializing with zeros
	// int* sub_column_result = (int* )calloc(COLS_B, sizeof(int));
	int sub_column_result[2] = {0};

	// Performing sub multiplication
	for (int i = 0; i < COLS_B; i++) 
	{
		for (int k = 0; k < elems_per_proc; ++k)
		{
			sub_column_result[i] += local_row[k] * matrix_b[k][i];
		}
	} 

	// Showing results of sub_multiplication
	for(i = 0; i < COLS_B; i++){
		printf("casilla[%d][%d]=%d\t", rank, i, sub_column_result[i]);
		printf("\n");
	}

	MPI_Gather(&sub_column_result, elems_per_proc, MPI_INT, 
		matrix_c, elems_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

	// MPI_Barrier(MPI_COMM_WORLD)

	if (rank == 0) {
		//Printing matrix C
		printf("%s\n", "Resultado: ");
		for (int i = 0; i < rows_c; ++i)
		{
			for (int j = 0; j < cols_c; ++j)
			{
				printf("c[%d][%d] = %d\n", i, j, matrix_c[i][j]);
			}
		}
	}
	if (rank == 0) {
		free(matrix_a);
		free(matrix_b);
		free(matrix_c);
	} 
	free(sub_column_result);
	MPI_Finalize();	
	return 0;
}

int allocateMemory(int *** matrix, int rows, int columns){
	*matrix = malloc(sizeof(int *) * rows);
	// printf("Locating %d rows", rows);
	// printf("size %d", sizeof(*matrix));
	for (int i = 0; i < rows; ++i)
	{
		// printf("Locating column %d", i);
		(*matrix)[i] = malloc(sizeof(int) * columns);
		// printf("size %d", sizeof((*matrix)[i]));
	}
}
