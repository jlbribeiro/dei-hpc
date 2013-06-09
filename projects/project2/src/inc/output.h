#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

#include "globals.h"
#include "types.h"

void output_match_to_thread_matches(thread_output_matches_t *thread_matches, int trans_id, int classification);
void output_match_to_thread_matches(thread_output_matches_t *thread_matches, int trans_id, int classification);
void output_thread_matches_to_file(thread_output_matches_t *thread_matches);
void output_thread_matches_to_master(thread_output_matches_t *thread_matches);

void show_rules(int n);
void show_transactions(int n);

#endif
