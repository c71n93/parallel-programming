#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>

void sentLongMsg(int my_rank, long int msg_size) {
  char* long_msg = (char*)malloc(msg_size);
  if (my_rank == 0) {
    auto start = std::chrono::high_resolution_clock::now();
    MPI_Send(long_msg, msg_size, MPI_BYTE, my_rank + 1, 0, MPI_COMM_WORLD);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    printf("%d: msg sent long in %lldms; msg_size = %ld\n", my_rank, duration.count(), msg_size);
  } else if (my_rank == 1) {
    sleep(1);
    char* msg = (char*)malloc(msg_size);
    MPI_Recv(msg, msg_size, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d: msg received; msg_size = %ld\n\n", my_rank, msg_size);
  }
  free(long_msg);
}

void ssentLongMsg(int my_rank, long int msg_size) {
  char* long_msg = (char*)malloc(msg_size);
  if (my_rank == 0) {
    auto start = std::chrono::high_resolution_clock::now();
    MPI_Ssend(long_msg, msg_size, MPI_BYTE, my_rank + 1, 0, MPI_COMM_WORLD);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    printf("%d: msg Ssent long in %lldms; msg_size = %ld\n", my_rank, duration.count(), msg_size);
  } else if (my_rank == 1) {
    sleep(1);
    char* msg = (char*)malloc(msg_size);
    MPI_Recv(msg, msg_size, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d: msg S received; msg_size = %ld\n\n", my_rank, msg_size);
  }
  free(long_msg);
}

void rsentLongMsg(int my_rank, long int msg_size) {
  char* long_msg = (char*)malloc(msg_size);
  if (my_rank == 0) {
    auto start = std::chrono::high_resolution_clock::now();
    MPI_Rsend(long_msg, msg_size, MPI_BYTE, my_rank + 1, 0, MPI_COMM_WORLD);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    printf("%d: msg Rsent long in %lldms; msg_size = %ld\n", my_rank, duration.count(), msg_size);
  } else if (my_rank == 1) {
    sleep(1);
    char* msg = (char*)malloc(msg_size);
    MPI_Recv(msg, msg_size, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d: msg R received; msg_size = %ld\n\n", my_rank, msg_size);
  }
  free(long_msg);
}

void bsentLongMsg(int my_rank, long int msg_size) {
  char* long_msg = (char*)malloc(msg_size);
  if (my_rank == 0) {
    auto start = std::chrono::high_resolution_clock::now();
    MPI_Bsend(long_msg, msg_size, MPI_BYTE, my_rank + 1, 0, MPI_COMM_WORLD);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    printf("%d: msg Bsent long in %lldms; msg_size = %ld\n", my_rank, duration.count(), msg_size);
  } else if (my_rank == 1) {
    sleep(1);
    char* msg = (char*)malloc(msg_size);
    MPI_Recv(msg, msg_size, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("%d: msg B received; msg_size = %ld\n\n", my_rank, msg_size);
  }
  free(long_msg);
}

int main(int argc, char *argv[]) {
  MPI_Init(&argc, &argv);

  int my_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  int world_size = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  long msg_size = 100000;

  sentLongMsg(my_rank, msg_size);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  msg_size = 100;
  sentLongMsg(my_rank, msg_size);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  msg_size = 100;
  ssentLongMsg(my_rank, msg_size);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  msg_size = 100000;
  rsentLongMsg(my_rank, msg_size);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  msg_size = 100;
  rsentLongMsg(my_rank, msg_size);
  MPI_Barrier(MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  msg_size = 1000;
  bsentLongMsg(my_rank, msg_size);
  MPI_Barrier(MPI_COMM_WORLD);

	MPI_Finalize();
	return 0;
}
