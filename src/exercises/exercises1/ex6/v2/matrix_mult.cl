#define MAX_N 1024

__kernel void matrix_mult(__global float *mat1, __global float *mat2, __global float *out, const unsigned int n)
{
	// unsigned int g_id = get_global_id(0);
	unsigned int l_id = get_local_id(0);
	unsigned int gr_id = get_group_id(0);
	unsigned int l_size = get_local_size(0);

	unsigned int i, j, k;
	unsigned int row_off;
	float sum;

	__local float local_cache[MAX_N];

	for (i = 0; i < l_size; i++)
	{
		row_off = (gr_id * l_size + i) * n;

		for (j = l_id; j < n; j += l_size)
			local_cache[j] = mat1[row_off + j];

		barrier(CLK_LOCAL_MEM_FENCE);

		for (j = l_id; j < n; j += l_size)
		{
			sum = 0;
			for (k = 0; k < n; k++)
				sum += local_cache[k] * mat2[j * n + k];

			out[row_off + j] = sum;
		}

		barrier(CLK_LOCAL_MEM_FENCE);
	}
}
