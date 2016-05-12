#ifndef OPENCLTASKPINIT_HPP
# define OPENCLTASKPINIT_HPP

# include <cmath>
# include "OpenCLTask.hpp"

class							OpenCLTaskPInit : public OpenCLTask
{
	public:
								OpenCLTaskPInit(int nbParticles);
								~OpenCLTaskPInit();
		void					setKernelArg(cl_mem particles, cl_mem particlesV, int isCube);
		void					setSphereDefine(void);
		
};

#endif
