#include "OpenCLTask.hpp"

OpenCLTask::OpenCLTask(int nbParticles) : _nbParticles(nbParticles)
{
	this->_buildOptions.push_back("-Werror");
	this->_defineOptions.insert(t_makePair("MAXGID", std::to_string(nbParticles)));
}

OpenCLTask::~OpenCLTask() {
	clReleaseKernel(this->_kernel);
	clReleaseProgram(this->_program);
}

std::string			OpenCLTask::getOptions() {
	std::string		res = "";

	for (auto &it : this->_buildOptions) {
		res += it + " ";
	}
	for (auto &it : this->_defineOptions) {
		res += "-D " + it.first + "="  + it.second + " ";
	}
	return res;
}

std::string			*getSourceContent(std::string filename)
{
	std::ifstream   ifs(filename);
	std::string     *content;

	content = new std::string(std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>());
	return content;
}

void				OpenCLTask::initTask(cl_context ctx, cl_device_id device,
								std::string progName, std::string kernelName)
{
	this->createProgram(progName, ctx, device);
	this->createKernel(kernelName, device);
}

void				OpenCLTask::createProgram(std::string filename, cl_context ctx,
								cl_device_id device)
{
	std::string		*content;
	const char		*contentCstr;
	std::string		options;
	char			log[16384];

	content = getSourceContent(filename);
	contentCstr = content->c_str();
	this->_program = clCreateProgramWithSource(ctx, 1, &(contentCstr), NULL, &(this->_err));
	checkCLError(this->_err, "creating program");
	options = this->getOptions();
	this->_err = clBuildProgram(this->_program, 0, NULL, options.c_str(), NULL, NULL);
	if (this->_err != CL_SUCCESS) {
		clGetProgramBuildInfo(this->_program, device, CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL);
		std::cerr << "Error on compilation(" << filename << "):" << std::endl;
		std::cerr << log << std::endl;
	}
	delete(content);
}

void			OpenCLTask::createKernel(std::string fnName, cl_device_id device)
{
	this->_kernel = clCreateKernel(this->_program, fnName.c_str(), &(this->_err));
	checkCLError(this->_err, "creating kernel");
	this->_err = clGetKernelWorkGroupInfo(this->_kernel, device, CL_KERNEL_WORK_GROUP_SIZE,
								sizeof(this->_localWorkSize), &(this->_localWorkSize), NULL);
	checkCLError(this->_err, "Getting work group info");

	this->_nbWorkGroup = this->_nbParticles / this->_localWorkSize + 1;
	this->_globalWorkSize = this->_nbWorkGroup * this->_localWorkSize;
}

void			OpenCLTask::execKernel(cl_command_queue queue)
{
	this->_err = clEnqueueNDRangeKernel(queue, this->_kernel, 1, NULL,
										&(this->_globalWorkSize), &(this->_localWorkSize),
										0, NULL, NULL); 
	checkCLError(this->_err, "enqueuing kernel");
}
