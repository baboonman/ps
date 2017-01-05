#include "CameraFree.hpp"

CameraFree::CameraFree(t_vecf eyePos, float pitch, float yaw)
	: _eyePos(eyePos), _pitch(pitch), _yaw(yaw), _oldX(0), _oldY(0)
{
	this->initKeyPress();
	this->calcViewDir();
	this->_viewMatrix.setMatrix(calculateMatrix());
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
	OpenGLMatrix	matrix;

	matrix.rotateX(this->_pitch);
	matrix.rotateY(this->_yaw);
	matrix.multVector(x, y, z, w);
}
