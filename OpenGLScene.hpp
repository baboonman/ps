#ifndef OPENGLSCENE_HPP
# define OPENGLSCENE_HPP

# include <map>
# include <vector>
# include "OpenGLShader.hpp"
# include "OpenGLMatrix.hpp"

class							OpenGLScene
{
	public:
								OpenGLScene(int nbParticles);
								~OpenGLScene();
		void					createShaderProg(std::string VSFile, std::string FSFile);

		void					drawScene(OpenGLMatrix & view, OpenGLMatrix & project);
		void					addMatricesToProgram(GLuint progID, OpenGLMatrix & model,
													OpenGLMatrix & view, OpenGLMatrix & project);
		void					initVbo();
		
		OpenGLMatrix*			getModelMatrix();
		GLuint					getVbo();
		GLuint					getProg();


	private:
		OpenGLShader*			_shader;
		GLuint					_vao;
		GLuint					_vbo[2];

		OpenGLMatrix			*_modelMatrix;
		OpenGLMatrix			_projectionMatrix;
		GLuint					_nbParticles;
};


#endif
