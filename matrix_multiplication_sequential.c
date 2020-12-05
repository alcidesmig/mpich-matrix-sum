#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define MATRIX_SIZE (long int) 25960

int main(int argc, char** argv) {
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

	for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {
		matrix_c[i] = matrix_a[i] + matrix_b[i];
	}

	clock_gettime(CLOCK_REALTIME, &t_end);

	etime = (t_end.tv_sec + t_end.tv_nsec / 1000000000.) -
	        (t_start.tv_sec + t_start.tv_nsec / 1000000000.);
	printf("\n\nTime spent: %lf\n\n", etime);

}
