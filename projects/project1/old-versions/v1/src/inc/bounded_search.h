#ifndef BOUNDED_SEARCH_H
#define BOUNDED_SEARCH_H

#include "globals.h"
#define NOT_FOUND -1

int bounded_search_first_rule_match(int value, int part_index, int begin, int end);
int bounded_search_last_rule_match(int value, int part_index, int begin, int end);
void match_bounded_search_transaction(int trans_id, int part_index, int begin, int end);
void match_bounded_search();
void *thread_match_bounded_search_worker();

#endif