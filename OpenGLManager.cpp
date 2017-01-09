#include "OpenGLManager.hpp"

static void				key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Control*			control;

	(void)scancode;
	control = reinterpret_cast<Control *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	control->processInput(key, action, mods);

	if (control->camOn)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

static void				cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	Control*			control;

	control = reinterpret_cast<Control *>(glfwGetWindowUserPointer(window));
	control->processMouse(xPos, yPos);
}

static void				framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	Control*			control;

	control = reinterpret_cast<Control *>(glfwGetWindowUserPointer(window));

	control->setNewDim(w, h);
	control->hasResized = true;
	glViewport(0, 0, w, h);
}

static void				error_callback(int error, const char* description)
{
	std::cout << "Error #" << error << ": " << description << std::endl;
}



OpenGLManager::OpenGLManager()
{
	this->_setWindowInfo(512, 512, "Untitled");
	this->_setClippingInfo(0.785f, 1, 0.1, 10000.f);
	this->_initOpenGl();
	this->_createProjectionMatrix();
}

OpenGLManager::OpenGLManager(GLfloat width, GLfloat height, std::string winName)
{
	this->_setWindowInfo(width, height, winName);
	this->_setClippingInfo(0.785f, width / height, 0.1, 1000000.f);
	this->_initOpenGl();
	this->_createProjectionMatrix();
}

/*
OpenGLManager::OpenGLManager(const OpenGLManager & rhs)
{
	*this = rhs;
}
*/

OpenGLManager::~OpenGLManager()
{
    glfwDestroyWindow(this->_window);
    glfwTerminate();
    // delete this->_window ???
}

/*
OpenGLManager&			OpenGLManager::operator=(const OpenGLManager & rhs)
{
	this->_window = 
	
	return (*this);
}
*/

void					OpenGLManager::_setWindowInfo(GLfloat width, GLfloat height, std::string name)
{
	this->_winInfo.width = width;
	this->_winInfo.height = height;
	this->_winInfo.winName = name;
}

void					OpenGLManager::_setClippingInfo(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	this->_clipInfo.fov = fov;
	this->_clipInfo.aspect = aspect;
	this->_clipInfo.zNear = zNear;
	this->_clipInfo.zFar = zFar;
}

void					OpenGLManager::_createProjectionMatrix(void)
{
	this->_clipInfo.aspect = this->_winInfo.width / this->_winInfo.height;
	this->_projectionMatrix.computeProjectionMatrix(this->_clipInfo.fov, this->_clipInfo.aspect, this->_clipInfo.zNear, this->_clipInfo.zFar);
}

void					OpenGLManager::resize(float width, float height)
{
	this->_winInfo.width = width;
	this->_winInfo.height = height;
	this->_clipInfo.aspect = width / height;
	this->_createProjectionMatrix();
}

void					OpenGLManager::_initOpenGl( void )
{
    int     			width, height;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(0);
	glfwWindowHint(GLFW_SAMPLES, 4);
  	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->_window = glfwCreateWindow(this->_winInfo.width, this->_winInfo.height,
    					this->_winInfo.winName.c_str(), NULL, NULL);
    if (!this->_window)
    {
        glfwTerminate();
        exit(0);
    }
	glfwSetFramebufferSizeCallback(this->_window, framebuffer_size_callback);
    glfwSetKeyCallback(this->_window, key_callback);
    glfwSetCursorPosCallback(this->_window, cursor_position_callback);
    glfwMakeContextCurrent(this->_window);
    glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDepthFunc(GL_LESS);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glfwGetFramebufferSize(this->_window, &(width), &(height));
    glViewport(0, 0, width, height);
}

int						OpenGLManager::shouldClose()
{
	return (glfwWindowShouldClose(this->_window));
}

void					OpenGLManager::swap()
{
	glfwSwapBuffers(this->_window);
}

OpenGLMatrix &			OpenGLManager::getProjMat()
{
	return this->_projectionMatrix;
}

void					OpenGLManager::setControl(Control *control)
{
	glfwSetWindowUserPointer(this->_window, static_cast<void *>(control));
}

void					OpenGLManager::setWindowName(std::string name)
{
	glfwSetWindowTitle(this->_window, name.c_str());
}
