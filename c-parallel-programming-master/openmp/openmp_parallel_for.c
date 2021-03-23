#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/* simple test of parallel for on openmp */

int main (int argc, char *argv[]) 
{
	int n = 7; //shared
	int i; //private

	#pragma omp parallel default(shared) private(i)
	{

		#pragma omp single
		{
			printf("thread_num: %d\n", omp_get_num_threads()); 
		}

		#pragma omp for schedule(static)
		for (i = 0; i < n; i++) 
		{
			printf("iteration: %d, thread: %d\n", i, omp_get_thread_num());
		}

	}

	return 0;
}


