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

template<class S, class T>
vec<T> operator*(const S& scalar, const vec<T>& v) {
	vec<T> temp;
	for(int ii = 0; ii < 4; ii++){
		temp(ii) = T(scalar) * v(ii); // Type conversion is necessary
	}

	return temp; // do not replace this line
}

template<class T>
std::ostream& operator<<(std::ostream& out, const vec<T>& v) {
	out << "[ " ;
	for(int ii = 0; ii < 4; ii++){
		out << v(ii) << ", ";
	}
	out << "]" ;
	return out;
}

template<class T>
std::istream& operator>>(std::istream& in, vec<T>& v) {
	for(int ii = 0; ii < 4; ii++){
		in >> v(ii);
	}
	return in;
}

// Default Constructor
template<class T>
vec<T>::vec(void) {
	for(int ii = 0; ii < 4; ii++){
		m_data[ii] = T(0);
	}
}

// Other constructor
template<class T>
vec<T>::vec(const T& x, const T& y, const T& z, const T& w) {
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
	m_data[3] = w;
}

// Copy constructor
template<class T>
vec<T>::vec(const vec<T>& other) {
	for(int ii = 0; ii < 4; ii++){
		m_data[ii] = other.m_data[ii];
	}
}

template<class T>
vec<T>::~vec(void) {
	// nothing is done on the heap. No cleanup needed
}

template<class T>
T vec<T>::length2(size_t n) const {
	// ensures that we dont compute the length from 8 elements when only 4 exist
	assert("vec<T>::length2() -- invalid argument" && n < 5);
	if(n == 0){
		return T(0);
	}
	else{
		T l2 = 0;
		for(int ii = 0; ii < n; ii++){
			l2 += pow(m_data[ii], 2);
		}

	return l2; 
	}
}

template<class T>
T vec<T>::length(size_t n) const {
	assert("vec<T>::length() -- invalid argument" && n < 5);
	// T because we need a return type T
	if(n == 0){
		return T(0);
	}
	else{
		return T(sqrt(this->length2(n))); 
	}
}

template<class T>
void vec<T>::normalize(size_t n) {
	assert("vec<T>::normalize() -- invalid argument" && n < 5);
	if(this->length2(n) == 0){
		return; // do nothing if length = 0
	}
	else{
		// find the length of the first n components
		T length_n = this->length(n);
		// divide each of the n components by that length
		for(int ii = 0; ii < n; ii++){
			m_data[ii] /= length_n;
		}
		return;
	}
}

template<class T>
T vec<T>::dot(const vec<T>& other, size_t n) const {
	assert("vec<T>::dot() -- invalid argument" && n < 5);
	T dot_product = 0;
	for(int ii = 0; ii < n; ii++){
		dot_product += (m_data[ii] * other(ii));
	}
	return dot_product; 
}

template<class T>
vec<T> vec<T>::cross(const vec<T>& other) const {
	// We implement the cross product and the last element is 0 (by default)
	T result[3]; // the components of the cross product
	T first[3];// the components of the first vector
	T second[3]; // the components of the second vector
	for(int ii = 0; ii < 3; ii++){ // index is 3 because only 3 elements are needed
		first[ii] = m_data[ii];
		second[ii] = other(ii);
	}
	result[0] = first[1] * second[2] - first[2] * second[1];
	result[1] = first[2] * second[0] - first[0] * second[2];
	result[2] = first[0] * second[1] - second[0] * first[1];

	return vec<T>(result[0], result[1], result[2]); 
}

template<class T>
const vec<T>& vec<T>::operator+=(const vec<T>& other) {
	for(int ii = 0; ii < 4; ii++){
		m_data[ii] += other(ii);
	}

	return *this; // Standard C++ technique. It means that we now point to our updated self.
}

template<class T>
const vec<T>& vec<T>::operator-=(const vec<T>& other) {
	for(int ii = 0; ii < 4; ii++){
		m_data[ii] -= other(ii);
	}
	return *this;
}

template<class T>
vec<T> vec<T>::operator-(void) const {
	vec<T> temp;
	for(int ii = 0; ii < 4; ii++){
		temp(ii) = -m_data[ii];
	}
	return temp; 
}

template<class T>
vec<T> vec<T>::operator+(const vec<T>& other) const {
	vec<T> temp;
	for(int ii = 0; ii < 4; ii++){
		temp(ii) = m_data[ii] + other(ii);
	}

	return temp; 
}

template<class T>
vec<T> vec<T>::operator-(const vec<T>& other) const {
	vec<T> temp;
	for(int ii = 0; ii < 4; ii++){
		temp(ii) = m_data[ii] - other(ii);
	}

	return temp; 
}

template<class T>
template<class S> 
const vec<T>& vec<T>::operator*=(const S& scalar) {
	for(int ii = 0; ii < 4; ii++){
		m_data[ii] = m_data[ii] * T(scalar); // Again, a type conversion
	}

	return *this;
}

template<class T>
template<class S> 
const vec<T>& vec<T>::operator/=(const S& scalar) {
	// WHY To maintain same accuracy levels. Example: int(0) != float(0)
	assert("vec<T>::operator/= -- invalid argument" && T(scalar) != T(0));
	for(int ii = 0; ii < 4; ii++){
		m_data[ii] /= T(scalar);
	}
	return *this;
}

template<class T>
template<class S> 

vec<T> vec<T>::operator*(const S& scalar) {
	vec<T> temp;
	for(int ii = 0; ii < 4; ii++){
		temp(ii) = m_data[ii] * T(scalar);
	}

	return temp; //do not replace this line
}

template<class T>
template<class S> 
vec<T> vec<T>::operator/(const S& scalar) {
	assert("vec<T>::operator/ -- invalid argument" && T(scalar) != T(0));
	vec<T> temp;
	for(int ii = 0; ii < 4; ii++){
		temp(ii) = m_data[ii] / T(scalar);
	}
	return temp; 
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
	for(int ii = 0; ii < 4; ii++){
		if(fabs(m_data[ii] - other(ii)) > 1e-5){ // if difference is greater than 1e-5
												 // it is more than floating point error
			//return false immediately
			return false;
		}
	}

	//otherwise all elements are almost equal
	return true; // do not replace this line
}

template<class T>
bool vec<T>::operator!=(const vec<T>& other) const {
	if(*this == other){
		return false;
	}
	else{
		return true;
	} 
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

template<class T>
vec<T>::operator T*(void) {
	return m_data;
}

template<class T>
vec<T>::operator const T*(void) const {
	return m_data;
}

#endif