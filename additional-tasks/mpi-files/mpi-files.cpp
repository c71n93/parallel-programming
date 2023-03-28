#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  int my_rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_File file;
  MPI_File_open(MPI_COMM_WORLD, "output.txt", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

  MPI_File_seek(file, 0, MPI_SEEK_END);
  char* buf = (char*)calloc(sizeof(int), 1);
  sprintf(buf, "%.2d, ", my_rank);
  MPI_File_write_ordered(file, buf, sizeof(int), MPI_CHAR, MPI_STATUS_IGNORE);
  free(buf);
  MPI_File_close(&file);

  MPI_Finalize();

  return 0;
}