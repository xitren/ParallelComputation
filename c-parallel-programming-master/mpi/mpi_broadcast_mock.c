#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* implementation of MPI_Bcast using MPI_Send and MPI_Recv */

int MY_Bcast(void *buffer, int count, MPI_Datatype datatype, int root,
             MPI_Comm comm) {
  int comm_rank, comm_size;
  MPI_Comm_rank(comm, &comm_rank);
  MPI_Comm_size(comm, &comm_size);
  if(comm_rank == root) {
    int i = comm_size - 1;
    while(i >= 0) {
      if(i != root){
        MPI_Send(buffer, count, datatype, i, 0, comm);
      }
      i--;
    }
  } else {
    MPI_Recv(buffer, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
  }
  return MPI_SUCCESS;
}

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
    MY_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    printf("Total time: %f\n", (MPI_Wtime()-t1));

  } else {
    MY_Bcast(&number, 1, MPI_INT, 0, MPI_COMM_WORLD);
    printf("Process %d received number %d from process 0\n", world_rank, number);
    MPI_Barrier(MPI_COMM_WORLD);
  }

  MPI_Finalize();
}
