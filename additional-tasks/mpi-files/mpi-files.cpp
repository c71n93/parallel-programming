#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int my_rank, world_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  MPI_File file;
  MPI_File_open(MPI_COMM_WORLD, "output.txt", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

  int buff_size = sizeof(int);
  char* buf = (char*)calloc(buff_size, 1);
  sprintf(buf, "%.2d, ", my_rank);
  int offset = buff_size * (world_size - my_rank - 1);
  MPI_File_write_at(file, offset, buf, buff_size, MPI_CHAR, MPI_STATUS_IGNORE);
  free(buf);
  MPI_File_close(&file);

  MPI_Finalize();

  return 0;
}