#ifndef __QUAT_H__
#define __QUAT_H__

#include"vec.h"
#include<cassert>

// TODO:
//	Implement the following. A quaternion is stored in a 4D vector. Since it is a generalization of imaginary numbers,
//	some operators will be overloaded with a different meaning. In particular, it should be possible to cast a quaternion
//	into a 4D vector, in which case we want vector math (not quaternion math) to apply. Be prepared to explain to the TA
//	if the vec<T> class has to be changed using the "virtual" keyword in this case and why or why not.



// CONVENTIONS:
//	The real part is stored in the vector's first component, the imaginary parts follow.

template<class T>
class quat : public vec<T> {
public:
	quat(const T& r = T(1), const T& i = T(0), const T& j = T(0), const T& k = T(0));		///< default and initialized constructor
	quat(const vec<T>& other);																///< convert a vector into a quaternion 
	quat(const quat<T>& other);																///< copy-constructor
	~quat(void);																			///< destructor
	
	
	quat<T>	operator* (const quat<T>& other) const;											///< Hamilton product between two quaternions
	quat<T> inverse(void) const;															///< Inverse quaternion
	const quat<T>& operator*=(const quat<T>& other);										///< Multiplication with another quaternion
	quat<T> conjugate(void) const;															///< Conjugate quaternion

	const T& real(void) const;																///< Real part of the quaternion, RO access
	const T& imaginary(size_t n) const;														///< Imaginary part of the quaternion, RO access
	T& real(void);																			///< Real part of the quaternion, RW access
	T& imaginary(size_t n);																	///< Imaginary part of the quaternion, RW access	
};

// TODO -- IMPLEMENT THE FOLLOWING ================================================================

template<class T>
quat<T> quat<T>::operator*(const quat<T>& other) const {
	// TODO: implement the Hamilton product between two quaternions
	// see also: https://en.wikipedia.org/wiki/Quaternion
	vec<T> temp;
	temp[0] = this->m_data[0]*other.m_data[0] - this->m_data[1]*other.m_data[1]
			- this->m_data[2]*other.m_data[2] - this->m_data[3]*other.m_data[3];

	temp[1] = this->m_data[0]*other.m_data[1] + this->m_data[1]*other.m_data[0]
			+ this->m_data[2]*other.m_data[3] - this->m_data[3]*other.m_data[2];

	temp[2] = this->m_data[0]*other.m_data[2] - this->m_data[1]*other.m_data[3]
			+ this->m_data[2]*other.m_data[0] + this->m_data[3]*other.m_data[1];

	temp[3] = this->m_data[0]*other.m_data[3] + this->m_data[1]*other.m_data[2]
			- this->m_data[2]*other.m_data[1] + this->m_data[3]*other.m_data[0];

	return quat<T>(temp); 
}

template<class T>
quat<T> quat<T>::inverse(void) const {
	// TODO: implement the inverse of a quaternion
	return quat<T>(); // TODO: replace this line
}

template<class T>
const quat<T>& quat<T>::operator*=(const quat<T>& other) {
	// TODO: implement the Hamilton product as cumulative operator
	return *this;
}

template<class T>
quat<T> quat<T>::conjugate(void) const {
	// TODO: implement the conjugate of a quaternion
	return quat<T>(); // TODO: replace this line
}

// END TODO =======================================================================================

template<class T>
quat<T>::quat(const T& r, const T& i, const T& j, const T& k) : vec<T>(){
	this->m_data[0] = r;
	this->m_data[1] = i;
	this->m_data[2] = j;
	this->m_data[3] = k;
}

template<class T>
quat<T>::quat(const vec<T>& other) : vec<T>(other) {
}

template<class T>
quat<T>::~quat(void) {
}

template<class T>
quat<T>::quat(const quat<T>& other) {
	*this = other;
}

template<class T>
const T& quat<T>::real(void) const {
	return this->m_data[0];
}

template<class T>
const T& quat<T>::imaginary(size_t n) const {
	assert("quat<T>::imaginary() const -- invalid argument" && n < 3);
	return this->m_data[n + 1];
}

template<class T>
T& quat<T>::real(void) {
	return this->m_data[0];
}

template<class T>
T& quat<T>::imaginary(size_t n) {
	assert("quat<T>::imaginary() const -- invalid argument" && n < 3);
	return this->m_data[n + 1];
}

// add visual difference between vectors and quarternions
template<class T>
std::ostream& operator<<(std::ostream& out, const quat<T>& 	q) {
	out << "[q " ;
	out << q.real() << ", ";
	for(int ii = 0; ii < 3; ii++){
		out << q.imaginary(ii) << ", ";
	}
	out << "]" ;
	return out;
}

#endif
