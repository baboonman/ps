/*
__kernel void	applyVel(__global float3 *particles, __global float3 *particlesVelocity,
							float centerX, float centerY, __global float *color)
{
	int		gid = get_global_id(0);
	if (gid > MAX_GID)
		return ;
	float3	center;
	float	dist;

	float3	vel = particlesVelocity[gid];
	float3	particle = particles[gid];
	center = (float3)(centerX, centerY, 0.f);
	dist = distance(center, particle);
	color[gid] = (10000 - dist) / 15000.f;
	if (dist < 2.0)
		return ;
	vel += (center - particle) * (((float)(DIVIDEND)) / (pown(dist, 2)));
	particlesVelocity[gid] = vel;
	particle += vel;
	particles[gid] = particle;
}
*/

__kernel void	moveParticles(__global float4 *particles, __global float4 *particlesV, float xpos, float ypos, float gi)
{
	const int	i = get_global_id(0);
	if (i >= MAXGID)
		return ;

	float4		center = (float4) (xpos, ypos, 0.0f, 1.0f);
	float4		particle = particles[i];
	float4		velocity = particlesV[i];
//	float		d = distance(particles[i], center);
	float		d = distance(particle, center);
//	float		d2 = d * d;

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
		
//	particlesV[i] += normalize(center - particles[i]) * ( CGM / (d2 / 1000.0f) ) * gi;
//	particles[i] += particlesV[i];

//	velocity += normalize(center - particle) * ( CGM / (d2 / 1000.0f) ) * gi;
	velocity += normalize(center - particle) * ( CGM / (d) ) * gi;
	particlesV[i] = velocity;
	particles[i]  = particle + velocity;
}
