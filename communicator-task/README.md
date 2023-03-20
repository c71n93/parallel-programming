# Creating a custom MPI communicator
### The program creates a custom communicator and compute the sum of N terms of harmonic series.
## Running
### From project directory run
```
$ mpicc exclude-communicator.cpp
$ mpirun -np <number of processes> ./a.out <N>
```
### The result will be as follows:
```
excl1_comm == MPI_COMM_NULL, exit
Hello from process 0 out of 3
0: partial_sum = 1.250000
Hello from process 2 out of 3
2: partial_sum = 0.333333
Hello from process 1 out of 3
1: partial_sum = 0.700000
result = 2.283333
```