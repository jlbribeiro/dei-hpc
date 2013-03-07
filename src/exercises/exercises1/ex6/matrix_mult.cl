#define MAX_N 1024

__kernel void matrix_mult(__global float *mat1, __global float *mat2, __global float *out, const unsigned int n)
{
	unsigned int g_id = get_global_id(0);
	unsigned int l_id = get_local_id(0);
	unsigned int gr_id = get_group_id(0);

	unsigned int i, j, k;
	unsigned int row_off, col_off;
	float sum;

	__local float local_cache[MAX_N];

	for (i = 0, row_off = 0; i < n; i++, row_off += n)
	{
		for (j = 0; j < 2; j++)
			local_cache[g_id * 2 + j] = mat1[row_off + g_id * 2 + j];

		barrier(CLK_LOCAL_MEM_FENCE);

		for (j = 0; j < 2; j++)
		{
			col_off = g_id * 2 + j;

			sum = 0;
			for (k = 0; k < n; k++)
				sum += local_cache[k] * mat2[col_off * n + k];

			out[row_off + col_off] = sum;
		}

		barrier(CLK_LOCAL_MEM_FENCE);
	}
}
