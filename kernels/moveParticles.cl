__kernel void	moveParticles(__global float4 *particles, __global float4 *particlesV,
									   float xpos, float ypos, float gi, char eq)
{
	const int	i = get_global_id(0);
	if (i >= MAXGID)
		return ;

	float4		center = (float4) (xpos, ypos, 0.0f, 1.0f);
	float4		particle = particles[i];
	float4		velocity = particlesV[i];
	float		d = distance(particle, center);
	float		d2 = d;

	if (eq == 0)
		d2 = d * d / 1000.f;

	if (d < 10.0f)
	{
//		particlesV[i] += normalize(center - particles[i]) * ( CGM / 10000.0f ) * gi;
//		particlesV[i] += normalize(center - particles[i]) * ( CGM / ( 100.0f / 1000.0f) ) * gi;
//		particles[i] += particlesV[i];
//		velocity += normalize(center - particle) * ( CGM / ( 100.0f / 1000.0f) ) * gi;
//		particlesV[i] = velocity;
		particles[i]  = particle + velocity;
		return;
	}
		
	velocity += normalize(center - particle) * ( CGM / (d2) ) * gi;
	particlesV[i] = velocity;
	particles[i]  = particle + velocity;
}
