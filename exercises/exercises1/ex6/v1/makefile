include paths.mk

PATH_FLAGS=${LD_LIBRARIES} ${INCLUDES}
COMPILING_FLAGS=-lm -lOpenCL

all: matrix_mult matrices

clean:
	rm -f *.o *~
	rm -f matrix_mult

matrix_mult: matrix_mult.c
	gcc ${PATH_FLAGS} matrix_mult.c -o matrix_mult ${COMPILING_FLAGS}

matrices:
	python matrices_generator.py
