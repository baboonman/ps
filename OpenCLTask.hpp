#ifndef OPENCLTASK_HPP
# define OPENCLTASK_HPP

# include <fstream>
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include "OpenGLInc.hpp"
# include "OpenCLInc.hpp"
# include "OpenCLError.hpp"

typedef std::pair<std::string, std::string> t_makePair;

class							OpenCLTask
{
	public:
								OpenCLTask();
								OpenCLTask(int nbParticles);
								OpenCLTask(OpenCLTask const & rhs);
		virtual					~OpenCLTask();
		OpenCLTask &			operator=(OpenCLTask const & rhs);
		void					initTask(cl_context ctx, cl_device_id device,
									std::string progName, std::string kernelName);
		void					execKernel(cl_command_queue queue);

	protected:
		void					createProgram(std::string filename, cl_context ctx, cl_device_id device);
		void					createKernel(std::string fnName, cl_device_id device);
		std::string				getOptions();

	protected:
		cl_int					_err;
		cl_program				_program;
		cl_kernel				_kernel;
		int						_nbParticles;
		size_t					_localWorkSize;
		size_t					_globalWorkSize;
		size_t					_nbWorkGroup;
		std::vector<std::string>            _buildOptions;
		std::map<std::string, std::string>  _defineOptions;
};

#endif
