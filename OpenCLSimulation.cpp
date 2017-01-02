#include "OpenCLSimulation.hpp"

OpenCLSimulation::OpenCLSimulation(int nbParticles) : _nbParticles(nbParticles)
{
	std::cout << "Init OpenGL Manager" << std::endl;
	this->_glMan = new OpenGLManager(1024, 1024, "ps goodness");
	this->_control.setDimension(1024, 1024, std::pow(nbParticles, 1.0f / 3.0f) * tan(0.392f) * 700.0f);
	this->_glMan->setControl(&(this->_control));	
	std::cout << "Init OpenGL Scene" << std::endl;
	this->_glScene = new OpenGLScene(nbParticles);
	this->_glScene->createShaderProg("shader/vs.glsl", "shader/fs.glsl");
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pX"), 0.0f);
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pY"), 0.0f);
	this->_glScene->initVbo();
	std::cout << "Create CL Context" << std::endl;
	this->createContext();
	this->initCLMem(this->_glScene->getVbo());
	this->initSimulation();
	std::cout << "Simulation Initialized" << std::endl;
}

OpenCLSimulation::~OpenCLSimulation()
{
	clReleaseMemObject(this->_particles);
	clReleaseMemObject(this->_particlesVelocity);
	delete this->_pInitTask;
	delete this->_pMoveTask;
	clReleaseCommandQueue(this->_queue);
	clReleaseContext(this->_ctx);
	delete this->_glScene;
	delete this->_glMan;
}

void							OpenCLSimulation::createContext()
{
	CGLContextObj				kCGLContext = CGLGetCurrentContext();
    CGLShareGroupObj			kCGLShareGroup = CGLGetShareGroup(kCGLContext);
	cl_context_properties		properties[] =
	{
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		(cl_context_properties)kCGLShareGroup,
		0
	};

    this->_ctx = clCreateContext(properties, 0, 0, NULL, NULL, &(this->_err));
    checkCLError(this->_err, "building context");
    this->_err = clGetContextInfo(this->_ctx, CL_CONTEXT_NUM_DEVICES,
              				sizeof(cl_int), &(this->_deviceNb), NULL);
	checkCLError(this->_err, "context info");

    if (this->_deviceNb < 1) {
		checkCLError(-1, "not enough devices");
    }

    this->_err = clGetContextInfo(this->_ctx,CL_CONTEXT_DEVICES,
						sizeof(cl_device_id), &(this->_device), NULL);
	checkCLError(this->_err, "binding context to device");

	this->_queue = clCreateCommandQueue(this->_ctx, this->_device, 0, &(this->_err));
	checkCLError(this->_err, "creating command queue");
}

void					OpenCLSimulation::initSimulation()
{
	std::cout << "Create pInit Task" << std::endl;
	this->_pInitTask = new OpenCLTaskPInit(this->_nbParticles);
	this->_pInitTask->initTask(this->_ctx, this->_device,"kernels/initParticles.cl", "initParticles");
	this->_pInitTask->setKernelArg(this->_particles, this->_particlesVelocity, 0);

	std::cout << "Create pMove Task" << std::endl;
	this->_pMoveTask = new OpenCLTaskPMove(this->_nbParticles);
	this->_pMoveTask->initTask(this->_ctx, this->_device,"kernels/moveParticles.cl", "moveParticles");
	this->_pMoveTask->setKernelArg(this->_particles, this->_particlesVelocity);
	this->_pMoveTask->setKernelVar(0.f, 0.f, this->_control.gravInverted);
}

void					OpenCLSimulation::initParticles()
{
	this->acquireGLObject();
	this->_pInitTask->execKernel(this->_queue);
	this->releaseGLObject();
	clFinish(this->_queue);
}

void					OpenCLSimulation::moveParticles(float xPos, float yPos)
{
	this->acquireGLObject();
	if (!this->_control.fixed)
	{
		this->_pMoveTask->setKernelVar(xPos, yPos, this->_control.gravInverted);
		glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pX"), xPos);
		glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pY"), yPos);
	}
	if (this->_control.launch)
		this->_pMoveTask->execKernel(this->_queue);
	this->releaseGLObject();
	clFinish(this->_queue);
}

void					OpenCLSimulation::runSimulation()
{
	int					i = 0;
	float				fps = 0.f;
	Timer				timer;

	std::cout << "launch Simulation" << std::endl;
	this->initParticles();

	std::cout << "Start Loop" << std::endl;
	while (!this->_glMan->shouldClose())
	{
//		timer.start();
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
timer.start();
		this->moveParticles(this->_control.posX, this->_control.posY);					//	0.0117
timer.stop();
		//this->_glScene->drawScene(this->_viewMatrix, this->_glMan->getProjMat());		//	0.0027
		this->_glScene->drawScene(this->_control.camera->getViewMatrix(), this->_glMan->getProjMat(), this->_control.camera->getEyePos());		//	0.0027

		this->_glMan->swap();
		glfwPollEvents();
//		timer.stop(); 
		fps += timer.getFps();
		i++;
		if (i == 100)
		{	
			//this->_glMan->setWindowName(std::to_string(fps / i));
			this->_glMan->setWindowName(std::to_string(1 / (fps / i)));
			fps = 0.f;
			i = 0;
		}
	}
}

void				OpenCLSimulation::acquireGLObject()
{
	clEnqueueAcquireGLObjects(this->_queue, 1, &(this->_particles), 0, NULL, NULL);
}

void				OpenCLSimulation::releaseGLObject()
{
	clEnqueueReleaseGLObjects(this->_queue, 1, &(this->_particles), 0, NULL, NULL);
}

void				OpenCLSimulation::initCLMem(GLuint vbo)
{
	this->_particles = clCreateFromGLBuffer(this->_ctx, CL_MEM_READ_WRITE, vbo, &(this->_err));
	checkCLError(this->_err, "creating buffer particles");

	this->_particlesVelocity = clCreateBuffer(this->_ctx, CL_MEM_READ_WRITE, 
					this->_nbParticles * sizeof(cl_float) * 4, NULL, &(this->_err));
	checkCLError(this->_err, "creating buffer particles velocity");
}
