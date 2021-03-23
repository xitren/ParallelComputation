#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

/* bucket sort using multiple threads
   first argument - problem size
   second argument - number of buckets
   third argument - number of threads */

int main (int argc, char *argv[]) 
{
	uint32_t seed = (unsigned int)time(NULL);
	srand (seed);

	int N = atoi(argv[1]);
	//int buckets_per_thread = atoi(argv[2]);
	int threads_count;
	int buckets_count;

	double *tab;
	double **buckets;
	int *bucket_sizes;

	double start;
	double end;

	omp_set_num_threads(atoi(argv[3]));

	#pragma omp parallel
	{
		int i;

		#pragma omp single
		{
			threads_count = omp_get_num_threads();
			buckets_count = atoi(argv[2]);

			tab = malloc(sizeof(double)*N);
			buckets = malloc(sizeof(double*) * buckets_count);
			bucket_sizes = malloc(sizeof(int) * buckets_count);

			printf("Threads: %d, Buckets: %d\n ", threads_count, buckets_count);

			for (i = 0; i < buckets_count; i++)
			{
				buckets[i] = malloc(sizeof(double) * N);
				bucket_sizes[i] = 0;
			}

			for (i = 0; i < N; i++)
				tab[i] = ((double)rand()/(double)(RAND_MAX));

			start = omp_get_wtime();
		}

		#pragma omp for schedule(static)
		for (i = 0; i < buckets_count; i++) 
		{
			//printf(" - filling %d by %d\n", i, omp_get_thread_num());
			int j;
			for (j = 0; j < N; j++)
			{
				int bucket_num = (int)(tab[j]*buckets_count);
				if (bucket_num == i)
					buckets[bucket_num][bucket_sizes[bucket_num]++] = tab[j];
			}
		}

		#pragma omp for schedule(static)
		for (i = 0; i < buckets_count; i++) 
		{
			//printf(" - sorting %d by %d\n", i, omp_get_thread_num());
			int bucket_start_index = 0;
			int j, k;
			for (j = 0; j < i; j++)
				bucket_start_index += bucket_sizes[j];
			
			for (j = 0; j < bucket_sizes[i]; j++)
			{
				for (k = 0; k < bucket_sizes[i] - 1; k++)
				{
					if (buckets[i][k] > buckets[i][k + 1])
					{
						double tmp = buckets[i][k];
						buckets[i][k] = buckets[i][k + 1];
						buckets[i][k + 1] = tmp;
					}
				}
			}

			for (j = 0; j < bucket_sizes[i]; j++)
				tab[bucket_start_index + j] = buckets[i][j];
		}
	
		#pragma omp single
		{
			end = omp_get_wtime();
			
			free(tab);

			for (i = 0; i < buckets_count; i++)
			{
				free(buckets[i]);
			}

			free(buckets);

			printf(" - total time of sorting: %f\n", end-start);
		}
	}

	return 0;
}

