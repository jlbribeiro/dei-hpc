#include <stdio.h>
#include <stdlib.h>

#include "inc/input.h"
#include "inc/utils.h"

int cmp_rules(const void *a, const void *b)
{
	int i;
	int diff;

	int *at = (int *)a;
	int *bt = (int *)b;

	for (i = 0; i < RULES_LEN; i++)
	{
		diff = at[i] - bt[i];

		if (diff != 0)
			return diff;

	}

	return 0;
}

int read_rules(char *filename)
{
	struct timeval start, end;

	int i = 0;

	FILE *f = fopen(filename, "r");

	START_CHRONO;
	while (	fscanf(f, "%d,", &rules[i][0]) != EOF )
	{
		fscanf(f, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &rules[i][1], &rules[i][2],
													&rules[i][3], &rules[i][4],
													&rules[i][5], &rules[i][6],
													&rules[i][7], &rules[i][8],
													&rules[i][9], &rules[i][10]);
		
		i++;
	}
	STOP_CHRONO;

	fclose(f);

	#ifdef DEBUG_PRINT
		fprintf(stderr, "Took %ld ms to read %d rules!\n", GET_CHRONO, i);
	#endif

	return i;
}

int read_transanctions(char *filename)
{
	struct timeval start, end;

	int i = 0;
	FILE *f = fopen(filename, "r");

	START_CHRONO;
	while (	fscanf(f, "%d,", &transactions[i][0]) != EOF )
	{
		fscanf(f, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d", &transactions[i][1], &transactions[i][2],
													&transactions[i][3], &transactions[i][4],
													&transactions[i][5], &transactions[i][6],
													&transactions[i][7], &transactions[i][8],
													&transactions[i][9], &transactions[i][10]);
		
		i++;
	}
	STOP_CHRONO;

	fclose(f);

	#ifdef DEBUG_PRINT
		fprintf(stderr, "Took %ld ms to read %d transactions!\n", GET_CHRONO, i);
	#endif

	return i;
}

void sort_rules()
{
	struct timeval start, end;

	START_CHRONO;
	qsort(rules, n_rules, sizeof(int) * RULES_LEN, cmp_rules);
	STOP_CHRONO;

	#ifdef DEBUG_PRINT
		fprintf(stderr, "Took %ld ms to sort %d rules!\n", GET_CHRONO, n_rules);
	#endif
}
