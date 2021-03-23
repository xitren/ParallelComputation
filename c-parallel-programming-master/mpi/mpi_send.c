#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* time measurement for MPI_Send */

int main(int argc, char** argv) {

MPI_Init(NULL, NULL);

int a = 1;
while(a < 256*256*256){

  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int size = a;

  double t1;
  double t2;

  if (world_size < 2) {
    fprintf(stderr, "World size must be greater than 1 for %s\n", argv[0]);
    MPI_Abort(MPI_COMM_WORLD, 1);
  }

  int *data;
  data = malloc((sizeof(int))*size);

  if (world_rank == 0) {
    t1 = MPI_Wtime();
    int b = 500;
    while (b--) {
      MPI_Send(data, size, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(data, size, MPI_INT, 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    t2 = MPI_Wtime();
    printf("%f\n", t2-t1);
  } else if (world_rank == 1) {
    int b = 500;
    while (b--) {
      MPI_Recv(data, size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(data, size, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
  }

  free(data);
  a*=2;
}

MPI_Finalize();

}
