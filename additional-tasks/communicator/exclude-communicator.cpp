#include <mpi.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    if (argc != 2) {
        printf("Wrong number of arguments\n");
        exit(-1);
    }
    int N = atoi(argv[1]);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    int world_size = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_Group world_grp = 0;
    MPI_Comm_group(MPI_COMM_WORLD, &world_grp);

    MPI_Group excl1_grp = 0;
    int ranks[1] = {0};
    MPI_Group_excl(world_grp, 1, ranks, &excl1_grp);

    MPI_Comm excl1_comm;
    MPI_Comm_create(MPI_COMM_WORLD, excl1_grp, &excl1_comm);

    if (excl1_comm == MPI_COMM_NULL) {
        printf("excl1_comm == MPI_COMM_NULL, exit\n");
        MPI_Finalize();
        return 0;
    }

    int my_local_rank;
    MPI_Comm_rank(excl1_comm, &my_local_rank);
    int excl1_size = 0;
    MPI_Comm_size(excl1_comm, &excl1_size);
    
    printf("Hello from process %d out of %d\n", my_local_rank, excl1_size);

    int root_rank = 0;
    int n_of_counting_processes = excl1_size;
    double partial_sum = 0;
    for (int n = my_rank; n <= N; n += n_of_counting_processes) {
      partial_sum += 1.0 / n;
    }
    printf("%d: partial_sum = %f\n", my_local_rank, partial_sum);
    double result = 0;
    MPI_Reduce(&partial_sum, &result, 1, MPI_DOUBLE, MPI_SUM, root_rank, excl1_comm);
    if (my_local_rank == root_rank) {
        printf("result = %lf\n", result);
    }

    MPI_Finalize();
    return 0;
}
