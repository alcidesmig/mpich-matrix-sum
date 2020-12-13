#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// #define PRINT_MATRIX
#define MATRIX_SIZE (long int) 5192


int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	
	srand(time(NULL));
	
	int myrank, w_size, flag;
	float etime;
	struct timespec t_start, t_end;

	MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
	MPI_Comm_size( MPI_COMM_WORLD, &w_size );

	int part_size_for_node = (MATRIX_SIZE) / (w_size);

	int * matrix_a = NULL, * matrix_b = NULL, * matrix_c = NULL;

	if (myrank == 0) {
		matrix_a = (int *) malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE);
		matrix_b = (int *) malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE);
		matrix_c = (int *) malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE);
		for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
			matrix_a[i] = rand() % 50;
			matrix_b[i] = rand() % 50;

		}
#ifdef PRINT_MATRIX
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				printf("%3d ", matrix_a[i * MATRIX_SIZE + j]);
			}
			printf("\n");
		}
		printf("\n");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				printf("%3d ", matrix_b[i * MATRIX_SIZE + j]);
			}
			printf("\n");
		}
#endif
	}
	if (myrank == 0) clock_gettime(CLOCK_REALTIME, &t_start);

	int * matrix_a_line_for_node = (int *) malloc(sizeof(int) * part_size_for_node * MATRIX_SIZE);
	int * matrix_b_line_for_node = (int *) malloc(sizeof(int) * part_size_for_node * MATRIX_SIZE);

	MPI_Scatter(matrix_a, part_size_for_node * MATRIX_SIZE, MPI_INT, matrix_a_line_for_node,
	            part_size_for_node * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter(matrix_b, part_size_for_node * MATRIX_SIZE, MPI_INT, matrix_b_line_for_node,
	            part_size_for_node * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	for (int i = 0; i < part_size_for_node * MATRIX_SIZE; i++) {
		matrix_a_line_for_node[i] = matrix_a_line_for_node[i] + matrix_b_line_for_node[i];
	}
	MPI_Gather(matrix_a_line_for_node, part_size_for_node * MATRIX_SIZE, MPI_INT,
	           matrix_c, part_size_for_node * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

	if (myrank == 0) {
		clock_gettime(CLOCK_REALTIME, &t_end);

#ifdef PRINT_MATRIX
		printf("\n");
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 10; j++) {
				printf("%3d ", matrix_c[i * MATRIX_SIZE + j]);
			}
			printf("\n");
		}
#endif
		etime = (t_end.tv_sec + t_end.tv_nsec / 1000000000.) -
		        (t_start.tv_sec + t_start.tv_nsec / 1000000000.);
		printf("\n\nTime spent: %lf\n\n", etime);
	}


	MPI_Finalize();

}
