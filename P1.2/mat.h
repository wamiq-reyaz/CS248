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
	// just switch indices so that a_{i,j} becomes a_{j,i}
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
	return temp; // do not replace this line
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

	return temp; // replace this line
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

#endif