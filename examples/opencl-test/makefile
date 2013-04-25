include paths.mk

PATH_FLAGS=${LD_LIBRARIES} ${INCLUDES}
COMPILING_FLAGS=-lOpenCL

all: opencl-test

clean:
	rm -f *.o *~
	rm -f opencl-test

opencl-test: opencl-test.c
	gcc ${PATH_FLAGS} opencl-test.c -o opencl-test ${COMPILING_FLAGS}
