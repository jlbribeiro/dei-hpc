#ifndef INPUT_H
#define INPUT_H

#include "globals.h"

int cmp_rules(const void *a, const void *b);
int read_rules(char *filename);
int read_transactions(char *filename);
void sort_rules();

#endif
