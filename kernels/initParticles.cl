__kernel void	initParticles(__global float4 *particles, __global float4 *particlesV, int isCube)
{
	const int	i = get_global_id(0);
	float		x, y, z, w;

	if (i  >= MAXGID)
		return ;

	w = 1.0f;
	if (isCube == 1)
	{
		x = i % int(PPERLINE) - (PPERLINE / 2);
		z = int(i / int(PPERLINE * PPERLINE)) - (PPERLINE / 2);
		y = int((i % int(PPERLINE * PPERLINE)) / PPERLINE) - (PPERLINE / 2);
	}
	else if (isCube == 0)
	{
		float	r = 1.0f;
		float	q = 0.0f;
		float	qX = 10.0f;
		float	phi, theta;
		float	j, sr, a, b;

		while (i > qX)
		{
			q += pow(qX, r);
			r++;
			qX += q;
		}
		j = i - q;
		sr = (2.0f * M_PI) / sqrt(pow(10.f, r));
		a = sr / 2.f;
		b = sr;
		phi = a * j;
		theta = b * j;
		x = r * sin(phi) * cos(theta);
		y = r * sin(phi) * sin(theta);
		z = r * cos(phi);
	}
	else if (isCube == 2)
	{
		float	n, r, oa, teta;
		int		oa_index, teta_index, n_index;

		oa_index = i / (TOT_TETA * NB_POINT_PER_LINE);
		teta_index = (i - (oa_index * TOT_TETA * NB_POINT_PER_LINE)) / NB_POINT_PER_LINE;
		n_index = (i - (oa_index * TOT_TETA * NB_POINT_PER_LINE + teta_index * NB_POINT_PER_LINE));

		oa = (float)(R_SPHERE) - ((float)(oa_index) * (float)(STEP_SPHERE));
		teta = (float)(teta_index) * (float)(STEP_CIRCLE);
		n = (float)(n_index) / (float)(NB_POINT_PER_LINE);
		r = sqrt(((float)(R_SPHERE * R_SPHERE)) - oa * oa);
		x = (cos(teta) * r) * n;
		y = (sin(teta) * r) * n;
		z = ((float)(R_SPHERE) - ((float)(oa_index) * (float)STEP_SPHERE));
	}
//	particles[i] = (float4) (x, y, z, w);
	particles[i] = (float4) (x * 50.0f, y * 50.0f, z * 50.0f, w);
	particlesV[i] = (float4) (0.0f, 0.0f, 0.0f, 0.0f);
}
