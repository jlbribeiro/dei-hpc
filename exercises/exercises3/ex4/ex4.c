/*
 * High Performance Computing
 * June 2013
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
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

void naive_approach()
{
	int i, j;
	double result = 0;

	for (i = 0; i < n_tests; i++, result = 0)
	{
		for (j = 0; j < degree; j++) 
			result += polynomial[j] * pow(tests[i], degree - j - 1);

		// printf("%lf\n", result);
	}
}

void horner_approach()
{
	int i, j;
	double result = 0;

	for (i = 0; i < n_tests; i++)
	{
		result = 0;
		for (j = 0; j < degree; j++) 
			result = result * tests[i] + polynomial[j];

		// printf("%lf\n", result);
	}
}

int main(int argc, char** argv)
{
	struct timeval start, end;

	read_input();

	// show_polynomial();


	printf("Starting: Naïve Approach\n");
	START_CHRONO;
	naive_approach();
	STOP_CHRONO;
	printf("Naïve approach got %ld miliseconds to perfom %d tests in a polynomial with %d degrees!\n", GET_CHRONO, n_tests, degree - 1);

	printf("Starting: Horner Approach\n");
	START_CHRONO;
	horner_approach();
	STOP_CHRONO;
	printf("Horner approach got %ld miliseconds to perfom %d tests in a polynomial with %d degrees!\n", GET_CHRONO, n_tests, degree - 1);
	
	return 0;
}
