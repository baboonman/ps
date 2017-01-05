#include "OpenCLTaskPMove.hpp"

OpenCLTaskPMove::OpenCLTaskPMove(int nbParticles) : OpenCLTask(nbParticles)
{
	float		mP = 1000.f;
	float		mC = 100000.f;
	float		mPart = 1.0e1;
	float		mCenter = 5.9e13;
	float		G = 6.67384e-11;
	float		CGM = G * mP * mC;
	float		GM = G * mPart * mCenter;

	(void)CGM;
	this->_defineOptions.insert(t_makePair("CGM", std::to_string(GM) + "f"));
}

OpenCLTaskPMove::~OpenCLTaskPMove() {}

void			OpenCLTaskPMove::setKernelArg(cl_mem particles, cl_mem particlesV)
{
	this->_err = clSetKernelArg(this->_kernel, 0, sizeof(cl_mem), &particles);
	checkCLError(this->_err, "setting kernel arg: particles");
	this->_err = clSetKernelArg(this->_kernel, 1, sizeof(cl_mem), &particlesV);
	checkCLError(this->_err, "setting kernel arg: particlesV");
}

void			OpenCLTaskPMove::setKernelVar(float xPos, float yPos, float zPos)
{
	this->_err = clSetKernelArg(this->_kernel, 2, sizeof(float), &xPos);
	checkCLError(this->_err, "setting kernel var: xPos");
	this->_err = clSetKernelArg(this->_kernel, 3, sizeof(float), &yPos);
	checkCLError(this->_err, "setting kernel var: yPos");
	this->_err = clSetKernelArg(this->_kernel, 4, sizeof(float), &zPos);
	checkCLError(this->_err, "setting kernel var: zPos");
}

void			OpenCLTaskPMove::setKernelVarGrav(float grav)
{
	this->_err = clSetKernelArg(this->_kernel, 5, sizeof(float), &grav);
	checkCLError(this->_err, "setting kernel var: inverted gravity");
}

void			OpenCLTaskPMove::setKernelVarEq(char eq)
{
	this->_err = clSetKernelArg(this->_kernel, 6, sizeof(char), &eq);
	checkCLError(this->_err, "setting kernel var: eq");
}
