#include <mpi.h>
#include <iostream>

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);
  int comm_size;
  MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  
  int var = 0;

  if(my_rank == 0) {
    std::cout << "before circulation: var = " << var << std::endl;
    MPI_Send(&var, 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
    int msg;
    MPI_Recv(&msg, 1, MPI_INT, comm_size - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << "after circulation: var = " << msg << std::endl;
  } else {
    int msg;
    MPI_Recv(&msg, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    std::cout << my_rank << ": recv: msg = " << msg << std::endl;
    int inc_msg = msg + 1;
    int next = my_rank % (comm_size - 1) + !!(my_rank % (comm_size - 1));
    std::cout << my_rank << ": send: msg = " << inc_msg << " to: "<< next << std::endl;
    MPI_Send(&inc_msg, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
  }

  MPI_Finalize();
	return 0;
}
