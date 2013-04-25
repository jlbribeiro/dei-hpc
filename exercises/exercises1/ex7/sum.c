#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <CL/cl.h>

#include "inc/utils.h"

#define DEBUG

#define MAX_N (1 << 23)
#define KERNEL_SRC "sum.cl"

long long h_in[MAX_N];
long long h_out;

int n;

long sum_cpu(long long *in, long long *out, unsigned int n)
{
	int i;
	long long sum;

	struct timeval start, end;

	sum = 0;

	START_CHRONO;
	for (i = 0; i < n; i++)
		sum += in[i];

	STOP_CHRONO;

	*out = sum;

	return GET_CHRONO;
}

void sum_gpu(long long *in, long long *out, unsigned int n)
{
	size_t global_size;
	size_t local_size;

	char *kernel_src;

	cl_int err;
	cl_platform_id platform_id;
	cl_device_id device_id;
	cl_uint max_compute_units;
	size_t max_workgroup_size;

	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel kernel;
	cl_mem d_array;

	cl_event event;
	cl_ulong start, end;

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

	kernel = clCreateKernel(program, "matrix_mult", &err);
	clErrorHandling("clCreateKernel");

	/* allocate memory and send to gpu */
	d_array = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(long long) * n, NULL, &err);
	clErrorHandling("clCreateBuffer");

	err = clEnqueueWriteBuffer(commands, d_array, CL_TRUE, 0, sizeof(long long) * n, in, 0, NULL, NULL);
	clErrorHandling("clEnqueueWriteBuffer");

	err  = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &max_compute_units, NULL);
	err |= clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_workgroup_size, NULL);
	clErrorHandling("clGetDeviceInfo");

	/* prepare kernel args */
	err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_array);
	err |= clSetKernelArg(kernel, 1, sizeof(unsigned int), &n);

	/* execute */
	local_size = n / max_compute_units / 8;
	if (local_size > max_workgroup_size)
		local_size = max_workgroup_size;

	/*
	 *	Usually it would be
	 *	global_size = local_size * max_compute_units;
	 *	but that would only be valid if local_size = n / max_compute_units;
	 *	local_size is n / max_compute_units / 8 because it obtains its hightest performance.
	 */
	for (global_size = local_size; global_size < n; global_size += local_size);

	err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global_size, &local_size, 0, NULL, &event);
	clErrorHandling("clEnqueueNDRangeKernel");

	clWaitForEvents(1, &event);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
	fprintf(stderr, "Time for event (ms): %10.5f \n", (end - start) / 1000000.0);

	err = clFinish(commands);
	clErrorHandling("clFinish");

	/* transfer back */
	err = clEnqueueReadBuffer(commands, d_array, CL_TRUE, 0, sizeof(long long), out, 0, NULL, NULL); // a single long long
	clErrorHandling("clEnqueueReadBuffer");

	/* cleanup*/
	clReleaseMemObject(d_array);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	clReleaseEvent(event);
}

int main()
{
	long process_time;

	unsigned int i;
	n = MAX_N;
	for (i = 0; i < n; i++)
		h_in[i] = i;

	fprintf(stderr, "Starting hard work.\n");

	process_time = sum_cpu(h_in, &h_out, n);
	fprintf(stderr, "Finished CPU in %ld milliseconds\n", process_time);

	fprintf(stderr, "\n");

	sum_gpu(h_in, &h_out, n);
	printf("%lld\n", h_out);

	return 	0;
}
