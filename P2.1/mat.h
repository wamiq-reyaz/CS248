#ifndef __MAT_H__
#define __MAT_H__

#include"vec.h"
#include"quat.h"
#include <cmath>
#include <iomanip>
#include <list>

#define _USE_MATH_DEFINES // for M_PI which is pi

template<class T>
class mat {
public:
	mat(void);																					///< default constructor
	explicit mat(const T& m00, const T& m01 = T(0), const T& m02 = T(0), const T& m03 = T(0),			///< initialized constructor
		const T& m10 = T(0), const T& m11 = T(0), const T& m12 = T(0), const T& m13 = T(0),
		const T& m20 = T(0), const T& m21 = T(0), const T& m22 = T(0), const T& m23 = T(0),
		const T& m30 = T(0), const T& m31 = T(0), const T& m32 = T(0), const T& m33 = T(0));
	mat(const mat<T>& other);																	///< copy constructor
	~mat(void);																					///< destructor

	void identity(void);																		///< overwrite this matrix with identity
	mat<T> transpose(void) const;																///< compute the transposed matrix

	// operators -- matrix-matrix
	mat<T> operator+(const mat<T>& other) const;												///< addition
	mat<T> operator-(const mat<T>& other) const;												///< subtraction
	mat<T> operator*(const mat<T>& other) const;												///< multiplication
	mat<T> operator-(void) const;																///< negation
	
	const mat<T>& operator+=(const mat<T>& other);												///< cumulative addition
	const mat<T>& operator-=(const mat<T>& other);												///< cumulative subtraction
	const mat<T>& operator*=(const mat<T>& other);												///< cumulative multiplication

	// operators -- matrix-vector
	vec<T> operator*(const vec<T>& v);															///< matrix-vector product

	// operators -- matrix-scalar
	template<class S> const mat<T>& operator*=(const S& scalar);								///< matrix-scalar product
	template<class S> const mat<T>& operator/=(const S& scalar);								///< matrix-scalar division
	template<class S> mat<T> operator*(const S& scalar);										///< matrix-scalar product
	template<class S> mat<T> operator/(const S& scalar);										///< matrix-scalar division

	// assignment operators and (in)equality
	const mat<T>& operator=(const mat<T>& other);												///< assignment
	bool operator==(const mat<T>& other) const;													///< equality
	bool operator!=(const mat<T>& other) const;													///< inequality

	// access components
	T& operator()(size_t i, size_t j);															///< RW access to components
	const T& operator()(size_t i, size_t j) const;												///< RO access to components
	
	// casting operator for OpenGL interoperability
	operator const T*(void) const;																///< cast to const T*
	operator T*(void);																			///< cast to T*

	/// ADDED for Project 2
	//=================================//

	// Be prepared to answer what the static keyword does to the TA.
	static mat<T> Zero(void);
	static mat<T> Identity(void);

	// see https://www.opengl.org/documentation/specs/version2.0/glspec20.pdf
	static mat<T> Translation(const vec<T>& t);
	static mat<T> Rotation(const T& deg, const vec<T>& axis);
	static mat<T> Ortho(const T& l, const T& r, const T& b, const T& t, const T& n, const T& f);
	static mat<T> Frustum(const T& l, const T& r, const T& b, const T& t, const T& n, const T& f);
	static mat<T> Scale(const vec<T>& s);
		
	// see https://www.opengl.org/wiki/
	static mat<T> Perspective(const T& fov, const T& aspect, const T& nplane, const T& fplane);
	static mat<T> Lookat(const vec<T>& eye, const vec<T>& center, const vec<T>& up);
	static mat<T> Crossproduct(const vec<T>& v);

	// quaternion to matrix
	// Don't forget to include quat.h in mat.h
	const mat<T>& operator=(const quat<T>& q);
	mat<T>(const quat<T>& q);

	// determinant and inverse
	T subdeterminant(size_t i, size_t j) const;
	T determinant(void) const;
	mat<T> adjoint(void) const;
	mat<T> inverse(T* det = NULL) const;
	mat<T> inverse_transpose(T* det = NULL) const;

protected:
	T	m_data[16];
};

// typedefs
typedef mat<float> matf;
typedef mat<double> matd;

template<class S, class T>
mat<T> operator*(const S& scalar, const vec<T>& M) {
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj--){
			// special concern. Make sure to convert S to T to prevent 
			// floats from becoming doubles
			temp(ii, jj) = T(scalar) * M(ii, jj);
		}
	}
	return temp; //
}

template<class T>
std::ostream& operator<<(std::ostream& out, const mat<T>& M) {
	out.precision(3); // not too many digits

	out << "[m\n";
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			out << std::setw (6) << M(ii, jj) << "	"; // print a row with space between elements
		}
		out << std::endl; // add a newline after each row
	}
	out << "]";
	return out;
}

template<class T>
std::istream& operator>>(std::istream& in, mat<T>& M) {
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			in >> M(ii, jj); 
		}
	}
	return in;
}



template<class T>
mat<T>::mat(void) {
	// Arithmetic values are set to zero by default. But they usually have
	// a small value 
	for(int ii = 0; ii < 16; ii++){
		m_data[ii] = T(0);
	}
}

template<class T>
mat<T>::mat(const T& m00, const T& m01, const T& m02, const T& m03,
			const T& m10, const T& m11, const T& m12, const T& m13,
			const T& m20, const T& m21, const T& m22, const T& m23,
			const T& m30, const T& m31, const T& m32, const T& m33) {

	// Column 1
	(*this)(0,0) = m00;
	(*this)(0,1) = m01;
	(*this)(0,2) = m02;
	(*this)(0,3) = m03;
	// Column 2
	(*this)(1,0) = m10;
	(*this)(1,1) = m11;
	(*this)(1,2) = m12;
	(*this)(1,3) = m13;
	// Column 3
	(*this)(2,0) = m20;
	(*this)(2,1) = m21;
	(*this)(2,2) = m22;
	(*this)(2,3) = m23;
	// Column 4
	(*this)(3,0) = m30;
	(*this)(3,1) = m31;
	(*this)(3,2) = m32;
	(*this)(3,3) = m33;
}

template<class T>
mat<T>::mat(const mat<T>& other) {
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			(*this)(ii, jj) = other(ii, jj);
		}
	}
}

template<class T>
mat<T>::~mat(void) {
	// nothing to do. Everything allocated on the stack
}

template<class T>
void mat<T>::identity(void) {
	// Imma do it the simple way. Use two iterators. IF ii==jj
	// put in a 1, otherwise 0
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			T element = 0;
			if(ii == jj){
				element = 1;
			}
			// Now put in the element
			(*this)(ii, jj) = element;
		}
	}
}

template<class T>
mat<T> mat<T>::transpose(void) const {
	// just switch indices so that a_{i,j} becomes a_{j,i}
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(jj, ii); 
		}
	}	
	return temp; 
}

template<class T>
mat<T> mat<T>::operator+(const mat<T>& other) const {
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) + other(ii, jj);
		}
	}	
	return temp; 
}

template<class T>
mat<T> mat<T>::operator-(const mat<T>& other) const {
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) - other(ii, jj);
		}
	}	
	return temp; 
}

template<class T>
mat<T> mat<T>::operator*(const mat<T>& other) const{
	mat<T> temp;
	// iterate over all rows and columns using ii, jj. kk is used as
	// dummy variable for summing
	for (int ii = 0; ii < 4; ii++) {
		for(int jj = 0; jj < 4; jj++) {
            for(int kk = 0; kk < 4; kk++) {
				// work in doubles for better accuracy
                temp(ii, jj) += T(double((*this)(ii, kk)) * double((other)(kk, jj)));
			}
		}
	}
	return temp; 
}

template<class T>
mat<T> mat<T>::operator-(void) const {
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = -(*this)(ii, jj);
		}
	}	
	return temp; 
}

template<class T>
const mat<T>& mat<T>::operator+=(const mat<T>& other) {
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			(*this)(ii, jj) = (*this)(ii, jj) + other(ii, jj);
		}
	}	
	return *this;
}

template<class T>
const mat<T>& mat<T>::operator-=(const mat<T>& other) {
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			(*this)(ii, jj) = (*this)(ii, jj) - other(ii, jj);
		}
	}	
	return *this;
}

template<class T>
const mat<T>& mat<T>::operator*=(const mat<T>& other) {
	mat<T> temp;
	for (int ii = 0; ii < 4; ii++) {
        for(int jj = 0; jj < 4; jj++) {
            for(int kk = 0; kk < 4; kk++) {
                temp(ii, jj) += (*this)(ii, kk) * other(kk, jj);
			}
		}
	}
	for (int ii = 0; ii < 4; ii++) {
        for(int jj = 0; jj < 4; jj++) {
			(*this)(ii, jj) = temp(ii, jj);
		}
	}
	return *this;
}

template<class T>
vec<T> mat<T>::operator*(const vec<T>& v) {
	vec<T> temp;
	// ii is the iterator for the index of result
	for(int ii = 0; ii < 4; ii++){
		// create a vector from the row ii. Dot product with the RHS vector.
		// we get the iith element of the result
		vec<T> row((*this)(ii,0), (*this)(ii,1), (*this)(ii,2), (*this)(ii,3));
		temp(ii) = row.dot(v);
	}

	return temp;
}

template<class T>
template<class S> 
const mat<T>& mat<T>::operator*=(const S& scalar) {
	for (int ii = 0; ii < 16; ii++) // I am in the class. Im gonna use it
	{
		m_data[ii] *= T(scalar);
	}
	return *this;
}

template<class T>
template<class S> 
const mat<T>& mat<T>::operator/=(const S& scalar) {
	assert("mat<T>::operator/= -- invalid argument" && T(scalar) != T(0));
	for (int ii = 0; ii < 16; ii++) // I am in the class. Im gonna use it
	{
		m_data[ii] /= T(scalar);
	}
	return *this;
}

template<class T>
template<class S> 
mat<T> mat<T>::operator*(const S& scalar) {
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) * T(scalar);
		}
	}	
	return temp; 
}

template<class T>
template<class S> 
mat<T> mat<T>::operator/(const S& scalar) {
	assert("mat<T>::operator/ -- invalid argument" && T(scalar) != T(0));
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) / T(scalar);
		}
	}	
	return temp; 
}

template<class T>
const mat<T>& mat<T>::operator=(const mat<T>& other) {
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			(*this)(ii, jj) = other(ii, jj);
		}
	}	
	return *this;
}

template<class T>
bool mat<T>::operator==(const mat<T>& other) const {
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			if((*this)(ii, jj) != other(ii, jj)){
				return false;
			}
		}
	}	
	// if every check succeeds
	return true;
}

template<class T>
bool mat<T>::operator!=(const mat<T>& other) const {
	if(*this == other){
		return false;
	}
	else{
		return true;
	}
}

template<class T>
T& mat<T>::operator()(size_t i, size_t j) {
	assert("mat<T>::operator() -- invalid arguments" && i < 4 && j < 4);
	return m_data[i + 4 * j];
}

template<class T>
const T& mat<T>::operator()(size_t i, size_t j) const{
	assert("mat<T>::operator() const -- invalid arguments" && i < 4 && j < 4);
	return m_data[i + j * 4];
}

template<class T>
mat<T>::operator const T*(void) const {
	return m_data;
}

template<class T>
mat<T>::operator T*(void){
	return m_data;
}

///// ADDED for Project 2////
//====================================//

// Be prepared to answer what the static keyword does to the TA.
template<class T>
mat<T> mat<T>::Zero(void){
	return mat<T>(); // by default zero
}

template<class T>
mat<T> mat<T>::Identity(void){
	return mat<T>(1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
				  0, 0, 0, 1);
}

// see https://www.opengl.org/documentation/specs/version2.0/glspec20.pdf
template<class T>
mat<T> mat<T>::Translation(const vec<T>& t){
	return mat<T>(1, 0, 0, t(0),
				  0, 1, 0, t(1),
				  0, 0, 1, t(2),
				  0, 0, 0, 1);
}
template<class T>
mat<T> mat<T>::Rotation(const T& deg, const vec<T>& axis){
	//according to the spec, we need to normalize axis
	// axis.normalize();
	
	// We are gonna let u, v, w be the axis components
	T u = axis(0);
	T v = axis(1);
	T w = axis(2);

	// we also need sines and cosines, and in DEGREES
	T c = cos(deg*M_PI/180);
	T s = sin(deg*M_PI/180);

	// from https://dspace.lboro.ac.uk/2134/18050
	T m11 = u*u + (v*v + w*w)*c;
	T m12 = u*v*(1 - c) - w*s;
	T m13 = u*w*(1 - c) + v*s;

	T m21 = u*v*(1 - c) + w*s;
	T m22 = v*v + (u*u + w*w)*c;
	T m23 = v*w*(1 - c) - u*s;

	T m31 = u*w*(1 - c) - v*s;
	T m32 = v*w*(1 - c) + u*s;
	T m33 = w*w + (u*u + v*v)*c;

	return mat<T>(m11, m12, m13, 0,
				  m21, m22, m23, 0,
				  m31, m32, m33, 0,
				  0,   0,   0,   1);
}

template<class T>
mat<T> mat<T>::Ortho(const T& l, const T& r, const T& b, const T& t, const T& n, const T& f){
	T m11 = 2/(r-l);
	T m14 = -(r+l)/(r-l);

	T m22 = 2/(t-b);
	T m24 = -(t+b)/(t-b);

	T m33 = -2/(f -n);
	T m34 = -(f+n)/(f-n);

	return mat<T>(m11,   0,   0,  m14,
				  0,   m22,   0,  m24,
				  0,     0, m33,  m34,
				  0,     0,   0,    1);
}

template<class T>
mat<T> mat<T>::Frustum(const T& l, const T& r, const T& b, const T& t, const T& n, const T& f){
	T m11 = 2*n/(r-l);
	T m13 = (r+l)/(r-l);

	T m22 = 2*n/(t-b);
	T m23 = (t+b)/(t-b);

	T m33 = -(f+n)/(f-n);
	T m34 = -2*n*f/(f-n);

	return mat<T>(m11,   0, m13,   0,
					0, m22, m23,   0,
					0,   0, m33, m34,
				    0,   0,  -1,   0);
}

template<class T>
mat<T> mat<T>::Scale(const vec<T>& s){
	return mat<T>(s(0),   0,    0,   0,
				  0,    s(1),   0,   0,
				  0,      0,  s(2),  0,
				  0,      0,    0,   1);
}

// see https://www.opengl.org/wiki/
template<class T>
mat<T> mat<T>::Perspective(const T& fov, const T& aspect, const T& nplane, const T& fplane){
	T f = 1/tan(fov/2); 

	T m11 = f/aspect;

	T m22 = f;

	T m33 = (fplane + nplane)/(nplane-fplane);
	T m34 = (2*nplane*fplane)/(nplane-fplane);	

	return mat<T>(m11,   0,   0,   0,
					0, m22,   0,   0,
					0,   0, m33, m34,
					0,   0,  -1,   0);
}

template<class T>
mat<T> mat<T>::Lookat(const vec<T>& eye, const vec<T>& center, const vec<T>& up){
	// The z points from the center to the camera (eye).
	vec<T> z = (eye - center);
	z.normalize();

	vec<T> approx_up = up; // up itself is a const
	approx_up.normalize();

	vec<T> x = approx_up.cross(z);
	x.normalize();

	vec<T> y = z.cross(x);

	mat<T> change_of_basis(x(0), x(1), x(2), 0, 
				  		   y(0), y(1), y(2), 0,
						   z(0), z(1), z(2), 0,
							 0,    0,    0,  1);
							 
	// the final matrix is given by first translating the eye onto
	// the origin, and then using change of basis.
	return (const mat<T>) change_of_basis * mat<T>::Translation(-eye);
}

template<class T>
mat<T> mat<T>::Crossproduct(const vec<T>& v){
	return mat<T>(   0, -v(2),  v(1), 0,
				  v(2), 	0, -v(0), 0,
				 -v(1),  v(0),     0, 0,
				     0,     0,     0, 1);
}

// quaternion to matrix
// Don't forget to include quat.h in mat.h
template<class T>
const mat<T>& mat<T>::operator=(const quat<T>& q){
	// dont think a check for self assignment is needed
	// the two objects have different classes
	mat<T> temp(q);
	*this = temp;
	return *this;
}

template<class T>
mat<T>::mat(const quat<T>& q){
	// normalize the components of the quarternion
	T norm = q.length();
	T r = q.real() / norm;
	T i = q.imaginary(0) / norm;
	T j = q.imaginary(1) / norm;
	T k = q.imaginary(2) / norm;

	// now construct the matrix
	// from http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/
	(*this)(0,0) = 1 - 2*j*j - 2*k*k;
	(*this)(0,1) = 2*i*j - 2*k*r;
	(*this)(0,2) = 2*i*k + 2*j*r;
	(*this)(0,3) = 0;

		(*this)(1,0) = 2*i*j + 2*k*r;
	(*this)(1,1) = 1 - 2*i*i - 2*k*k;
	(*this)(1,2) = 2*j*k - 2*i*r;
	(*this)(1,3) = 0;

	(*this)(2,0) = 2*i*k - 2*j*r;
	(*this)(2,1) = 2*j*k + 2*i*r;
	(*this)(2,2) = 1 - 2*i*i - 2*j*j;
	(*this)(2,3) = 0;

	(*this)(3,0) = 0;
	(*this)(3,1) = 0;
	(*this)(3,2) = 0;
	(*this)(3,3) = 1;
}

// determinant and inverse
template<class T>
T mat<T>::subdeterminant(size_t i, size_t j) const{
	// convert to a 3x3 matrix
	std::list<size_t> row;
	std::list<size_t> column;

	row.push_back(0); row.push_back(1); row.push_back(2); row.push_back(3);
	column.push_back(0); column.push_back(1); column.push_back(2); column.push_back(3);
	
	row.remove(i); column.remove(j); // we dont need these columns and rows

	// set up the iterators
	size_t ii, jj; ii = jj = 0;

	// and the submatrix
	T M[3][3];

	// find the determinant of the new matrix	
	for(std::list<size_t>::iterator it_row = row.begin(); it_row != row.end(); it_row++){
		// std::cout << "\nThe iterator it_row is here:" << *it_row << std::endl;
		jj = 0;
		for(std::list<size_t>::iterator it_col = column.begin(); it_col != column.end(); it_col++){
			// else copy the elements to the new submatrix
			// std::cout << "it_col is here:" << *it_col << " ";
			M[ii][jj] = (*this)(*it_row, *it_col); // 
			jj++; //onto the next column
		}
		ii++; // onto the next row
	}

	T det = 0;
	det += M[0][0] * (M[1][1]*M[2][2] - M[1][2]*M[2][1])\
		-  M[0][1] * (M[1][0]*M[2][2] - M[1][2]*M[2][0])\
		+  M[0][2] * (M[1][0]*M[2][1] - M[1][1]*M[2][0]);

	return det;
}

template<class T>
T mat<T>::determinant(void) const{
	// let's expand along the first row
	T det = 0;
	int sign = 1;
	for(size_t ii = 0; ii < 4; ii++){
		det += sign * (*this)(0, ii) * this->subdeterminant(0, ii);
		sign *= -1; // flip the sign for the next one
	}
	return det;
}

template<class T>
mat<T> mat<T>::adjoint(void) const{
	mat<T> temp;
	// faster lookups (hopefully, it will be cached)
	mat<T> signs( 1, -1,  1, -1,
    	 		 -1,  1, -1,  1,
				  1, -1,  1, -1,
				 -1,  1, -1,  1);

	for(size_t ii = 0; ii < 4; ii++){ // row
		for(size_t jj = 0; jj < 4; jj++){ // column
			// for the adjoint, we actually need the transpose of the matrix of cofactors
			// That is given by the subdeterminant
			temp(ii, jj) = signs(ii, jj) * this->subdeterminant(ii, jj);
		}
	}
	return temp.transpose();
}

template<class T>
mat<T> mat<T>::inverse(T* det) const{
	if(det == NULL){
		return mat<T>(); // a zero initialized matrix
	}
	else{
		*det = this->determinant();
		return this->adjoint()/ *det;
	}
}

template<class T>
mat<T> mat<T>::inverse_transpose(T* det) const{
	if(det == NULL){
		return mat<T>(); // a zero initialized matrix
	}
	else{
		*det = this->determinant();
		return this->adjoint().transpose()/ *det;
	}
}

#endif