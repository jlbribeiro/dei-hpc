__kernel void sqrt_kernel(__global float *in, __global float *out, const unsigned int n)     
{
	unsigned int i;

	// Get our global/local thread ID
	unsigned int l_id = get_local_id(0); 
	unsigned int l_size = get_local_size(0); 

	// Make sure we do not go out of bounds
	for (i = l_id; i < n; i += l_size)
		out[i] = sqrt(in[i]); // currently just copying

}
