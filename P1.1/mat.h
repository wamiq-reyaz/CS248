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

	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj--){
			// special concern. Make sure to convert S to T to prevent 
			// floats from becoming doubles
			temp(ii, jj) = T(scalar) * M(ii, jj);
		}
	}
	return temp; // do not replace this line
}

template<class T>
std::ostream& operator<<(std::ostream& out, const mat<T>& M) {
	// TODO: output a matrix row-by-row to the "out" stream
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			out << M(ii, jj) << "	"; // print a row with space between elements
		}
		out << std::endl; // add a newline after each row
	}
	return out;
}

template<class T>
std::istream& operator>>(std::istream& in, mat<T>& M) {
	// TODO: read a matrix row-by-row from the "in" stream

	return in;
}



template<class T>
mat<T>::mat(void) {
	// Arithmetic values are set to zero by default. BUt they usually have
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
	// TODO -- initialize m_data with the provided components.
	//		!! USE operator() to access the elements of this matrix, e.g., (*this)(i,j) = x
	// Row 1
	(*this)(0,0) = m00;
	(*this)(0,1) = m01;
	(*this)(0,2) = m02;
	(*this)(0,3) = m03;
	// Row 2
	(*this)(1,0) = m10;
	(*this)(1,1) = m11;
	(*this)(1,2) = m12;
	(*this)(1,3) = m13;
	// Row 3
	(*this)(2,0) = m20;
	(*this)(2,1) = m21;
	(*this)(2,2) = m22;
	(*this)(2,3) = m23;
	// Row 4
	(*this)(3,0) = m30;
	(*this)(3,1) = m31;
	(*this)(3,2) = m32;
	(*this)(3,3) = m33;
}

template<class T>
mat<T>::mat(const mat<T>& other) {
	// TODO -- copy other to (*this) component by component
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
	// TODO -- overwrite this matrix with an identity matrix.
	// Imma do it the simple way. Use two iterators. IF ii==jj
	// put in a 1
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
	// TODO -- compute the transpose of this matrix in a new matrix and return.
	// interchanging the indices should do the trick
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(jj, ii);
		}
	}	
	return temp; // do not replace this line
}

template<class T>
mat<T> mat<T>::operator+(const mat<T>& other) const {
	// TODO -- compute a new matrix (*this)+other, return the new matrix
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) + other(ii, jj);
		}
	}	
	return temp; // do not replace this line
}

template<class T>
mat<T> mat<T>::operator-(const mat<T>& other) const {
	// TODO -- compute a new matrix (*this)-other, return the new matrix
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) - other(ii, jj);
		}
	}	
	return temp; // do not replace this line
}

template<class T>
mat<T> mat<T>::operator*(const mat<T>& other) const {
	// TODO -- compute a new matrix (*this) * other, return the new matrix

	return mat<T>(); // replace this line
}

template<class T>
mat<T> mat<T>::operator-(void) const {
	// TODO -- compute a new matrix -(*this), return the new matrix
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = -(*this)(ii, jj);
		}
	}	
	return temp; // do not replace this line
}

template<class T>
const mat<T>& mat<T>::operator+=(const mat<T>& other) {
	// TODO -- add other to this matrix
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			(*this)(ii, jj) = (*this)(ii, jj) + other(ii, jj);
		}
	}	
	return *this;
}

template<class T>
const mat<T>& mat<T>::operator-=(const mat<T>& other) {
	// TODO -- subtract other from this matrix
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			(*this)(ii, jj) = (*this)(ii, jj) - other(ii, jj);
		}
	}	
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
	vec<T> temp;
	// ii is the iterator for the index of result
	// jj is the iterator for the running sum
	for(int ii = 0; ii < 4; ii++){
		// create a vector from the row ii. Dot product with the RHS vector.
		// we get the iith element of the result
		vec<T> row((*this)(ii,0), (*this)(ii,1), (*this)(ii,2), (*this)(ii,3));
		temp(ii) = row.dot(v);
	}

	return temp; // replace this line
}

template<class T>
template<class S> 
const mat<T>& mat<T>::operator*=(const S& scalar) {
	// TODO -- multiply each matrix component by scalar. Don't forget to convert scalar to type T.
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
	// TODO -- divide each matrix component by scalar. Don't forget to convert scalar to type T.
	for (int ii = 0; ii < 16; ii++) // I am in the class. Im gonna use it
	{
		m_data[ii] /= T(scalar);
	}
	return *this;
}

template<class T>
template<class S> 
mat<T> mat<T>::operator*(const S& scalar) {
	// TODO -- compute a new matrix (*this) * scalar. Don't forget to convert scalar to type T.
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) * T(scalar);
		}
	}	
	return temp; // do not replace this line
}

template<class T>
template<class S> 
mat<T> mat<T>::operator/(const S& scalar) {
	assert("mat<T>::operator/ -- invalid argument" && T(scalar) != T(0));
	// TODO -- divide each matrix component by scalar and store in a new matrix. return the new matrix. Don't forget to convert scalar to type T.
	mat<T> temp;
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			temp(ii, jj) = (*this)(ii, jj) / T(scalar);
		}
	}	
	return temp; // do not replace this line
}

template<class T>
const mat<T>& mat<T>::operator=(const mat<T>& other) {
	// TODO -- overwrite each component in this matrix by the matching component in other
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			(*this)(ii, jj) = other(ii, jj);
		}
	}	
	return *this;
}

template<class T>
bool mat<T>::operator==(const mat<T>& other) const {
	// TODO -- return true if each component of (*this) is equal to the matching component of other, false otherwise
	for(int ii = 0; ii < 4; ii++){
		for(int jj = 0; jj < 4; jj++){
			if((*this)(ii, jj) != other(ii, jj)){
				return false;
			}
		}
	}	
	return true;
}

template<class T>
bool mat<T>::operator!=(const mat<T>& other) const {
	// TODO -- return false if each component of (*this) is equal to the matching component of other, false otherwise
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
mat<T>::operator T*(void) {
	return m_data;
}

#endif