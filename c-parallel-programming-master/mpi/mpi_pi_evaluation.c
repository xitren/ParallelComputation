#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <inttypes.h>

/* PI evaluation using Monte-Carlo method 
   first argument - problem size */

int main (int argc, char *argv[])
{
	MPI_Init(NULL, NULL);

	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	MPI_Barrier(MPI_COMM_WORLD);

	double t1 = MPI_Wtime();

	uint64_t n_total = atoi(argv[1]);
	uint64_t n_in_circle = 0;

	uint32_t seed = (unsigned int)time(NULL) + world_rank;
	srand (seed);

	uint64_t n = n_total;
	while (--n)
	{
		double rnd_x = ((double)rand()/(double)(RAND_MAX));
		double rnd_y = ((double)rand()/(double)(RAND_MAX));
		if ((rnd_x*rnd_x + rnd_y*rnd_y) < 1)
			n_in_circle++;
	}

	uint64_t n_recv;

	MPI_Reduce(&n_in_circle, &n_recv, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	if (world_rank == 0)
	{
		double pi = 4*((double)n_recv)/((double)(n_total*world_size));
		printf("n = %d, n_proc = %d, pi ~ %lf, time = %f\n", n_total, world_size, pi, (MPI_Wtime()-t1));
	}

	MPI_Finalize();
}
