CC=mpicc
FLAGS=-ansi -Wall -O4 -fopenmp -pthread -march=native

INC_DIR=inc
BUILD_DIR=../bin
OUTPUT_DIR=/tmp/cad-output

PROGRAM=${BUILD_DIR}/main
OUTPUT_FILE=${OUTPUT_DIR}/output.csv

HEADERS=${wildcard ${INC_DIR}/*.h}
SOURCES=${wildcard *.c}
OBJECTS=${SOURCES:%.c=${BUILD_DIR}/%.o}

all: ${PROGRAM}

clean: clean-program clean-output

clean-program:
	rm -rf ${BUILD_DIR}

clean-output:
	rm -rf ${OUTPUT_DIR}

run: ${PROGRAM} clean-output
	@mkdir -p ${OUTPUT_DIR}
	mpirun -np 9 --hostfile hosts ${PROGRAM} /dataset/THE_PROBLEM/trans_day_0.csv /dataset/THE_PROBLEM/rules2M.csv ${OUTPUT_FILE}

run-local: ${PROGRAM} clean-output
	@mkdir -p ${OUTPUT_DIR}
	mpirun -np 4 ${PROGRAM} ~/cad-dataset/THE_PROBLEM/trans_day_0.csv ~/cad-dataset/THE_PROBLEM/rules2M.csv ${OUTPUT_FILE}

deploy:
	scp -r ../src jpbat@student.dei.uc.pt:/home/lei2009/jpbat/cad

${PROGRAM}: ${OBJECTS} ${HEADERS}
	@mkdir -p ${BUILD_DIR}
	${CC} ${FLAGS} ${OBJECTS} -o ${PROGRAM}

${BUILD_DIR}/%.o: %.c ${HEADERS}
	@mkdir -p ${BUILD_DIR}
	${CC} ${FLAGS} $< -c -o $@
