#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

/* implementation of MPI_Allgather using MPI_Send and MPI_Recv only */

int MY_Allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                 void *recvbuf, int recvcount, MPI_Datatype recvtype,
                 MPI_Comm comm) {
  int comm_rank, comm_size;
  MPI_Comm_rank(comm, &comm_rank);
  MPI_Comm_size(comm, &comm_size);

  int sendbufsize;
  MPI_Type_size(sendtype, &sendbufsize);
  sendbufsize *= sendcount;

  int i = 0;

  while(i < comm_rank) {
    MPI_Recv(recvbuf + (i*sendbufsize), recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
    i++;
  }

  i = 0;

  while(i < comm_size) {
    if(i != comm_rank)
      MPI_Send(sendbuf, sendcount, sendtype, i, 0, comm);
    i++;
  }

  i = comm_rank + 1;
  while(i < comm_size) {
    MPI_Recv(recvbuf + (i*sendbufsize), recvcount, recvtype, i, 0, comm, MPI_STATUS_IGNORE);
    i++;
  }

  memcpy(recvbuf + (comm_rank*sendbufsize), sendbuf, sendbufsize);

  return MPI_SUCCESS;
          
}

int main(int argc, char** argv) {
  MPI_Init(NULL, NULL);
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int *buf = (int*) malloc(8 * sizeof(int));
  int *buf2 = (int*) malloc(8 * sizeof(int));
  int i = 0;
  while(i<8) {
    buf[i] = world_rank*8 + i;
    buf2[i] = world_rank*8 + i;
    i++;
  }

  double t1 = MPI_Wtime();

  printf("proc %d before: %d:%d:%d:%d:%d:%d:%d:%d\n", world_rank, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);

  MY_Allgather(&(buf2[world_rank*2]), 2, MPI_INT, buf, 2, MPI_INT, MPI_COMM_WORLD);

  printf("proc %d after: %d:%d:%d:%d:%d:%d:%d:%d\n", world_rank, buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);

  free(buf);

  MPI_Barrier(MPI_COMM_WORLD);

  if(world_rank == 0)  
    printf("total time: %f\n", (MPI_Wtime()-t1));

  MPI_Finalize();
}
