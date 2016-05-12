__kernel void	moveParticles(__global float4 *particles, __global float4 *particlesV, float xpos, float ypos, float gi)
{
	float4 center = (float4) (xpos, ypos, 0.0f, 1.0f);
	const int	i = get_global_id(0);
	if (i  >= MAXGID)
		return ;

	float d = distance(particles[i], center);
	float d2 = d*d;

	if (d < 10.0f)
	{
//		particlesV[i] += normalize(center - particles[i]) * ( CGM / 10000.0f ) * gi;
		particlesV[i] += normalize(center - particles[i]) * ( CGM / ( 100.0f / 1000.0f) ) * gi;
		particles[i] += particlesV[i];
		return;
	}
		
	particlesV[i] += normalize(center - particles[i]) * ( CGM / (d2 / 1000.0f) ) * gi;
	particles[i] += particlesV[i];
}
