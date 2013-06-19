/*
 * High Performance Computing
 * June 2013
 */

#include <stdio.h>
#include <stdlib.h>

int degree, n_tests;
int *polynomial, *tests;

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
				{
					printf("+ ");
				}	
				printf("%d", polynomial[i]);
			}
			else
			{
				printf("- %d", polynomial[i] * -1);
			}
			
			if (degree - 1 - i == 1)
			{
				printf("x");
			}
			else if (degree - 1 - i > 0)
			{
				printf("x^%d", degree - 1 - i);
			}
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
	tests = (int*) malloc (n_tests * sizeof(int));

	for (i = 0; i < n_tests; i++)
		scanf("%d", &tests[i]);
}

int main(int argc, char** argv)
{
	read_input();

	show_polynomial();

	return 0;
}
