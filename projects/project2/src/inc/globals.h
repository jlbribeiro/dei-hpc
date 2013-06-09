#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#include "constants.h"
#include "types.h"

extern int n_rules;
extern int n_transactions;

extern int rules[MAX_SIZE_RULES][RULES_LEN];
extern int transactions[MAX_SIZE_TRANSACTIONS][TRANSACTIONS_LEN];

extern work_queue_t work_queue;
extern pthread_mutex_t output_mutex;

extern FILE *output_fd;

extern int mpi_self_rank;
extern int mpi_n_processes;

#endif