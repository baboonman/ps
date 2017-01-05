#include "OpenGLScene.hpp"

OpenGLScene::OpenGLScene(int nbParticles) : _nbParticles(nbParticles)
{
	float				tZ;

//	tZ = std::pow(nbParticles, 1.0f / 3.0f) * -700.0f;
	tZ = -70000.f;
	this->_modelMatrix = new OpenGLMatrix;
	this->_modelMatrix->translate(0, 0, tZ);
//	this->_modelMatrix->translate(0, 0, -50);
}

OpenGLScene::~OpenGLScene()
{
	delete this->_modelMatrix;
}

void					OpenGLScene::createShaderProg(std::string VSFile, std::string FSFile)
{
	this->_shader = new OpenGLShader;
	this->_shader->addShader(GL_VERTEX_SHADER, VSFile);
	this->_shader->addShader(GL_FRAGMENT_SHADER , FSFile);
	if (!this->_shader->createProgram())
		return ;
	glUseProgram(this->_shader->getProgram());
}

void					OpenGLScene::drawScene(OpenGLMatrix & view, OpenGLMatrix & project, t_vecf& eyePos)
{
	glBindVertexArray(this->_vao);
	this->addMatricesToProgram(this->_shader->getProgram(), *(this->_modelMatrix), view, project);
	this->addCameraPositionToProgram(this->_shader->getProgram(), eyePos);
	glDrawArrays(GL_POINTS, 0, this->_nbParticles);
	glBindVertexArray(0);
	glFinish();
}

void					OpenGLScene::addMatricesToProgram(GLuint progID, OpenGLMatrix & model, OpenGLMatrix & view, OpenGLMatrix & project)
{
	GLint			uloc_M;
	GLint			uloc_V;
	GLint			uloc_P;

    uloc_M = glGetUniformLocation(progID, "M");
    uloc_V = glGetUniformLocation(progID, "V");
    uloc_P = glGetUniformLocation(progID, "P");

    glUniformMatrix4fv(uloc_M, 1, GL_FALSE, model.getMatrixArray());
    glUniformMatrix4fv(uloc_V, 1, GL_FALSE, view.getMatrixArray());
    glUniformMatrix4fv(uloc_P, 1, GL_FALSE, project.getMatrixArray());
}

void				OpenGLScene::addCameraPositionToProgram(GLuint progID, t_vecf& eyePos)
{
	GLint			uloc;

	uloc = glGetUniformLocation(progID, "eyePos");

	glUniform3f(uloc, eyePos.x, eyePos.y, eyePos.z);
}

OpenGLMatrix*		OpenGLScene::getModelMatrix()
{
	return this->_modelMatrix;
}

void					OpenGLScene::initVbo()
{
	GLuint				attrloc;

	glGenVertexArrays(1, &(this->_vao));
	glBindVertexArray(this->_vao);
	glGenBuffers(1, this->_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * this->_nbParticles, NULL, GL_STREAM_DRAW);
	attrloc = glGetAttribLocation(this->_shader->getProgram(), "in_Position");
	glVertexAttribPointer(attrloc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(attrloc);

	glBindVertexArray(0);
}

GLuint				OpenGLScene::getVbo()
{
	return this->_vbo[0];
}

GLuint				OpenGLScene::getProg()
{
	return this->_shader->getProgram();
}
