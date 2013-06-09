#ifndef BOUNDED_SEARCH_H
#define BOUNDED_SEARCH_H

#include "globals.h"
#define NOT_FOUND -1

int bounded_search_first_rule_match(int value, int part_index, int begin, int end);
int bounded_search_last_rule_match(int value, int part_index, int begin, int end);
void bounded_search_transaction_match(thread_output_matches_t *thread_matches, int trans_id, int part_index, int begin, int end);
void bounded_search_match();
void *thread_match_bounded_search_worker(void *arg);

#endif