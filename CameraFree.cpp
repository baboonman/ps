#include "CameraFree.hpp"

CameraFree::CameraFree(t_vecf eyePos, float pitch, float yaw)
	: _eyePos(eyePos), _pitch(pitch), _yaw(yaw), _oldX(0), _oldY(0)
{
	this->initKeyPress();
	this->calcViewDir();
	this->_viewMatrix.setMatrix(calculateMatrix());
}

void			CameraFree::setMouseFirstPos(double x, double y)
{
	this->_oldX = x;
	this->_oldY = y;
}

void			CameraFree::controlKey(int key, int action , int mods)
{
//	float		sens = 0.5f;
	float		sens = 500.f;
	t_vecf		zero = {0.0f, 0.0f, 0.0f};
	t_vecf		yTrP = {0.0f, 1.0f, 0.0f};
	t_vecf		yTrM = {0.0f, -1.0f, 0.0f};

	if (action == ACTION_PRESS)
	{
		if (mods == MOD_SHIFT)
		{
			if (key == 'E')
				this->_keyPress[2] = yTrM;
		}
		else
		{
			if (key == 'E')
				this->_keyPress[2] = yTrP;
			else if (key == (int)'W')
				this->_keyPress[0] = inverse(this->_viewDir);
			else if (key == (int)'S')
				this->_keyPress[0] = this->_viewDir;
			else if (key == (int)'D')
				this->_keyPress[1] = this->_viewNorm;
			else if (key == (int)'A')
				this->_keyPress[1] = inverse(this->_viewNorm);
		}
	}
	else if (action == ACTION_RELEASE)
	{
		if (key == 'E')
			this->_keyPress[2] = zero;
		else if (key == (int)'W' || key == (int)'S')
			this->_keyPress[0] = zero;
		else if (key == (int)'D' || key == (int)'A')
			this->_keyPress[1] = zero;
	}
	this->_eyePos = translate(this->_eyePos, this->mergeKeyPress(sens));
	this->_viewMatrix.setMatrix(calculateMatrix());
}

void			CameraFree::initKeyPress()
{
	t_vecf		zero = {0.0f, 0.0f, 0.0f};
	this->_keyPress[0] = zero;
	this->_keyPress[1] = zero;
	this->_keyPress[2] = zero;
}

t_vecf			CameraFree::mergeKeyPress(float sens)
{
	t_vecf		merge = add(add(this->_keyPress[0], this->_keyPress[1]), this->_keyPress[2]);
	merge = scale(normalize(merge), sens);
	return merge;
}

void			CameraFree::calcViewDir()
{
	this->_viewDir.x = sin(this->_yaw);
	this->_viewDir.y = -(sin(this->_pitch)*cos(this->_yaw));
	this->_viewDir.z = (cos(this->_pitch)*cos(this->_yaw));
	this->_viewNorm.x = cos(this->_yaw);
	this->_viewNorm.y = 0.0f;
	this->_viewNorm.z = -sin(this->_yaw);
	this->_viewDir = normalize(this->_viewDir);
	this->_viewNorm = normalize(this->_viewNorm);
}

void			CameraFree::controlMouse(double xPos, double yPos)
{
	double		sens = 0.0005f;
//	double		x = xPos - this->_oldX;
//	double		y = yPos - this->_oldY;
	double		x = this->_oldX - xPos;
	double		y = this->_oldY - yPos;
	double		len = sqrt( x * x + y * y );
	double		xn = x / len;
	double		yn = y / len;
	double 		rad = sens * len;

	this->_pitch = rad * yn;
	if (this->_pitch > PI_2_1)
		this->_pitch = PI_2_1;
	else if (this->_pitch < -PI_2_1)
		this->_pitch = -PI_2_1;
	this->_yaw = rad * xn;
	this->calcViewDir();
	this->_viewMatrix.setMatrix(calculateMatrix());
}

Matrix			CameraFree::calculateMatrix()
{
	float cosPitch = cos(this->_pitch);
	float sinPitch = sin(this->_pitch);
	float cosYaw = cos(this->_yaw);
	float sinYaw = sin(this->_yaw);

	t_vecf xAxis = { cosYaw, 0, -sinYaw };
	t_vecf yAxis = { sinYaw * sinPitch, cosPitch, cosYaw * sinPitch };
	t_vecf zAxis = { sinYaw * cosPitch, -sinPitch, cosPitch * cosYaw };

	Matrix		viewMatrix(xAxis.x, yAxis.x, zAxis.x, 0,
						   xAxis.y, yAxis.y, zAxis.y, 0,
						   xAxis.z, yAxis.z, zAxis.z, 0,
						   -dot( xAxis, _eyePos ), -dot( yAxis, _eyePos ), -dot( zAxis, _eyePos ), 1);

	return viewMatrix;
}

t_vecf &			CameraFree::getEyePos()
{
	return (this->_eyePos);
}

void				CameraFree::applyMatOnVec(float &x, float &y, float &z, float &w)
{
//	this->_viewMatrix.multVector(x, y, z, w);
//	OpenGLMatrix	matrix;

//	matrix.rotateX(this->_pitch);
//	matrix.rotateY(this->_yaw);
//	matrix.multVector(x, y, z, w);

	/*
	 *  *      m0      m3      m6   
	 *  *      m1      m4      m7   
	 *  *      m2      m5      m8  
	*/

	(void)w;

	float			nx, ny, nz;
	float			c = cos(this->_pitch);
	float			s = sin(this->_pitch);
	float			C = 1.f - c;

	float			matrix[16];
	
	matrix[0]  = this->_viewNorm.x * this->_viewNorm.x * C + c;
	matrix[1]  = this->_viewNorm.y * this->_viewNorm.x * C + this->_viewNorm.z * s;
	matrix[2]  = this->_viewNorm.z * this->_viewNorm.x * C - this->_viewNorm.y * s;
	matrix[3]  = this->_viewNorm.x * this->_viewNorm.y * C - this->_viewNorm.z * s;
	matrix[4]  = this->_viewNorm.y * this->_viewNorm.y * C + c;
	matrix[5]  = this->_viewNorm.z * this->_viewNorm.y * C + this->_viewNorm.x * s;
	matrix[6]  = this->_viewNorm.x * this->_viewNorm.z * C + this->_viewNorm.y * s;
	matrix[7]  = this->_viewNorm.y * this->_viewNorm.z * C - this->_viewNorm.x * s;
	matrix[8]  = this->_viewNorm.z * this->_viewNorm.z * C + c;


	nx = matrix[0] * x + matrix[3] * y + matrix[6] * z;
	ny = matrix[1] * x + matrix[4] * y + matrix[7] * z;
	nz = matrix[2] * x + matrix[5] * y + matrix[8] * z;

	x = nx;
	y = ny;
	z = nz;

}

//	Equation (derived from matrix) of the rotation of a point about
//	an arbitrary line (point on line + dir vec).
//	Takes P(x, y, z) point to rotate, A(a, b, c) point on rot-axis
//	and vD(u, v, w) direction vector of the rot-axis

static t_vecf		rotAxis(t_vecf point, t_vecf center, t_vecf dir, float t)
{
	t_vecf			res;
	float			cosT = cos(t);
	float			sinT = sin(t);
	float			oneMinusCosT = 1 - cosT;


	float			x = point.x;
	float			y = point.y;
	float			z = point.z;

	float			a = center.x;
	float			b = center.y;
	float			c = center.z;

	float			u = dir.x;
	float			v = dir.y;
	float			w = dir.z;

	float			u2 = dir.x * dir.x;
	float			v2 = dir.y * dir.y;
	float			w2 = dir.z * dir.z;

    res.x = (a*(v2 + w2) - u*(b*v + c*w - u*x - v*y - w*z)) * oneMinusCosT
         + x*cosT
         + (-c*v + b*w - w*y + v*z)*sinT;

    res.y = (b*(u2 + w2) - v*(a*u + c*w - u*x - v*y - w*z)) * oneMinusCosT
         + y*cosT
         + (c*u - a*w + w*x - u*z)*sinT;

    res.z = (c*(u2 + v2) - w*(a*u + b*v - u*x - v*y - w*z)) * oneMinusCosT
         + z*cosT
         + (-b*u + a*v - v*x + u*y)*sinT;

	return (res);
}

void				CameraFree::applyViewOnVec(float &x, float &y, float &z)
{
	t_vecf			res;
	t_vecf			vDir = normalize(cross(this->_viewDir, this->_viewNorm));

	res = rotAxis({x, y, z}, this->_eyePos, this->_viewNorm, this->_pitch);
	res = rotAxis(res, this->_eyePos, vDir, this->_yaw);

	x = res.x;
	y = res.y;
	z = res.z;
}
