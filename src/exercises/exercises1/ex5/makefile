include paths.mk

PATH_FLAGS=${LD_LIBRARIES} ${INCLUDES}
COMPILING_FLAGS=-lm -lOpenCL

all: sqrt

clean:
	rm -f *.o *~
	rm -f sqrt

sqrt: sqrt.c
	gcc ${PATH_FLAGS} sqrt.c -o sqrt ${COMPILING_FLAGS}
