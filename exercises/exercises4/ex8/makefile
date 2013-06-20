# Compilation flags
CC=mpicc
FLAGS=-ansi -Wall -Wno-unused-result -O4 -march=native

# Targets
PROGRAM=main
INPUT_FILENAME=input

# Dependencies
SOURCES=${wildcard *.c}
OBJECTS=${SOURCES:.c=.o}

# MPI-related
N_NODES=4
HOSTFILE=hosts

all: ${PROGRAM}

clean:
	rm -rf ${INPUT_FILENAME} ${OBJECTS} ${PROGRAM}

run: input ${PROGRAM}
	mpirun -np ${N_NODES} -hostfile ${HOSTFILE} ./${PROGRAM} ${INPUT_FILENAME}

input:
	python generator.py 5000000 > ${INPUT_FILENAME}

${PROGRAM}: ${OBJECTS}
	${CC} ${FLAGS} ${OBJECTS} -o ${PROGRAM}

%.o: %.c
	${CC} ${FLAGS} $< -c -o $@