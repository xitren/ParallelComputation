#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* example usage of build-in MPI_Allgather */

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int *buf = (int*) malloc(8 * sizeof(int));
  int *buf2 = (int*) malloc(8 * sizeof(int));
  int i = 0;
  while(i<8) {
    buf2[i] = world_rank*8 + i;
    buf[i] = world_rank*8 + i;
    i++;
  }

  double t1 = MPI_Wtime();

  printf("proc %d before: %d:%d:%d:%d:%d:%d:%d:%d\n", world_rank, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);

  MPI_Allgather(&(buf2[world_rank*2]), 2, MPI_INT, buf, 2, MPI_INT, MPI_COMM_WORLD);

  printf("proc %d after: %d:%d:%d:%d:%d:%d:%d:%d\n", world_rank, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);

  free(buf);
  free(buf2);

  if(world_rank == 0)
   printf("total time: %f\n", (MPI_Wtime()-t1));

  MPI_Finalize();
}
