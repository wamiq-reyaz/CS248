#ifndef __VEC_H__
#define __VEC_H__	// TODO -- why is this here? Be prepared to explain to the TA.
					// to prevent inclusion twice or thrice...or infinitely many times

#include<iostream>
#include<cfloat>
#include<cmath>
#include<cassert>


/// four-dimensional vector class
template<class T>
class vec {
public:
	vec(void);																	///< default constructor
	// do not use x = T(0). definition clashes with vec(void)
	vec(const T& x, const T& y = T(0), const T& z = T(0), const T& w = T(0));	///< initialized constructor
	// Simply gen a 'vec' from another one
	vec(const vec<T>& other);													///< copy constructor
	~vec(void);																	///< destructor

	// the following have an argument n, to allow 2D and 3D vector operations while using an actual 4D vector
	T length2(size_t n=4) const;												///< squared length of the first n components of the vector
	T length(size_t n = 4) const;												///< length of the first n components of the vector
	void normalize(size_t n=4);													///< normalize the first n components of the vector
	T dot(const vec<T>& other, size_t n = 4) const;								///< compute the dotproduct between this and other, first n components only
	vec<T> cross(const vec<T>& other) const;									///< compute the crossproduct between the first three components of this and other
	
	// operators -- vector-vector
	const vec<T>& operator+=(const vec<T>& other);								///< cumulative addition
	const vec<T>& operator-=(const vec<T>& other);								///< cumulative subtraction
	vec<T> operator-(void) const;												///< negation (unary operator)
	vec<T> operator+(const vec<T>& other) const;								///< addition
	vec<T> operator-(const vec<T>& other) const;								///< subtraction	

	// operators -- vector-scalar
	// Here the S is the of the scalar type.
	template<class S> const vec<T>& operator*=(const S& scalar);				///< multiplication by scalar
	template<class S> const vec<T>& operator/=(const S& scalar);				///< division by scalar
	template<class S> vec<T> operator*(const S& scalar);						///< vector times scalar
	template<class S> vec<T> operator/(const S& scalar);						///< vector divided by scalar
	const vec<T> operator=(const vec<T>& other);								///< assignment operator
	bool operator==(const vec<T>& other) const;									///< equality operator
	bool operator!=(const vec<T>& other) const;									///< inequality operator

	// element access
	// for use outside the class
	T& operator()(size_t n);													///< RW access to element
	const T& operator()(size_t n) const;										///< RO access to element

	// use as T* type for OpenGL interoperability
	operator T*(void);															///< RW access to pointer
	operator const T*(void) const;												///< RO access to pointer

protected:
	T	m_data[4];																///< data array
};

/// typedefs
typedef vec<float>	vecf;
typedef vec<double>	vecd;

// TODO -- why is the following needed? Be prepared to explain to the TA
// the scalars can be of different types
template<class S, class T>
vec<T> operator*(const S& scalar, const vec<T>& v) {
	// TODO -- multiply each component of v with scalar, in a new vector. return new vector
	vec<T> temp;
	for(int i = 0; i < 4; i++){
		temp(i) = T(scalar) * v(i); // Type conversion is necessary
	}

	return temp; // do not replace this line
}

template<class T>
std::ostream& operator<<(std::ostream& out, const vec<T>& v) {
	// TODO: output a vector component-wise to the "out" stream
	out << v(0) << " " << v(1) << " " << v(2) << " " << v(3) << std::endl;
	return out;
}

template<class T>
std::istream& operator>>(std::istream& in, vec<T>& v) {
	// TODO: read a vector component-wise from the "in" stream

	return in;
}

// Default Constructor
template<class T>
vec<T>::vec(void) {
	// TODO -- initialize m_data with 0s
	for(int ii = 0; ii < 4; ii++){
		m_data[ii] = T(0);
	}
}

// Other constructor
template<class T>
vec<T>::vec(const T& x, const T& y, const T& z, const T& w) {
	// TODO -- initialize m_data with x,y,z,w. What does the "= T(0)" in the class definition do? Be prepared to explain to the TA.
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
	m_data[3] = w;

}

// Copy constructor
template<class T>
vec<T>::vec(const vec<T>& other) {
	// TODO -- copy contents of other to this vector
	for(int i = 0; i < 4; i++){
		m_data[i] = other.m_data[i];
	}
}

template<class T>
vec<T>::~vec(void) {
	// TODO -- is there anything to do here? Be prepared to explain to the TA.

}

template<class T>
T vec<T>::length2(size_t n) const {
	// TODO -- what does the following line do? Be prepared to explain to the T
	// ensures that we do nt compute the length from 8 elements when only 4 exist
	assert("vec<T>::length2() -- invalid argument" && n < 5);
	// TODO -- compute squared length of the first n components of the vector
	if(n == 0){
		return T(0);
	}
	else{
		T l2 = 0;
		for(int i = 0; i < n; i++){
			l2 += pow(m_data[i], 2);
		}

	return l2; // do not replace this line
	}
}

template<class T>
T vec<T>::length(size_t n) const {
	assert("vec<T>::length() -- invalid argument" && n < 5);
	// TODO -- compute the length of the first n components of the vector
	// T because we need a return type T
	if(n == 0){
		return 0;
	}
	else{
		return T(sqrt(this->length2(n))); // do not replace this line
	}
}

template<class T>
void vec<T>::normalize(size_t n) {
	assert("vec<T>::normalize() -- invalid argument" && n < 5);
	// TODO -- if length(n)==0, do nothing, otherwise normalize first n components, leave rest intact
}

template<class T>
T vec<T>::dot(const vec<T>& other, size_t n) const {
	assert("vec<T>::dot() -- invalid argument" && n < 5);
	// TODO -- compute dot product between this vector and other, first n components only
	T dot_product = 0;
	for(int i = 0; i < n; i++){
		dot_product += (m_data[i] * other(i));
	}
	return dot_product; // replace this line. TODO: why use T(0) instead of 0? Be prepared to explain to the TA.
}

template<class T>
vec<T> vec<T>::cross(const vec<T>& other) const {
	///< TODO -- compute the crossproduct between the first three components of this and other

	return vec<T>(); // replace this line
}

template<class T>
const vec<T>& vec<T>::operator+=(const vec<T>& other) {
	// TODO -- add other to this vector component-wise, store in this vector
	for(int i = 0; i < 4; i++){
		m_data[i] += other(i);
	}

	return *this; // TODO -- why would you return a reference to *this? Be prepared to explain to the TA.
				  // Standard C++ technique. It means that we now point to our updated self. I think.
}

template<class T>
const vec<T>& vec<T>::operator-=(const vec<T>& other) {
	// TODO -- subtract other from this vector component-wise, store in this vector
	for(int i = 0; i < 4; i++){
		m_data[i] -= other(i);
	}
	return *this;
}

template<class T>
vec<T> vec<T>::operator-(void) const {
	// TODO -- why can't we return a reference, here? Be prepared to explain to the TA.
	// TODO -- return a new vector in which each component is the negated component from this vector

	return vec<T>(); // replace this line
}

template<class T>
vec<T> vec<T>::operator+(const vec<T>& other) const {
	// TODO -- return a new vector in which each component is the sum of the components of this vector and other

	return vec<T>(); // replace this line
}

template<class T>
vec<T> vec<T>::operator-(const vec<T>& other) const {
	// TODO -- return a new vector in which each component is the difference between the components of this vector and other

	return vec<T>(); // replace this line
}

template<class T>
template<class S> 
const vec<T>& vec<T>::operator*=(const S& scalar) {
	// TODO -- replace each component of this with the matching component of this multiplied with the scalar
	//         Make sure to convert the scalar from S to T
	for(int i = 0; i < 4; i++){
		m_data[i] = m_data[i] * T(scalar); // Again, a type conversion
	}

	return *this;
}
template<class T>
template<class S> 
const vec<T>& vec<T>::operator/=(const S& scalar) {
	// TODO -- why do we convert scalar and 0 to type T and not compare it as type S? Be prepared to explain to the TA.
	// WHY To maintain same accuracy levels. Example: int(0) != float(0)
	assert("vec<T>::operator/= -- invalid argument" && T(scalar) != T(0));
	// TODO -- replace each component of this with the matching component of this multiplied with the scalar
	//         Make sure to convert the scalar from S to T
	for(int i = 0; i < 4; i++){
		m_data[i] /= T(scalar);
	}
	return *this;
}

template<class T>
template<class S> 
vec<T> vec<T>::operator*(const S& scalar) {
	// TODO -- return a new vector in which each component equals the matching component of this vector times the scalar
	//		   Make sure to convert the scalar from S to T
	vec<T> temp;
	for(int i = 0; i < 4; i++){
		temp(i) = m_data[i] * T(scalar);
	}

	return temp; // replace this line
}

template<class T>
template<class S> 
vec<T> vec<T>::operator/(const S& scalar) {
	assert("vec<T>::operator/ -- invalid argument" && T(scalar) != T(0));
	// TODO -- return a new vector in which each component equals the matching component of this vector divided by the scalar
	vec<T> temp;
	for(i = 0; i < 4; i++){
		temp(i) = m_data[i] / T(scalar);
	}
	return temp; // replace this line
}

template<class T>
const vec<T> vec<T>::operator=(const vec<T>& other) {
	// TODO -- overwrite each component in this vector with the matching component of other.
	for(int i = 0; i < 4; i++){
		m_data[i] = other(i);
	}

	return *this;
}

template<class T>
bool vec<T>::operator==(const vec<T>& other) const {
	// TODO -- return true if each component of this and other are equal, false otherwise

	return false; // replace this line
}

template<class T>
bool vec<T>::operator!=(const vec<T>& other) const {
	// TODO -- return false if each component of this and other are equal, false otherwise

	return false; // replace this line
}

template<class T>
T& vec<T>::operator()(size_t n) {
	// Really simple bounds check
	assert("vec<T>::operator() -- invalid argument" && n < 4);
	return m_data[n];
}

template<class T>
const T& vec<T>::operator()(size_t n) const {
	// Really simple bounds check
	assert("vec<T>::operator() const -- invalid argument" && n < 4);
	return m_data[n];
}

// TODO -- what does this method do? Be prepared to explain to the TA.
template<class T>
vec<T>::operator T*(void) {
	return m_data;
}

template<class T>
vec<T>::operator const T*(void) const {
	return m_data;
}

#endif