#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <CL/cl.h>

#include "inc/utils.h"

#define DEBUG
#define MATRIX_PRINT_SAMPLE fprintf(stderr, "%f %f %f %f\n", out[0], out[1], out[n * n - 2], out[n * n - 1])

#define KERNEL_SRC "matrix_mult.cl"
#define MAX_N 1024

#define MATRICES_FILE "matrices.in"

float h_mat1[MAX_N * MAX_N];
float h_mat2[MAX_N * MAX_N];
float h_out[MAX_N * MAX_N];

int n;

void matrix_print(float *mat, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < n; j++)
			printf("%f ", mat[i * n + j]);

		printf("\n");
	}
}

void matrix_read(char *filepath, float *mat1, float *mat2, int *n)
{
	FILE *file;
	int i, j;

	file = fopen(filepath, "r");
	if (!file)
	{
		fprintf(stderr, "Error: File \"%s\" couldn't be open. Aborting.\n", filepath);
		exit(-1);
	}

	fscanf(file, "%d", n);

	for (i = 0; i < *n; i++)
		for (j = 0; j < *n; j++)
			fscanf(file, "%f", &mat1[i * *n + j]);

	for (i = 0; i < *n; i++)
		for (j = 0; j < *n; j++)
			fscanf(file, "%f", &mat2[i * *n + j]);

	fclose(file);
}

void matrix_transpose(float *mat, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			swapf(&mat[i * n + j], &mat[j * n + i]);

}

long matrix_mult_cpu_naive(float *mat1, float *mat2, float *out, int n)
{
	struct timeval start, end;

	int i, j, k;

	memset(out, 0, n * n * sizeof(float));

	START_CHRONO;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++)
				out[i * n + j] += mat1[i * n + k] * mat2[k * n + j];

	STOP_CHRONO;

	#ifdef DEBUG
		MATRIX_PRINT_SAMPLE;
	#endif

	return GET_CHRONO;
}

long matrix_mult_cpu_cache(float *mat1, float *mat2, float *out, int n)
{
	struct timeval start, end;

	int i, j, k;

	memset(out, 0, n * n * sizeof(float));

	START_CHRONO;
	matrix_transpose(mat2, n);

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			for (k = 0; k < n; k++)
				out[i * n + j] += mat1[i * n + k] * mat2[j * n + k];

	STOP_CHRONO;

	#ifdef DEBUG
		MATRIX_PRINT_SAMPLE;
	#endif

	return GET_CHRONO;
}

void matrix_mult_gpu(float *mat1, float *mat2, float *out, int n)
{
	size_t global_size;
	size_t local_size;

	char *kernel_src;

	cl_int err;
	cl_platform_id platform_id;
	cl_device_id device_id;
	size_t max_workgroup_size;

	cl_context context;
	cl_command_queue commands;
	cl_program program;
	cl_kernel kernel;
	cl_mem d_mat1;
	cl_mem d_mat2;
	cl_mem d_out;

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
	d_mat1 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * n * n, NULL, &err);
	d_mat2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float) * n * n, NULL, &err);
	d_out = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n * n, NULL, &err);
	clErrorHandling("clCreateBuffer");

	err = clEnqueueWriteBuffer(commands, d_mat1, CL_TRUE, 0, sizeof(float) * n * n, mat1, 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(commands, d_mat2, CL_TRUE, 0, sizeof(float) * n * n, mat2, 0, NULL, NULL);
	clErrorHandling("clEnqueueWriteBuffer");

	err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &max_workgroup_size, NULL);
	clErrorHandling("clGetDeviceInfo");

	/* prepare kernel args */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_mat1);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_mat2);
	err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_out);
	err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);

	/* execute */
	global_size = max_workgroup_size;
	local_size = max_workgroup_size;

	err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global_size, &local_size, 0, NULL, &event);
	clErrorHandling("clEnqueueNDRangeKernel");

	clWaitForEvents(1, &event);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &end, NULL);
	fprintf(stderr, "Time for event (ms): %10.5f \n", (end - start) / 1000000.0);

	err = clFinish(commands);
	clErrorHandling("clFinish");

	/* transfer back */
	err = clEnqueueReadBuffer(commands, d_out, CL_TRUE, 0, sizeof(float) * n * n, out, 0, NULL, NULL);
	clErrorHandling("clEnqueueReadBuffer");

	MATRIX_PRINT_SAMPLE;

	/* cleanup*/
	clReleaseMemObject(d_mat1);
	clReleaseMemObject(d_mat2);
	clReleaseMemObject(d_out);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	clReleaseEvent(event);
}

int main()
{
	long process_time;

	matrix_read(MATRICES_FILE, h_mat1, h_mat2, &n);

	fprintf(stderr, "Starting hard work.\n");
/*
	process_time = matrix_mult_cpu_naive(h_mat1, h_mat2, h_out, n);
	fprintf(stderr, "Finished CPU naïve in %ld milliseconds\n", process_time);

	fprintf(stderr, "\n");
*/
	process_time = matrix_mult_cpu_cache(h_mat1, h_mat2, h_out, n);
	fprintf(stderr, "Finished CPU with use of cache in %ld milliseconds\n", process_time);

	fprintf(stderr, "\n");

	matrix_mult_gpu(h_mat1, h_mat2, h_out, n);

	return 	0;
}
