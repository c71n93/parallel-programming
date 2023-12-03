#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int commsize, my_rank;
	// Initialize the MPI environment
	MPI_Init(&argc, &argv);
	// Get the rank of the process
	MPI_Comm_size(MPI_COMM_WORLD, &commsize);
	// Get the rank of the process
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	printf ("Communicator size= %d My rank= %d\n", commsize, my_rank );
	// Finalize the MPI environment.
	MPI_Finalize();
	return 0;
}
