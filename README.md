## MPI
Simple task for the discipline of parallel and distributed programming. Objective: sum of matrices using communication between nodes with MPI.
Two implementations were made, sending the matrices to be added individually and then sending them together (interspersed in another matrix).

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

### Comparisons (using matrix_multiplication_mpi.c)
e.g. 8 nodes = 1 (control) + 7 (process) nodes

__Matrix size = 51920x51920__

Non-feasible time

__Matrix size = 25960x25960__
- MPI (8 nodes): 1.643992s
- MPI (4 nodes): 2.491534s
- Sequential: 1.585778s

__Matrix size = 12980x12980__
- MPI (8 nodes): 0.487209s
- MPI (4 nodes): 0.633036s
- Sequential: 0.395317s

__Matrix size = 5192x5192__
- MPI (8 nodes): 0.087036s
- MPI (4 nodes): 0.104057s
- Sequential: 0.064066s
