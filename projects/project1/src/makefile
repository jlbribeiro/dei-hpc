CC=g++
HEADERS=inc/constants.h inc/globals.h inc/types.h inc/utils.h inc/input.h inc/output.h inc/naive.h inc/bounded_search.h
OBJECTS=../bin/utils.o ../bin/input.o ../bin/output.o ../bin/naive.o ../bin/bounded_search.o
FLAGS=-ansi -Wall -g -O4 -fopenmp -pthread -march=native

all: ../bin/main

clean:
	rm -f ../bin/*.o ../bin/main

../bin/main: ../bin/main.o ${HEADERS} ${OBJECTS}
	${CC} ${FLAGS} ../bin/utils.o ../bin/input.o ../bin/output.o ../bin/naive.o ../bin/bounded_search.o ../bin/main.o -o ../bin/main

../bin/main.o: main.c ${HEADERS}
	${CC} ${FLAGS} main.c -c -o ../bin/main.o

../bin/utils.o: utils.c
	${CC} ${FLAGS} utils.c -c -o ../bin/utils.o

../bin/input.o: input.c
	${CC} ${FLAGS} input.c -c -o ../bin/input.o

../bin/output.o: output.c
	${CC} ${FLAGS} output.c -c -o ../bin/output.o

../bin/naive.o: naive.c
	${CC} ${FLAGS} naive.c -c -o ../bin/naive.o

../bin/bounded_search.o: bounded_search.c
	${CC} ${FLAGS} bounded_search.c -c -o ../bin/bounded_search.o

test-problem:
	@echo "Testing THE PROBLEM."
	@echo "Day 0"
	@./main dataset/THE_PROBLEM/trans_day_0.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day0_sorted.csv
	@echo "Day 1"
	@./main dataset/THE_PROBLEM/trans_day_1.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day1_sorted.csv
	@echo "Day 2"
	@./main dataset/THE_PROBLEM/trans_day_2.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day2_sorted.csv
	@echo "Day 3"
	@./main dataset/THE_PROBLEM/trans_day_3.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day3_sorted.csv
	@echo "Day 4"
	@./main dataset/THE_PROBLEM/trans_day_4.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day4_sorted.csv
	@echo "Day 5"
	@./main dataset/THE_PROBLEM/trans_day_5.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day5_sorted.csv
	@echo "Day 6"
	@./main dataset/THE_PROBLEM/trans_day_6.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day6_sorted.csv
	@echo "Day 7"
	@./main dataset/THE_PROBLEM/trans_day_7.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day7_sorted.csv
	@echo "Day 8"
	@./main dataset/THE_PROBLEM/trans_day_8.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day8_sorted.csv
	@echo "Day 9"
	@./main dataset/THE_PROBLEM/trans_day_9.csv dataset/THE_PROBLEM/rules2M.csv > /tmp/cad-project-output-the-problem.csv
	@sort /tmp/cad-project-output-the-problem.csv > /tmp/cad-project-output-the-problem-sorted.csv
	@diff /tmp/cad-project-output-the-problem.csv dataset/THE_PROBLEM/output_day9_sorted.csv

test-small:
	@echo "Testing small input."
	@./main dataset/debug_small/trans.csv dataset/debug_small/rules.csv > /tmp/cad-project-output-small.csv
	@sort /tmp/cad-project-output-small.csv > /tmp/cad-project-output-small_sorted.csv
	@sort dataset/debug_small/output.csv > /tmp/cad-project-expected-output-small-sorted.csv
	@diff /tmp/cad-project-output-small_sorted.csv /tmp/cad-project-expected-output-small-sorted.csv

test-extra-small:
	@echo "Testing extra small input."
	@./main dataset/debug_extra_small/debug_input_transactions.csv dataset/debug_extra_small/debug_rules_table.csv > /tmp/cad-project-output-extra_small.csv
	@sort /tmp/cad-project-output-extra_small.csv > /tmp/cad-project-output-extra_small_sorted.csv
	@sort dataset/debug_extra_small/debug_output.csv > /tmp/cad-project-expected-output-debug_extra_small-sorted.csv
	@diff /tmp/cad-project-output-extra_small_sorted.csv /tmp/cad-project-expected-output-debug_extra_small-sorted.csv
