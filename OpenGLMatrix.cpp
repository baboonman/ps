#include "OpenGLMatrix.hpp"

std::ostream&			operator<<(std::ostream& flux, OpenGLMatrix const& m)
{
	flux << m.getMatrix();
	return flux;
}

OpenGLMatrix::OpenGLMatrix()
{
}

OpenGLMatrix::~OpenGLMatrix()
{
}

Matrix					OpenGLMatrix::getMatrix( void ) const { return this->_matrix; }
void					OpenGLMatrix::setMatrix( Matrix matrix ) { this->_matrix = matrix; }

void					OpenGLMatrix::scale( float  scale )
{
	Matrix				matrix;

	matrix = createScaleMatrix(scale);
	this->_matrix *= matrix;
}

void					OpenGLMatrix::rotateX( float alpha )
{
	Matrix				matrix;

	matrix = createRotXMatrix(alpha);
	this->_matrix *= matrix;
}

void					OpenGLMatrix::rotateY( float alpha )
{
	Matrix				matrix;

	matrix = createRotYMatrix(alpha);
	this->_matrix *= matrix;
}

void					OpenGLMatrix::rotateZ( float alpha )
{
	Matrix				matrix;

	matrix = createRotZMatrix(alpha);
	this->_matrix *= matrix;
}

void					OpenGLMatrix::translate( float x, float y, float z )
{
	Matrix				matrix;

	matrix = createTranslationMatrix(x, y, z);
	this->_matrix *= matrix;
}

void					OpenGLMatrix::project( float fovy, float aspect, float near, float far )
{
	Matrix				matrix;

	matrix = createProjectionMatrix(fovy, aspect, near, far);
	this->_matrix *= matrix;
}

void					OpenGLMatrix::computeProjectionMatrix( float fovy, float aspect, float near, float far )
{
	this->_matrix = createProjectionMatrix(fovy, aspect, near, far);
}

void					OpenGLMatrix::multVector( float &x, float &y, float &z, float &w )
{
	this->_matrix.multVector(x, y, z, w);	
}

Matrix					OpenGLMatrix::createScaleMatrix( float scale ) const
{
	Matrix				matrix;

	matrix[0] = scale;
	matrix[5] = scale;
	matrix[10] = scale;
	return matrix;
}

Matrix					OpenGLMatrix::createRotXMatrix( float alpha ) const
{
	Matrix				matrix;
	float				cosA = cos(alpha);
	float				sinA = sin(alpha);

	matrix[5]  =  cosA;
	matrix[6]  =  sinA;
	matrix[9]  = -sinA;
	matrix[10] =  cosA;
	return matrix;
}

Matrix					OpenGLMatrix::createRotYMatrix( float alpha ) const
{
	Matrix				matrix;
	float				cosA = cos(alpha);
	float				sinA = sin(alpha);

	matrix[0]  =  cosA;
	matrix[2]  = -sinA;
	matrix[8]  =  sinA;
	matrix[10] =  cosA;
	return matrix;
}

Matrix					OpenGLMatrix::createRotZMatrix( float alpha ) const
{
	Matrix				matrix;
	float				cosA = cos(alpha);
	float				sinA = sin(alpha);

	matrix[0]  =  cosA;
	matrix[1]  =  sinA;
	matrix[4]  = -sinA;
	matrix[5]  =  cosA;
	return matrix;
}

Matrix					OpenGLMatrix::createTranslationMatrix( float x, float y, float z ) const
{
	Matrix				matrix;

	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
	return matrix;
}

Matrix					OpenGLMatrix::createProjectionMatrix( float fovy, float aspect, float near, float far ) const
{
	Matrix				matrix;
	float				f;

    f = 1.0f / tanf(fovy / 2.0f);
    matrix[0]  = f / aspect;
    matrix[5]  = f;
    matrix[10] = (far + near)/ (near - far);
    matrix[11] = -1.0f;
    matrix[14] = 2.0f * (far * near) / (near - far);
	matrix[15] = 0.0f;
	return matrix;
}

float				*OpenGLMatrix::getMatrixArray( void )
{
	return this->_matrix.getMatrix();
}

void				OpenGLMatrix::resetMatrix( void )
{
	this->_matrix.identity();
}

OpenGLMatrix&		OpenGLMatrix::multMatrix(const OpenGLMatrix& rhs)
{
	this->_matrix *= rhs.getMatrix();
	return *this;
}
