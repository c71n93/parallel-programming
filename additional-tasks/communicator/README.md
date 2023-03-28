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
Hello from process 0 out of 7
0: partial_sum = 1.125000
Hello from process 1 out of 7
1: partial_sum = 0.611111
Hello from process 2 out of 7
2: partial_sum = 0.433333
Hello from process 3 out of 7
3: partial_sum = 0.250000
Hello from process 4 out of 7
4: partial_sum = 0.200000
Hello from process 5 out of 7
5: partial_sum = 0.166667
Hello from process 6 out of 7
6: partial_sum = 0.142857
result = 2.928968
```