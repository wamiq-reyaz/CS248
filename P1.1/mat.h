#ifndef __MAT_H__
#define __MAT_H__

#include"vec.h"

template<class T>
class mat {
public:
	mat(void);																					///< default constructor
	mat(const T& m00, const T& m01 = T(0), const T& m02 = T(0), const T& m03 = T(0),			///< initialized constructor
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

protected:
	T	m_data[16];
};

// typedefs
typedef mat<float> matf;
typedef mat<double> matd;

template<class S, class T>
mat<T> operator*(const S& scalar, const vec<T>& M) {
	// TODO -- multiply each component of M with scalar, in a new matrix. return new matrix

	return mat<T>(); // replace this line
}

template<class T>
std::ostream& operator<<(std::ostream& out, const mat<T>& M) {
	// TODO: output a matrix row-by-row to the "out" stream

	return out;
}

template<class T>
std::istream& operator>>(std::istream& in, mat<T>& M) {
	// TODO: read a matrix row-by-row from the "in" stream

	return in;
}



template<class T>
mat<T>::mat(void) {
	// TODO -- initialize m_data with 0s
}

template<class T>
mat<T>::mat(const T& m00, const T& m01, const T& m02, const T& m03,
			const T& m10, const T& m11, const T& m12, const T& m13,
			const T& m20, const T& m21, const T& m22, const T& m23,
			const T& m30, const T& m31, const T& m32, const T& m33) {
	// TODO -- initialize m_data with the provided components.
	//		!! USE operator() to access the elements of this matrix, e.g., (*this)(i,j) = x
}

template<class T>
mat<T>::mat(const mat<T>& other) {
	// TODO -- copy other to (*this) component by component
}

template<class T>
mat<T>::~mat(void) {
}

template<class T>
void mat<T>::identity(void) {
	// TODO -- overwrite this matrix with an identity matrix.
}

template<class T>
mat<T> mat<T>::transpose(void) const {
	// TODO -- compute the transpose of this matrix in a new matrix and return.
	
	return mat<T>(); // replace this line
}

template<class T>
mat<T> mat<T>::operator+(const mat<T>& other) const {
	// TODO -- compute a new matrix (*this)+other, return the new matrix

	return mat<T>(); // replace this line
}

template<class T>
mat<T> mat<T>::operator-(const mat<T>& other) const {
	// TODO -- compute a new matrix (*this)-other, return the new matrix

	return mat<T>(); // replace this line
}

template<class T>
mat<T> mat<T>::operator*(const mat<T>& other) const {
	// TODO -- compute a new matrix (*this) * other, return the new matrix

	return mat<T>(); // replace this line
}

template<class T>
mat<T> mat<T>::operator-(void) const {
	// TODO -- compute a new matrix -(*this), return the new matrix

	return mat<T>(); // replace this line
}

template<class T>
const mat<T>& mat<T>::operator+=(const mat<T>& other) {
	// TODO -- add other to this matrix

	return *this;
}

template<class T>
const mat<T>& mat<T>::operator-=(const mat<T>& other) {
	// TODO -- subtract other from this matrix

	return *this;
}

template<class T>
const mat<T>& mat<T>::operator*=(const mat<T>& other) {
	// TODO -- replace this matrix by (*this) * other. Make sure you do not overwrite elements that you still need.
	//		   You may use mat<T>::operator*()

	return *this;
}

template<class T>
vec<T> mat<T>::operator*(const vec<T>& v) {
	// TODO -- compute the matrix-vector product (*this) * v and return the result

	return vec<T>(); // replace this line
}

template<class T>
template<class S> 
const mat<T>& mat<T>::operator*=(const S& scalar) {
	// TODO -- multiply each matrix component by scalar. Don't forget to convert scalar to type T.

	return *this;
}

template<class T>
template<class S> 
const mat<T>& mat<T>::operator/=(const S& scalar) {
	assert("mat<T>::operator/= -- invalid argument" && T(scalar) != T(0));
	// TODO -- divide each matrix component by scalar. Don't forget to convert scalar to type T.

	return *this;
}

template<class T>
template<class S> 
mat<T> mat<T>::operator*(const S& scalar) {
	// TODO -- compute a new matrix (*this) * scalar. Don't forget to convert scalar to type T.

	return mat<T>(); // replace this line
}

template<class T>
template<class S> 
mat<T> mat<T>::operator/(const S& scalar) {
	assert("mat<T>::operator/ -- invalid argument" && T(scalar) != T(0));
	// TODO -- divide each matrix component by scalar and store in a new matrix. return the new matrix. Don't forget to convert scalar to type T.

	return mat<T>(); // replace this line
}

template<class T>
const mat<T>& mat<T>::operator=(const mat<T>& other) {
	// TODO -- overwrite each component in this matrix by the matching component in other

	return *this;
}

template<class T>
bool mat<T>::operator==(const mat<T>& other) const {
	// TODO -- return true if each component of (*this) is equal to the matching component of other, false otherwise

	return false;
}

template<class T>
bool mat<T>::operator!=(const mat<T>& other) const {
	// TODO -- return false if each component of (*this) is equal to the matching component of other, false otherwise

	return false;
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
mat<T>::operator T*(void) {
	return m_data;
}

#endif