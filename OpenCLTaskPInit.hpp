#ifndef OPENCLTASKPINIT_HPP
# define OPENCLTASKPINIT_HPP

# include <cmath>
# include "OpenCLTask.hpp"

class							OpenCLTaskPInit : public OpenCLTask
{
	public:
								OpenCLTaskPInit(int nbParticles);
								~OpenCLTaskPInit();
		void					setKernelArg(cl_mem particles, cl_mem particlesV);
		void					setKernelVar(int initShape);
		void					setSphereDefine(void);
		
};

#endif
