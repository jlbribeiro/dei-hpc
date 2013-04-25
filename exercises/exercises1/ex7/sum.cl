#define MAX_N 1024

__kernel void matrix_mult(__global long long *array, const unsigned int n)
{
	unsigned int g_id = get_global_id(0);
	unsigned int l_id = get_local_id(0);
	unsigned int gr_id = get_group_id(0);
	unsigned int l_size = get_local_size(0);

	if (g_id >= n - 1)
		return;

	array[g_id] = g_id;
}
