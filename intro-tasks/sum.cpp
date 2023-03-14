#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[]) {

  if (argc != 2) {
        printf("Wrong number of arguments\n");
        exit(-1);
    }
  int N = atoi(argv[1]);

  MPI_Init(&argc, &argv);
  int comm_size;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  if(my_rank == 0) {
    double partial_sum;
    double sum = 0;
    for (int i = 1; i < comm_size; i++){
        MPI_Recv(&partial_sum, 1, MPI_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        sum += partial_sum;
    }
    std::cout << "sum = " << sum << std::endl;
  } else {
    int n_of_counting_processes = comm_size - 1;
    double partial_sum = 0;
    for (int n = my_rank; n <= N; n += n_of_counting_processes) {
      partial_sum += 1.0 / n;
    }
    MPI_Send(&partial_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
	return 0;
}
