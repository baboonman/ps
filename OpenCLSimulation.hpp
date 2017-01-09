#ifndef OPENCLSIMULATION_HPP
# define OPENCLSIMULATION_HPP

# include <vector>
# include <string>
# include "OpenGLManager.hpp"
# include "OpenGLScene.hpp"
# include "OpenCLInc.hpp"
# include "OpenCLError.hpp"
# include "OpenCLTaskPInit.hpp"
# include "OpenCLTaskPMove.hpp"
# include "Control.hpp"
# include "Timer.hpp"

class							OpenCLSimulation
{
	public:
								OpenCLSimulation(OpenCLSimulation const & rhs);
								OpenCLSimulation(int nbParticles);
								OpenCLSimulation();
		virtual					~OpenCLSimulation();

		OpenCLSimulation&		operator=(OpenCLSimulation const & rhs);
		void					initSimulation();
		void					runSimulation();

	private:
		void					initSim();
		void					initParticles();
		void					moveParticles();
		void					createContext();
		void					acquireGLObject();
		void					releaseGLObject();
		void					initCLMem(GLuint vbo);
		void					checkForResize();
		void					checkForReset();
		void					updateGravityCenter();

	private:
		cl_int					_err;
		cl_context				_ctx;
		cl_command_queue		_queue;
		cl_device_id			_device;
		cl_int					_deviceNb;
		cl_mem					_particles;
		cl_mem					_particlesVelocity;
		OpenCLTaskPInit*		_pInitTask;
		OpenCLTaskPMove*		_pMoveTask;
		OpenGLManager*			_glMan;
		OpenGLScene*			_glScene;
		Control					_control;

		int						_nbParticles;
};

# endif
