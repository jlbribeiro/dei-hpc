#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "constants.h"
#include "types.h"

extern int n_rules;
extern int n_transactions;

extern int rules[MAX_SIZE][RULES_LEN];
extern int transactions[MAX_SIZE][TRANSACTIONS_LEN];

extern work_queue_t work_queue;
extern pthread_mutex_t output_mutex;

extern FILE *output_fd;

#endif