include paths.mk

PATH_FLAGS=${LD_LIBRARIES} ${INCLUDES}
COMPILING_FLAGS=-O3 -lm -lOpenCL

all: sum

clean:
	rm -f *.o *~
	rm -f sum

sum: sum.c
	gcc ${PATH_FLAGS} sum.c -o sum ${COMPILING_FLAGS}
