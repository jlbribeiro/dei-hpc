/*
 * High Performance Computing
 * June 2013
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

#include "inc/utils.h"

int degree, n_tests;
int *polynomial;
double *tests;

void show_polynomial()
{
	int i;
	for (i = 0; i < degree; ++i)
	{
		if (polynomial[i])
		{
			if (polynomial[i] > 0)
			{	
				if (i > 0)
					printf("+ ");
			}
			else
				printf("- ");
			
			if (polynomial[i] != 1 || i == degree - 1)
					printf("%d", abs(polynomial[i]));
	
			if (degree - 1 - i == 1)
				printf("x");
			else if (degree - 1 - i > 0)
				printf("x^%d", degree - 1 - i);
			printf(" ");
		}
	}
	printf("\n");
}

void read_input()
{	
	int i;

	scanf("%d", &degree);
	degree++;
	polynomial = (int*) malloc (degree * sizeof(int));

	for (i = 0; i < degree; i++)
		scanf("%d", &polynomial[i]);

	scanf("%d", &n_tests);
	tests = (double*) malloc (n_tests * sizeof(double));

	for (i = 0; i < n_tests; i++)
		scanf("%lf", &tests[i]);
}

void naive_approach(FILE *f)
{
	int i, j;
	double result = 0;

	for (i = 0; i < n_tests; i++, result = 0)
	{
		for (j = 0; j < degree; j++) 
			result += polynomial[j] * pow(tests[i], degree - j - 1);

		fprintf(f, "%lf\n", result);
	}
}

void horner_approach(FILE *f)
{
	int i, j;
	double result = 0;

	for (i = 0; i < n_tests; i++, result = 0)
	{
		#pragma omp parallel for private(j), reduction (+: result)
		for (j = 0; j < degree; j++) 
			result = result * tests[i] + polynomial[j];

		fprintf(f, "%lf\n", result);
	}
}

void prefix_approach(FILE *f)
{
	int i, j;
	double result = 0;

	int *aux = (int*) malloc (degree * sizeof(int));
	
	aux[0] = 0;
	for (i = 1; i < degree; i++)
		aux[i] = aux[i-1] + polynomial[i];

	for (i = 0; i < degree; i++)
		printf("%d ", aux[i]);
	printf("\n");

	free(aux);
}

void evaluate(char *approach)
{
	FILE *f;
	char filename[36];
	struct timeval start, end;
	
	sprintf(filename, "output/%s_approach", approach);
	printf("Starting: %s Approach\n", approach);
	
	f = fopen(filename, "w");

	if (!strcmp("Naïve", approach))
	{	
		START_CHRONO;
		naive_approach(f);
		STOP_CHRONO;
	}

	else if (!strcmp("Horner", approach))
	{	
		START_CHRONO;
		horner_approach(f);
		STOP_CHRONO;
	}

	else if (!strcmp("Prefix", approach))
	{	
		START_CHRONO;
		prefix_approach(f);
		STOP_CHRONO;
	}

	printf("%s approach got %ld miliseconds to perfom %d tests in a polynomial with %d degrees!\n", approach, GET_CHRONO, n_tests, degree - 1);		
	fclose(f);
}

int main(int argc, char** argv)
{
	read_input();

	// show_polynomial();

	evaluate("Naïve");
	evaluate("Horner");
	evaluate("Prefix");

	free(polynomial);
	free(tests);

	return 0;
}
