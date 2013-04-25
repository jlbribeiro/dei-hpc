#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

#include "globals.h"
#include "types.h"

void match_dequeuer_join();
void match_dequeuer_start();
void match_enqueue(int transaction_id, int classification);
void match_output(match_t *match);

void show_rules(int n);
void show_transactions(int n);

void *thread_match_dequeuer(void *arg);

#endif
