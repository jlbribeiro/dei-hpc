/*
 * High Performance Computing
 * April 2013
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 5
#define DIFF 0.01

double matrix[2][N * N];
int now = 0;

void show_matrix()
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%.3lf ", matrix[now][i * N + j]);
		}
		printf("\n");
	}
}

void worker(double outside)
{
	int i, j, step;
	double diff = 2 * DIFF;

	memset(matrix, 0, 2 * N * N * sizeof(double));

	#pragma omp parallel for private(i)
	for (i = 0; i < N; i++)
	{
		matrix[now][i] = matrix[now][(N-1) * N + i] = matrix[now][i * N] = matrix[now][i * N + (N-1)] = outside;
		matrix[!now][i] = matrix[!now][(N-1) * N + i] = matrix[!now][i * N] = matrix[!now][i * N + (N-1)] = outside;
	}

	#pragma omp barrier
	
	for (step = 0; diff > DIFF ; step++, now = !now)
	{
		diff = 0;

		#pragma omp parallel for private(i, j), reduction (+: diff)
		for (i = 1; i < N-1; i++)
		{
			for (j = 1; j < N-1; j++)
			{
				matrix[now][i * N + j] = 
				(
					matrix[!now][(i-1) * N + j] + 
					matrix[!now][(i+1) * N + j] + 
					matrix[!now][i * N + (j-1)] + 
					matrix[!now][i * N + (j+1)]
				) / 4;

				diff += fabs(matrix[now][i * N + j] - matrix[!now][i * N + j]);
			}
		}
	}

	printf("Took %d steps\n", step);
	show_matrix();
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("wrong usage!\n");
		printf("%s <outside temperature>\n", argv[0]);
		exit(0);
	}

	worker(atof(argv[1]));

	return 0;
}
