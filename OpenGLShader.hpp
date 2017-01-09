#ifndef OPENGLSHADER_HPP
# define OPENGLSHADER_HPP

# include <stdio.h>
# include <stdlib.h>
# include <string>
# include <vector>
# include <fstream>
# include <sstream>

# include "OpenGLInc.hpp"

class					OpenGLShader
{
	
	public:
								OpenGLShader();
								OpenGLShader(OpenGLShader const & rhs);
		virtual					~OpenGLShader();
		OpenGLShader&			operator=(OpenGLShader const & rhs);
		int						addShader( GLenum type, std::string filename );
		int						createProgram( void );
		GLuint					getProgram( void );

		char					*filetobuf( const char *file );
		std::string				readFile(std::string & source);
		void					deleteShader( void );

	private:
    	GLuint					_shaderProgram;
		std::vector<GLuint>		_shaders;
};

#endif
