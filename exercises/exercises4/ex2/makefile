CC=mpicc
FLAGS=-Wall -ansi -lm -g
EX=ex2

all:
	${CC} ${EX}.c -o ${EX} ${FLAGS}

run:
	mpirun -np 2 ./${EX}

clean:
	rm ${EX}
