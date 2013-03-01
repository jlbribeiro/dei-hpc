#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <CL/cl.h>

#define KERNEL_SRC "sqrt_kernel.cl"
#define MAX_N 1024

#define MATRIX1_FILE "matrix1"
#define MATRIX2_FILE "matrix2"

#define clErrorHandling(method) if (err != CL_SUCCESS) { fprintf(stderr, "Error @ %s - code: %d", method, err); exit(1); }

float h_mat1[MAX_N * MAX_N];
float h_mat2[MAX_N * MAX_N];
float h_output[MAX_N * MAX_N];
int n = MAX_N;

long calc_time(struct timeval start, struct timeval end)
{
	long mtime, seconds, useconds;

	seconds  = end.tv_sec  - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	return ((seconds) * 1000 + useconds/1000.0) + 0.5;
}

void swapf(float *a, float *b)
{
	float tmp = *a;
	*a = *b;
	*b = tmp;
}

char* file_to_string(char *filepath)
{
	size_t size;
	FILE* file;
	char* string;

	file = fopen(filepath, "r");
	if (!file)
	{
		fprintf(stderr, "Failed to load file [%s]", filepath);
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

void matrix_read(char *filename, float *mat)
{
	FILE *f = fopen(filename, "r");
	int i, j;

	if (f == NULL)
	{
		printf("File [%s] not found!\n", filename);
		exit(0);
	}

	fscanf(f, "%d", &n);

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			fscanf(f, "%f", &mat[i * n + j]);

	fclose(f);
}

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

void matrix_transpose(float *mat, int n)
{
	int i, j;

	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++)
			swapf(&mat[i * n + j], &mat[j * n + i]);
}

void cpu_mult_naive(float *mat1, float *mat2, float* res, int n)
{
	int i, j, k;

	memset(res, 0, n * n * sizeof(float));
	
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			for (k = 0; k < n; ++k)
				res[i * n + j] += mat1[i * n + k] * mat2[k * n + j];

}

void cpu_mult_cache(float *mat1, float *mat2, float* res, int n)
{
	int i, j, k;

	memset(res, 0, n * n * sizeof(float));
	
	matrix_transpose(mat2, n);

	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			for (k = 0; k < n; ++k)
				res[i * n + j] += mat1[i * n + k] * mat2[j * n + k];
}

void gpu_mult()
{
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
	cl_mem d_mat1;
	cl_mem d_mat2;
	cl_mem d_output;

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
	d_output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * n * n, NULL, &err);
	clErrorHandling("clCreateBuffer");

	err = clEnqueueWriteBuffer(commands, d_mat1, CL_TRUE, 0, sizeof(float) * n * n, h_mat1, 0, NULL, NULL);
	err |= clEnqueueWriteBuffer(commands, d_mat2, CL_TRUE, 0, sizeof(float) * n * n, h_mat2, 0, NULL, NULL);
	clErrorHandling("clEnqueueWriteBuffer");

	err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &local_size, NULL);
	clErrorHandling("clGetDeviceInfo");

	/* prepare kernel args */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_mat1);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_mat2);
	err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_output);
	err |= clSetKernelArg(kernel, 3, sizeof(unsigned int), &n);

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

	matrix_print(h_output, n);

	/* cleanup*/
	clReleaseMemObject(d_mat1);
	clReleaseMemObject(d_mat2);
	clReleaseMemObject(d_output);
	clReleaseProgram(program);
	clReleaseKernel(kernel);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);
	clReleaseEvent(event);
}

int main()
{
	struct timeval start, end;

	matrix_read(MATRIX1_FILE, h_mat1);
	matrix_read(MATRIX2_FILE, h_mat2);

	gettimeofday(&start, NULL);
	cpu_mult_naive(h_mat1, h_mat2, h_output, n);
	gettimeofday(&end, NULL);
	printf("Finished CPU naïve in %ld milliseconds\n", calc_time(start, end));

	gettimeofday(&start, NULL);
	cpu_mult_cache(h_mat1, h_mat2, h_output, n);
	gettimeofday(&end, NULL);
	printf("Finished CPU with use of cache in %ld milliseconds\n", calc_time(start, end));

	// gpu_mult(NULL, NULL, NULL, 0);

	return 	0;
}
