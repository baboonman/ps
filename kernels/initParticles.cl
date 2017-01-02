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
/*
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
*/

		float	r = 100.0f;
//		float	rr = 3.0f * r;

		float	ss = sqrt((float)MAXGID);
		float	k = i % ((int)ss);
		float	l = i / ((int)ss);
		float	stepU = (2.f * M_PI) / ss;
		float	stepV = (2.f * M_PI) / ss;
		float	u = stepU * k;
		float	v = stepV * l;

//		x = (rr + r * cos(v)) * cos(u);
//		y = (rr + r * cos(v)) * sin(u);
//		z = r * sin(v);

		x = ( r + cos(u / 2.f) * sin(v) - sin(u / 2.f) * sin(2.f * v) ) * cos(u);
		y = ( r + cos(u / 2.f) * sin(v) - sin(u / 2.f) * sin(2.f * v) ) * sin(u);
		z = sin(u / 2.f) * sin(v) + cos(u / 2.f) * sin(2.f * v);

/*
		float	r = 100.f;
		float	ss = sqrt((float)MAXGID);
		float	u = i % ((int)ss);
		float	v = i / ((int)ss);
		float	stepT = (2.f * M_PI) / ss;
		float	stepP = M_PI / ss;
		float	t = stepT * u;
		float	p = stepP * v;

		x = r * cos(t) * sin(p);
		y = r * sin(t) * sin(p);
		z = r * cos(p);
*/

//		printf("x: %f, y: %f, z: %f\n", x, y, z);

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
