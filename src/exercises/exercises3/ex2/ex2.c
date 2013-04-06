/*
 * High Performance Computing
 * April 2013
 */

#include <omp.h>
#include <stdio.h>

#define SIZE 100000

#define NUMBERS_FILE "numbers.txt"

void read_file(char *filename, float *A, float *B)
{
	FILE *f;
	int i;

	f = fopen(filename, "r");

	for (i = 0; i < SIZE; i++)
		fscanf(f, "%f", &A[i]);
	for (i = 0; i < SIZE; i++)
		fscanf(f, "%f", &B[i]);

	fclose(f);
}

void worker(float *A, float *B, float *C)
{
	int i;

	#pragma omp parallel for
	for (i = 0; i < SIZE; i ++)
		C[i] = A[i] + B[i];
}

int main(int argc, char** argv)
{
	float A[SIZE], B[SIZE], C[SIZE];
	
	read_file(NUMBERS_FILE, A, B);

	worker(A, B, C);
	printf("%f %f %f %f\n", C[0], C[1], C[100000-1], C[100000-2]);

	return 0;
}
