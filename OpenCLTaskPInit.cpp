#include "OpenCLTaskPInit.hpp"

OpenCLTaskPInit::OpenCLTaskPInit(int nbParticles) : OpenCLTask(nbParticles)
{
	float			pPerLine;

	pPerLine = std::pow(nbParticles, 1.0f / 3.0f);
	std::cout << "pPerLine = " << pPerLine << std::endl;
	this->_defineOptions.insert(t_makePair("PPERLINE", std::to_string(pPerLine) + "f"));
	this->_defineOptions.insert(t_makePair("M_PI", std::to_string(3.1415926) + "f"));
	this->setSphereDefine();
}

OpenCLTaskPInit::~OpenCLTaskPInit() {}

void			OpenCLTaskPInit::setKernelArg(cl_mem particles, cl_mem particlesV)
{
	this->_err = clSetKernelArg(this->_kernel, 0, sizeof(cl_mem), &particles);
	checkCLError(this->_err, "setting kernel arg: particles");
	this->_err = clSetKernelArg(this->_kernel, 1, sizeof(cl_mem), &particlesV);
	checkCLError(this->_err, "setting kernel arg: particlesV");
}

void			OpenCLTaskPInit::setKernelVar(int initShape)
{
	this->_err = clSetKernelArg(this->_kernel, 2, sizeof(cl_mem), &initShape);
	checkCLError(this->_err, "setting kernel arg: initShape");
}

void			OpenCLTaskPInit::setSphereDefine(void)
{
	int	tot_oa, tot_teta;
	float	rSphere, stepSphere, stepCircle, nbPointPerLine;

	rSphere = 100.f;
	stepCircle = (0.01f * M_PI);
	nbPointPerLine = 15;
	stepSphere = (4 * rSphere * M_PI * nbPointPerLine) / (this->_nbParticles * stepCircle);
	tot_oa = (int)((float)((rSphere / stepSphere) * 2.f));
	tot_teta = ((2.f * M_PI) / stepCircle);

	this->_defineOptions.insert(t_makePair("R_SPHERE", std::to_string(rSphere) + "f"));
	this->_defineOptions.insert(t_makePair("STEP_SPHERE", std::to_string(stepSphere) + "f"));
	this->_defineOptions.insert(t_makePair("STEP_CIRCLE", std::to_string(stepCircle) + "f"));
	this->_defineOptions.insert(t_makePair("NB_POINT_PER_LINE", std::to_string(nbPointPerLine) + "f"));
	this->_defineOptions.insert(t_makePair("TOT_OA", std::to_string(tot_oa) + ".f"));
	this->_defineOptions.insert(t_makePair("TOT_TETA", std::to_string(tot_teta) + ".f"));
}
