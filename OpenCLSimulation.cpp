#include "OpenCLSimulation.hpp"

OpenCLSimulation::OpenCLSimulation() : _nbParticles(100000)
{
	this->initSim();
}

OpenCLSimulation::OpenCLSimulation(int nbParticles) : _nbParticles(nbParticles)
{
	this->initSim();
}

void					OpenCLSimulation::initSim()
{
	this->_glMan = new OpenGLManager(1024, 1024, "ps goodness");
	this->_control.setDimension(1024, 1024, tan(0.392f) * 70000.0f);
	this->_control.setStartEyePos({0.f, 0.f, 70000.f});
	this->_glMan->setControl(&(this->_control));	
	this->_glScene = new OpenGLScene(this->_nbParticles);
	this->_glScene->createShaderProg("shader/vs.glsl", "shader/fs.glsl");
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pX"), 0.0f);
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pY"), 0.0f);
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pZ"), 0.0f);
	this->_glScene->initVbo();
	this->createContext();
	this->initCLMem(this->_glScene->getVbo());
	this->initSimulation();
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
	this->_pInitTask = new OpenCLTaskPInit(this->_nbParticles);
	this->_pInitTask->initTask(this->_ctx, this->_device,"kernels/initParticles.cl", "initParticles");
	this->_pInitTask->setKernelArg(this->_particles, this->_particlesVelocity);
	this->_pInitTask->setKernelVar(0);

	this->_pMoveTask = new OpenCLTaskPMove(this->_nbParticles);
	this->_pMoveTask->initTask(this->_ctx, this->_device,"kernels/moveParticles.cl", "moveParticles");
	this->_pMoveTask->setKernelArg(this->_particles, this->_particlesVelocity);
	this->_pMoveTask->setKernelVar(0.f, 0.f, 0.f);
	this->_pMoveTask->setKernelVarGrav(this->_control.gravInverted);
	this->_pMoveTask->setKernelVarEq(0);
}

void					OpenCLSimulation::initParticles()
{
	this->acquireGLObject();
	this->_pInitTask->execKernel(this->_queue);
	this->releaseGLObject();
	clFinish(this->_queue);
}

void					OpenCLSimulation::updateGravityCenter()
{
	this->_pMoveTask->setKernelVar(this->_control.posX, this->_control.posY, this->_control.posZ);
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pX"), this->_control.posX);
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pY"), this->_control.posY);
	glUniform1f(glGetUniformLocation(this->_glScene->getProg(), "pZ"), this->_control.posZ);
}

void					OpenCLSimulation::moveParticles()
{
	this->acquireGLObject();
	this->_pMoveTask->setKernelVarGrav(this->_control.gravInverted);
	if (!this->_control.fixed)
		this->updateGravityCenter();
	if (this->_control.launch)
		this->_pMoveTask->execKernel(this->_queue);
	this->releaseGLObject();
	clFinish(this->_queue);
}

void					OpenCLSimulation::checkForResize()
{
	if (this->_control.hasResized) {
		this->_glMan->resize(this->_control.getWidth(), this->_control.getHeight());
		this->_control.hasResized = false;
	}
}

void					OpenCLSimulation::checkForReset()
{
	if (this->_control.hasReset) {
		this->_control.hasReset = false;
		this->updateGravityCenter();
		this->_pMoveTask->setKernelVarEq(this->_control.eq);
		this->_pInitTask->setKernelVar(this->_control.initShape);
		this->initParticles();
	}
}

void					OpenCLSimulation::runSimulation()
{
	int					i = 0;
	float				fps = 0.f;
	Timer				timer;

	this->initParticles();
	while (!this->_glMan->shouldClose())
	{
		timer.start();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		this->checkForReset();
		this->checkForResize();
		this->moveParticles();
		this->_glScene->drawScene(this->_control.camera->getViewMatrix(),
				this->_glMan->getProjMat(), this->_control.camera->getEyePos());
		this->_glMan->swap();
		glfwPollEvents();
		timer.stop(); 
		fps += timer.getFps();
		i++;
		if (i == 30)
		{	
			this->_glMan->setWindowName(std::to_string(fps / i) + " fps");
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
