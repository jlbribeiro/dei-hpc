#ifndef CONSTANTS_H
#define CONSTANTS_H

#define TRANSACTIONS_FILENAME argv[1]
#define RULES_FILENAME argv[2]
#define OUTPUT_FILENAME argv[3]

#define WILDCARD 0

#define MAX_SIZE_RULES 2000000
#define MAX_SIZE_TRANSACTIONS 1000000

#define TRANSACTIONS_LEN 10
#define TRANSACTION_BUF_SIZE ((5 + 1) * TRANSACTIONS_LEN + 1)

#define RULES_LEN (TRANSACTIONS_LEN + 1)
#define RULE_BUF_SIZE ((5 + 1) * RULES_LEN + 1)

#define MAX_CORES omp_get_num_procs()
#define WORK_BATCH_SIZE 2500

#define MPI_MAX_PROCESSES 9
#define MPI_RANK_MASTER 0

/* The following value MUST be odd (transaction_id/classification pairs + array length on index 0) */
#define THREAD_OUTPUT_MATCHES_SIZE (WORK_BATCH_SIZE * 2 * 3 + 1)
#define THREAD_OUTPUT_MATCHES_BUFFER_SIZE (THREAD_OUTPUT_MATCHES_SIZE * RULE_BUF_SIZE)

#endif
