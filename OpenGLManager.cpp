#include "OpenGLManager.hpp"

void					OpenGLManager::createProjectionMatrix(void)
{
	this->_clipInfo.aspect = this->_winInfo.width / this->_winInfo.height;
	this->_projectionMatrix.computeProjectionMatrix(this->_clipInfo.fov, this->_clipInfo.aspect, this->_clipInfo.zNear, this->_clipInfo.zFar);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Control*		control;

	(void)scancode;
	(void)mods;
	control = reinterpret_cast<Control *>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
	control->processInput(key, action);
}

static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
	Control*		control;

	control = reinterpret_cast<Control *>(glfwGetWindowUserPointer(window));
	control->processMouse(xPos, yPos);
}

void error_callback(int error, const char* description)
{
	std::cout << "Error #" << error << ": " << description << std::endl;
}

void				OpenGLManager::initOpenGl( void )
{
    int     width, height;
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(0);
    
	glfwWindowHint(GLFW_SAMPLES, 4);
  	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->_window = glfwCreateWindow(this->_winInfo.width, this->_winInfo.height, this->_winInfo.winName.c_str(), NULL, NULL);
    if (! this->_window )
    {
        glfwTerminate();
        exit(0);
    }
    
    glfwSetKeyCallback(this->_window, key_callback);
    glfwSetCursorPosCallback(this->_window, cursor_position_callback);
    glfwMakeContextCurrent(this->_window);
    glfwSwapInterval(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDepthFunc(GL_LESS);
	glfwGetFramebufferSize(this->_window, &(width), &(height));
    glViewport(0, 0, width, height);
}

OpenGLManager::OpenGLManager()
{
	_winInfo.width = 512;
	_winInfo.height = 512;
	_winInfo.winName = "Untitled";
	_clipInfo.fov = 0.785f;
	_clipInfo.aspect = 1;
	_clipInfo.zNear = 0.1;
	_clipInfo.zFar = 10000;
	initOpenGl();
	this->createProjectionMatrix();
}

OpenGLManager::OpenGLManager( GLfloat width, GLfloat height, std::string winName ) 
{
	_winInfo.width = width;
	_winInfo.height = height;
	_winInfo.winName = winName;
	_clipInfo.fov = 0.785f;
	_clipInfo.aspect = width / height;
	_clipInfo.zNear = 0.1f;
	_clipInfo.zFar = 100000.f;
	initOpenGl();
	this->createProjectionMatrix();
}

OpenGLManager::~OpenGLManager()
{
    glfwDestroyWindow(_window);
    glfwTerminate();
}

int				OpenGLManager::shouldClose()
{
	return (glfwWindowShouldClose(this->_window));
}

void				OpenGLManager::swap()
{
	glfwSwapBuffers(this->_window);
}

OpenGLMatrix			OpenGLManager::getProjMat()
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
