#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MARK_AS_USED(x) (void)(x)

#define WILDCARD 0

#define MAX_SIZE 3000000

#define TRANSACTIONS_LEN 10
#define TRANSACTION_BUF_SIZE ((5 + 1) * TRANSACTIONS_LEN + 1)

#define RULES_LEN (TRANSACTIONS_LEN + 1)
#define RULE_BUF_SIZE ((5 + 1) * RULES_LEN + 1)

#define BUFFER_SIZE 10000000

#endif
