#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include <pthread.h>

#include "constants.h"
#include "types.h"

extern int n_rules;
extern int n_transactions;

extern int rules[MAX_SIZE][RULES_LEN];
extern int transactions[MAX_SIZE][TRANSACTIONS_LEN];

extern pthread_mutex_t output_lock;

extern pthread_t thread_match_dequeuer_id;

extern work_queue_t work_queue;

#endif