#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>

#include "globals.h"
#include "types.h"

void output_match_to_thread_buffer(thread_output_buffer_t *thread_buffer, int trans_id, int classification);
void output_match_to_thread_buffer(thread_output_buffer_t *thread_buffer, int trans_id, int classification);
void output_thread_buffer_to_file(thread_output_buffer_t *thread_buffer);

void show_rules(int n);
void show_transactions(int n);

#endif
