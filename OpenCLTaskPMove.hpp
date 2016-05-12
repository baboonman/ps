#ifndef OPENCLTASKPMOVE_HPP
# define OPENCLTASKPMOVE_HPP

# include <cmath>
# include "OpenCLTask.hpp"

class							OpenCLTaskPMove : public OpenCLTask
{
	public:
								OpenCLTaskPMove(int nbParticles);
								~OpenCLTaskPMove();
		void					setKernelArg(cl_mem particles, cl_mem particlesV);
		void					setKernelVar(float xPos, float yPos, float i);
		
};

#endif

