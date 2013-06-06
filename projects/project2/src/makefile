CC=mpicc
HEADERS=
BUILD_DIR=../bin
OBJECTS=
FLAGS=-ansi -Wall -g
#FLAGS=-ansi -Wall -g -O4 -fopenmp -pthread -march=native

all: ${BUILD_DIR}/main

clean:
	rm -f ${BUILD_DIR}/*.o ${BUILD_DIR}/main

run:
	mpirun -np 9 --hostfile hosts ${BUILD_DIR}/main

deploy:
	scp -r ../* jpbat@student.dei.uc.pt:/home/lei2009/jpbat/cad

${BUILD_DIR}/main: ${BUILD_DIR}/main.o ${HEADERS} ${OBJECTS}
	${CC} ${FLAGS} ${BUILD_DIR}/main.o -o ${BUILD_DIR}/main

${BUILD_DIR}/main.o: main.c ${HEADERS}
	${CC} ${FLAGS} main.c -c -o ${BUILD_DIR}/main.o
