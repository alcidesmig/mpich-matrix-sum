#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define MATRIX_SIZE 12980

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);

	int myrank, w_size, flag;

	MPI_Comm_rank( MPI_COMM_WORLD, &myrank );
	MPI_Comm_size( MPI_COMM_WORLD, &w_size );

	int part_size_for_node = MATRIX_SIZE / (w_size - 1);

	MPI_Status status;
	MPI_Request * request_a = (MPI_Request *) malloc(sizeof(MPI_Request) * (w_size - 1));
	MPI_Request * request_b = (MPI_Request *) malloc(sizeof(MPI_Request) * (w_size - 1));
	MPI_Request * request_c = (MPI_Request *) malloc(sizeof(MPI_Request) * (w_size - 1));

	if (myrank == 0) {
		float etime;
		struct timespec t_start, t_end;
		int * matrix_a = (int *) malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE);
		int * matrix_b = (int *) malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE);
		int * matrix_c = (int *) malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE);
		for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
			matrix_a[i] = rand() % 50;
			matrix_b[i] = rand() % 50;
		}
		clock_gettime(CLOCK_REALTIME, &t_start);
		for (int i = 1; i < w_size; i++) {
			printf("Sending for %d\n", i);
			MPI_Isend((matrix_a + ((i - 1)*part_size_for_node * MATRIX_SIZE)), part_size_for_node * MATRIX_SIZE, MPI_INT, i, i, MPI_COMM_WORLD, &request_a[i - 1]);
			MPI_Isend((matrix_b + ((i - 1)*part_size_for_node * MATRIX_SIZE)), part_size_for_node * MATRIX_SIZE, MPI_INT, i, i * 1024, MPI_COMM_WORLD, &request_b[i - 1]);
		}
		for (int i = 1; i < w_size; i++) {
			MPI_Wait(&request_a[i - 1], &status);
			MPI_Test(&request_a[i - 1], &flag, &status);
			if (!flag) {
				printf("Error on sending a to %d", i);
			}
			MPI_Wait(&request_b[i - 1], &status);
			MPI_Test(&request_b[i - 1], &flag, &status);
			if (!flag) {
				printf("Error on sending b to %d", i);
			}
		}
		printf("Sent to all nodes\n");
		for (int i = 1; i < w_size; i++) {
			MPI_Irecv((matrix_c + ((i - 1)*part_size_for_node * MATRIX_SIZE)), part_size_for_node * MATRIX_SIZE, MPI_INT, i, i * 1024 * 1024, MPI_COMM_WORLD, &request_c[i - 1]);
		}
		for (int i = 1; i < w_size; i++) {
			MPI_Wait(&request_c[i - 1], &status);
			MPI_Test(&request_c[i - 1], &flag, &status);
			if (!flag) {
				printf("Error on receiving c from %d", i);
			}
		}
		clock_gettime(CLOCK_REALTIME, &t_end);
		printf("Responses received\n");
		// for (int i = 0; i < part_size_for_node * MATRIX_SIZE; i++) {
		// 	if (i % MATRIX_SIZE == 0) printf("\n");
		// 	printf("%3d ", matrix_c[i]);
		// }
		etime = (t_end.tv_sec + t_end.tv_nsec / 1000000000.) -
		        (t_start.tv_sec + t_start.tv_nsec / 1000000000.);
		printf("\n\nTime spent: %lf\n\n", etime);
	} else {
		int number_amount;

		int * lines_a = (int *) malloc(sizeof(int) * part_size_for_node * MATRIX_SIZE);
		int * lines_b = (int *) malloc(sizeof(int) * part_size_for_node * MATRIX_SIZE);
		MPI_Recv(lines_a, part_size_for_node * MATRIX_SIZE, MPI_INT, 0, myrank, MPI_COMM_WORLD,
		         &status);
		MPI_Get_count(&status, MPI_INT, &number_amount);
		if (number_amount != part_size_for_node * MATRIX_SIZE) {
			printf("Error in %d on receiving a from 0", myrank);
		}
		MPI_Recv(lines_b, part_size_for_node * MATRIX_SIZE, MPI_INT, 0, myrank * 1024, MPI_COMM_WORLD,
		         &status);
		MPI_Get_count(&status, MPI_INT, &number_amount);
		if (number_amount != part_size_for_node * MATRIX_SIZE) {
			printf("Error in %d on receiving b from 0", myrank);
		}
		printf("Got data %d!\n", myrank);
		int * lines_result = (int *) malloc(sizeof(int) * part_size_for_node * MATRIX_SIZE);
		for (int i = 0; i < part_size_for_node * MATRIX_SIZE; i++) {
			lines_result[i] = lines_a[i] + lines_b[i];
		}
		MPI_Send(lines_result, part_size_for_node * MATRIX_SIZE, MPI_INT, 0, myrank * 1024 * 1024, MPI_COMM_WORLD);
	}
	MPI_Finalize();

}

