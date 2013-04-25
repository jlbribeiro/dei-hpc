#ifndef OUTPUT_H
#define OUTPUT_H

#include <pthread.h>

#include "output.h"
#include "globals.h"

void output_match(int transaction_id, int rule_id);
void show_rules(int n);
void show_transactions(int n);

#endif
