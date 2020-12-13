## MPI
Simple task for the discipline of parallel and distributed programming. Objective: sum of matrices using communication between nodes with MPI.
Three implementations were made, one sending the matrices to be added individually and one sending them together (interspersed in another matrix) both using Isend, and the last using Scatter/Gather, sending the matrices separated.

### MPI
```
export NUM_NODES=8
make mpi
make run_mpi
```

### Sequential
```
make seq
make run_seq
```

### Comparisons (using matrix_sum_mpi.c and matrix_sum_mpi_isend.c)

For Isend: 8 nodes = 1 (control) + 7 (process) nodes
For Scatter/Gather (S/G): 8 nodes = 8 (process) nodes (control used to process data too)

These tests were performed on a single machine with 16 cores. The results do not imply in inefficiency in the use of the MPI since the objective of it goes beyond the processing of data in a single machine.

__Matrix size = 51920x51920__

Non-feasible time

__Matrix size = 25960x25960__
- MPI (8 nodes, Isend): 1.643992s
- MPI (8 nodes, S/G):   2.637622s
- MPI (4 nodes, Isend): 2.491534s
- MPI (4 nodes, S/G):   2.867721s
- Sequential: 1.585778s

__Matrix size = 12980x12980__
- MPI (8 nodes, Isend): 0.487209s
- MPI (8 nodes, S/G):   0.647834s
- MPI (4 nodes, Isend): 0.633036s
- MPI (4 nodes, S/G):   0.759742s
- Sequential: 0.395317s

__Matrix size = 5192x5192__
- MPI (8 nodes, Isend): 0.087036s
- MPI (8 nodes, S/G):   0.116285
- MPI (4 nodes, Isend): 0.104057s
- MPI (4 nodes, S/G):   0.130446s
- Sequential: 0.064066s
