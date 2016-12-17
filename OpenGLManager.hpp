#ifndef OPENGLMANAGER_HPP
# define OPENGLMANAGER_HPP

# include <math.h>
# include <iostream>
# include <vector>
# include <string>

# include "OpenGLInc.hpp"
# include "OpenGLMatrix.hpp"
# include "Control.hpp"

#define FROMFPS(X)				( 1.0f / X )
#define TOFPS(X)				( 1.0f / X )

typedef struct					s_window_info
{
	GLfloat						width;
	GLfloat						height;
	std::string					winName;
}								t_window_info;

typedef struct					s_clipping_info
{
	GLfloat						fov;
	GLfloat						aspect;
	GLfloat						zNear;
	GLfloat						zFar;
}								t_clipping_info;

class							OpenGLManager
{
	public:
								OpenGLManager();
//								OpenGLManager(const OpenGLManager & rhs);
								OpenGLManager(GLfloat width, GLfloat height, std::string winName);
								~OpenGLManager();
//		OpenGLManager&			operator=(const OpenGLManager & rhs);

		void					setWindowName(std::string name);
		int						shouldClose( void );
		void					swap( void );
		void					setControl(Control *control);
		OpenGLMatrix			getProjMat();


	private:
		void					_initOpenGl( void );
		void					_setWindowInfo(GLfloat width, GLfloat height, std::string name);
		void					_setClippingInfo(GLfloat fov, GLfloat aspect, GLfloat zNear, GLfloat zFar);
		void					_createProjectionMatrix( void );

    	GLFWwindow				*_window;
		OpenGLMatrix			_projectionMatrix;
		t_window_info			_winInfo;
		t_clipping_info			_clipInfo;

};

#endif
