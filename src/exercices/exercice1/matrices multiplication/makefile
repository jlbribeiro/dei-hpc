include paths.mk

PATH_FLAGS=${LD_LIBRARIES} ${INCLUDES}
COMPILING_FLAGS=-lOpenCL -lm

all: multiply_matrices

clean:
	rm -f *.o *~
	rm -f multiply_matrices

multiply_matrices: multiply_matrices.c
	gcc ${PATH_FLAGS} multiply_matrices.c -o multiply_matrices ${COMPILING_FLAGS}

run:
	make all
	python matrix_generator.py
	./multiply_matrices