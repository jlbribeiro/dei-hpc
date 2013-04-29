CC=mpicc
FLAGS=-Wall -ansi -lm -g
EX=ex1

all:
	${CC} ${EX}.c -o ${EX} ${FLAGS}

run:
	mpirun -np 2 ./${EX}
