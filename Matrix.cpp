# include "Matrix.hpp"

std::ostream&			operator<<(std::ostream& flux, Matrix const& m)
{
	flux << std::fixed << std::setprecision(5);
	flux << "Matrix:" << std::endl;
	flux << "[ " << std::setw(10) << m[0]  << " " << std::setw(10) << m[4]  << " " 
				 << std::setw(10) << m[8]  << " " << std::setw(10) << m[12] << " ]\n"
		 << "[ " << std::setw(10) << m[1]  << " " << std::setw(10) << m[5]  << " " 
		 		 << std::setw(10) << m[9]  << " " << std::setw(10) << m[13] << " ]\n"
		 << "[ " << std::setw(10) << m[2]  << " " << std::setw(10) << m[6]  << " "
				 << std::setw(10) << m[10] << " " << std::setw(10) << m[14] << " ]\n"
		 << "[ " << std::setw(10) << m[3]  << " " << std::setw(10) << m[7]  << " "
		 		 << std::setw(10) << m[11] << " " << std::setw(10) << m[15] << " ]\n";
	flux << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return flux;
}

Matrix					Matrix::operator*(const Matrix& rhs) const
{
	return Matrix(  rhs[0]*this->_matrix[0]  + rhs[4]*this->_matrix[1]  + rhs[8]*this->_matrix[2]   + rhs[12]*this->_matrix[3],
					rhs[1]*this->_matrix[0]  + rhs[5]*this->_matrix[1]  + rhs[9]*this->_matrix[2]   + rhs[13]*this->_matrix[3], 
					rhs[2]*this->_matrix[0]  + rhs[6]*this->_matrix[1]  + rhs[10]*this->_matrix[2]  + rhs[14]*this->_matrix[3], 
					rhs[3]*this->_matrix[0]  + rhs[7]*this->_matrix[1]  + rhs[11]*this->_matrix[2]  + rhs[15]*this->_matrix[3],
					rhs[0]*this->_matrix[4]  + rhs[4]*this->_matrix[5]  + rhs[8]*this->_matrix[6]   + rhs[12]*this->_matrix[7],
					rhs[1]*this->_matrix[4]  + rhs[5]*this->_matrix[5]  + rhs[9]*this->_matrix[6]   + rhs[13]*this->_matrix[7], 
					rhs[2]*this->_matrix[4]  + rhs[6]*this->_matrix[5]  + rhs[10]*this->_matrix[6]  + rhs[14]*this->_matrix[7], 
					rhs[3]*this->_matrix[4]  + rhs[7]*this->_matrix[5]  + rhs[11]*this->_matrix[6]  + rhs[15]*this->_matrix[7],
					rhs[0]*this->_matrix[8]  + rhs[4]*this->_matrix[9]  + rhs[8]*this->_matrix[10]  + rhs[12]*this->_matrix[11], 
					rhs[1]*this->_matrix[8]  + rhs[5]*this->_matrix[9]  + rhs[9]*this->_matrix[10]  + rhs[13]*this->_matrix[11],
					rhs[2]*this->_matrix[8]  + rhs[6]*this->_matrix[9]  + rhs[10]*this->_matrix[10] + rhs[14]*this->_matrix[11], 
					rhs[3]*this->_matrix[8]  + rhs[7]*this->_matrix[9]  + rhs[11]*this->_matrix[10] + rhs[15]*this->_matrix[11],
					rhs[0]*this->_matrix[12] + rhs[4]*this->_matrix[13] + rhs[8]*this->_matrix[14]  + rhs[12]*this->_matrix[15],
					rhs[1]*this->_matrix[12] + rhs[5]*this->_matrix[13] + rhs[9]*this->_matrix[14]  + rhs[13]*this->_matrix[15],
					rhs[2]*this->_matrix[12] + rhs[6]*this->_matrix[13] + rhs[10]*this->_matrix[14] + rhs[14]*this->_matrix[15],
					rhs[3]*this->_matrix[12] + rhs[7]*this->_matrix[13] + rhs[11]*this->_matrix[14] + rhs[15]*this->_matrix[15] );
}

Matrix&    				Matrix::operator*=(const Matrix& rhs)
{
	*this = *this * rhs;
	return *this;
}

float					Matrix::operator[](int index) const
{
	    return this->_matrix[index];
}

float&					Matrix::operator[](int index)
{
	    return this->_matrix[index];
}

Matrix&					Matrix::operator=(const Matrix & rhs)
{
	(void)rhs;
	return *this;
}

Matrix::Matrix()
{
	identity();
}

Matrix::Matrix(Matrix const & rhs)
{
	*this = rhs;
}

Matrix::Matrix( float m00, float m01, float m02, float m03,
				float m04, float m05, float m06, float m07,
				float m08, float m09, float m10, float m11, 
				float m12, float m13, float m14, float m15 )
{
	this->_matrix[0]  = m00;
	this->_matrix[1]  = m01;
	this->_matrix[2]  = m02;
	this->_matrix[3]  = m03;
	this->_matrix[4]  = m04;
	this->_matrix[5]  = m05;
	this->_matrix[6]  = m06;
	this->_matrix[7]  = m07;
	this->_matrix[8]  = m08;
	this->_matrix[9]  = m09;
	this->_matrix[10] = m10;
	this->_matrix[11] = m11;
	this->_matrix[12] = m12;
	this->_matrix[13] = m13;
	this->_matrix[14] = m14;
	this->_matrix[15] = m15;
}

Matrix::~Matrix()
{
}

Matrix&					Matrix::identity()
{
	this->_matrix[0]  = this->_matrix[5]  = this->_matrix[10] = this->_matrix[15] = 1.0f;
	this->_matrix[1]  = this->_matrix[2]  = this->_matrix[3]  = this->_matrix[4]  =
	this->_matrix[6]  = this->_matrix[7]  = this->_matrix[8]  = this->_matrix[9]  =
	this->_matrix[11] = this->_matrix[12] = this->_matrix[13] = this->_matrix[14] = 0.0f;
	return *this;
}

float					*Matrix::getMatrix()
{
	return this->_matrix;
}

void					Matrix::multVector( float &x, float &y, float &z, float &w )
{
	float nx, ny, nz, nw;

	nx = this->_matrix[0] * x + this->_matrix[4] * y + this->_matrix[8]  * z + this->_matrix[12] * w;
	ny = this->_matrix[1] * x + this->_matrix[5] * y + this->_matrix[9]  * z + this->_matrix[13] * w;
	nz = this->_matrix[2] * x + this->_matrix[6] * y + this->_matrix[10] * z + this->_matrix[14] * w;
	nw = this->_matrix[3] * x + this->_matrix[7] * y + this->_matrix[11] * z + this->_matrix[15] * w;
	x = nx;
	y = ny;
	z = nz;
	w = nw;
}
