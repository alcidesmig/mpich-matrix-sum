mpi:
	mpicc matrix_sum_mpi.c -o out_mpi
run_mpi:
	mpirun -n $(NUM_NODES) ./out_mpi
seq:
	gcc matrix_sum_sequential.c -o out_seq
run_seq:
	./out_seq
