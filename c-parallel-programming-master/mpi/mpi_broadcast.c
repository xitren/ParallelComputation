#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* example usage of MPI_Bcast */

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int number;
  if (world_rank == 0) {

    double t1 = MPI_Wtime();

    number = 1;
    MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Total time: %f\n", (MPI_Wtime()-t1));

  } else {
    MPI_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received number %d from process 0\n", world_rank, number);
    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
