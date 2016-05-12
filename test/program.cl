__kernel void	test(__global float *in)
{
	const int	i = get_global_id(0);
	
	in[i] = in[i] *in[i];

	printf("in[%d] = %f", i, in[i]);
}
