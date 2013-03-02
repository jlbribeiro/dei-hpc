#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <CL/cl.h>

#define KERNEL_SRC "sqrt_kernel.cl"
#define MAX_N 1000

#define clErrorHandling(method) if (err != CL_SUCCESS) { fprintf(stderr, "Error @ %s - code: %d", method, err); exit(1); }

float h_input[MAX_N];
float h_output[MAX_N];

char* file_to_string(char *filepath)
{
	size_t size;
	FILE* file;
	char* string;

	file = fopen(filepath, "r");
	if (!file)
	{
		fprintf(stderr, "Failed to read kernel");
		exit(1);
	}

	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	string = (char *) malloc(size + 1);
	fread(string, size, 1, file);
	string[size] = 0;

	return string;
}

int main()
{
	int n, i;

	size_t global;
	size_t local;

	char *kernel_src;

	cl_int err;
	cl_platform_id platform_id;
	cl_device_id device_id;
	size_t local_size;

	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel kernel;
	cl_mem d_input;
	cl_mem d_output;

	cl_event event;
	cl_ulong start, end;

	n = MAX_N;
	for (i = 0; i < n; i++)
		h_input[i] = i * i;

	/* start OpenCL */
	err = clGetPlatformIDs(1, &platform_id,NULL);
	clErrorHandling("clGetPlatformIDs");

	err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	clErrorHandling("clGetDeviceIDs");

	context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
	clErrorHandling("clCreateContext");

	commands = clCreateCommandQueue(context, device_id, CL_QUEUE_PROFILING_ENABLE, &err);
	clErrorHandling("clCreateCommandQueue");

	/* create kernel */
	kernel_src = file_to_string(KERNEL_SRC);
	program = clCreateProgramWithSource(context, 1, (const char**) &kernel_src, NULL, &err);
	free(kernel_src);
	clErrorHandling("clCreateProgramWithSource");

	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	clErrorHandling("clBuildProgram");

	kernel = clCreateKernel(program, "sqrt_kernel", &err);
	clErrorHandling("clCreateKernel");

	/* allocate memory and send to gpu */
	d_input = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * n, NULL, &err);
	clErrorHandling("clCreateBuffer");

	d_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n, NULL, &err);
	clErrorHandling("clCreateBuffer");

	err = clEnqueueWriteBuffer(commands, d_input, CL_TRUE, 0, sizeof(float) * n, h_input, 0, NULL, NULL);
	clErrorHandling("clEnqueueWriteBuffer");

	err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &local_size, NULL);
	clErrorHandling("clGetDeviceInfo");

	/* prepare kernel args */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_input);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_output);
	err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &n);

	/* execute */
	global = local_size;
	local = local_size;

	err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, &event);
	clErrorHandling("clEnqueueNDRangeKernel");

	clWaitForEvents(1, &event);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
	fprintf(stderr, "Time for event (ms): %10.5f \n", (end - start)/1000000.0);

	err = clFinish(commands);
	clErrorHandling("clFinish");

	/* transfer back */
	err = clEnqueueReadBuffer(commands, d_output, CL_TRUE, 0, sizeof(float) * n, h_output, 0, NULL, NULL);
	clErrorHandling("clEnqueueReadBuffer");

	for (i = 0; i < n; i++)
		printf("%f ", h_output[i]);
	printf("\n");

	/* cleanup*/
	clReleaseMemObject(d_input);
	clReleaseMemObject(d_output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	clReleaseEvent(event);

	return 	0;
}
