#ifndef __VCC_CAMERA_H__
#define __VCC_CAMERA_H__

#include"vec.h"				// Your implementation of 4D vectors
#include"quat.h"			// Your implementation of quaternions
#include"mat.h"				// Your implementation of matrices

template<class T>
class camera {
public:
	camera(void);											///< constructor
	camera(const camera<T>& other);							///< copy-constructor
	~camera(void);											///< destructor

	// member access & update
	const vec<T>&		get_eye(void) const;				///< return the eye vector
	const vec<T>&		get_up(void) const;					///< return the up vector
	const vec<T>&		get_center(void) const;				///< return the center position
	const T&			get_FOV(void) const;				///< return the Field-Of-View in y-direction
	const T&			get_aspect(void) const;				///< return the aspect ratio
	const T&			get_nearplane(void) const;			///< return the near clipping plane
	const T&			get_farplane(void) const;			///< return the far clipping plane
	T					get_distance(void) const;			///< return the eye-to-center-distance

	mat<T>				get_projection(void) const;			///< get the projection matrix
	mat<T>				get_view(void) const;				///< get the view matrix

	void				set_eye(const vec<T>& v);			///< set the eye position
	void				set_up(const vec<T>& v);			///< set the up vector
	void				set_center(const vec<T>& v);		///< set the center (object origin)
	void				set_FOV(const T& fov);				///< set the Field-Of-View in y-direction
	void				set_aspect(const T& aspect);		///< set the aspect ratio
	void				set_nearplane(const T& nearplane);	///< set the near clipping plane
	void				set_farplane(const T& farplane);	///< set the far clipping plane

	// motion
	void				left_right_move(const T& dx);		///< move left (dx<0) and right (dx>0)
	void				up_down_move(const T& dy);			///< move up (dy>0) and down (dy<0)
	void				fwd_back_move(const T& dz);			///< move forward (dz>0) and backward (dz<0)
	void				left_right_rotate(const T& deg);	///< rotate left (deg<0) and right (deg>0)
	void				up_down_rotate(const T& deg);		///< rotate up(deg>0) and down (deg<0)

	const camera<T>&	operator=(const camera<T>& other);	///< assignment operator
protected:
	vec<T>				m_eye;								///< stores the eye position
	vec<T>				m_center;							///< stores the center position
	vec<T>				m_up;								///< stores the up vector
	T					m_fov;								///< stores the field-of-view
	T					m_aspect;							///< stores the aspect ratio
	T					m_nearplane;						///< stores the near clipping plane
	T					m_farplane;							///< stores the far clipping plane
};



// ========================================================================================================================
// BEGIN TODO: Implement the following
// ========================================================================================================================
template<class T>
mat<T> camera<T>::get_projection(void) const {
	// TODO: using m_fov,m_aspect, etc., return a perspective projection matrix
	return mat<T>(); // TODO: replace this line
}

template<class T>
mat<T> camera<T>::get_view(void) const {
	// TODO: using m_eye, m_center, etc., return a view matrix	
	return mat<T>(); // TODO: replace this line
}

template<class T>
void camera<T>::left_right_move(const T& dx) {
	// TODO: move the camera left, right by dx units & update m_eye
	//       To do this, first compute the "right" vector, a local X-axis
}

template<class T>
void camera<T>::up_down_move(const T& dy) {
	// TODO: move the camera up and down by dy units & update m_eye
}

template<class T>
void camera<T>::fwd_back_move(const T& dz) {
	// TODO: move the camera forward and backward by dz units and update m_eye
	//       To do this, first compute a unit Z-axis vector	
}

template<class T>
void camera<T>::left_right_rotate(const T& deg) {
	// TODO: rotate left and right around the local Y-axis.
	//       update only m_eye!	
}

template<class T>
void camera<T>::up_down_rotate(const T& deg) {
	// TODO: rotate up and down around the local X-axis
	//       you will have to compute the X-axis first. Update only m_eye!	
}


// ========================================================================================================================
// END TODO
// ========================================================================================================================



typedef camera<float>	camf;
typedef camera<double>	camd;

template<class T>
camera<T>::camera(void) : m_eye(vec<T>(T(0),T(0),T(0),T(1))), m_center(vec<T>(T(0), T(0), T(0),T(1))), m_up(vec<T>(T(0), T(1), T(0),T(1))), m_fov(T(45)), m_aspect(T(1)), m_nearplane(0.1f), m_farplane(T(100)) {
}

template<class T>
camera<T>::camera(const camera<T>& other) {
	*this = other;
}

template<class T>
camera<T>::~camera(void) {
}

template<class T>
const vec<T>& camera<T>::get_eye(void) const {
	return m_eye;
}

template<class T>
const vec<T>& camera<T>::get_up(void) const {
	return m_up;
}

template<class T>
const vec<T>& camera<T>::get_center(void) const {
	return m_center;
}

template<class T>
const T& camera<T>::get_FOV(void) const {
	return m_fov;
}

template<class T>
const T& camera<T>::get_aspect(void) const {
	return m_aspect;
}

template<class T>
const T& camera<T>::get_nearplane(void) const {
	return m_nearplane;
}

template<class T>
const T& camera<T>::get_farplane(void) const {
	return m_farplane;
}

template<class T>
T camera<T>::get_distance(void) const {
	return (m_eye - m_center).length(3);
}

template<class T>
void camera<T>::set_eye(const vec<T>& v) {
	m_eye = v;
}

template<class T>
void camera<T>::set_up(const vec<T>& v) {
	m_up = v;
}

template<class T>
void camera<T>::set_center(const vec<T>& v) {
	m_center = v;
}

template<class T>
void camera<T>::set_FOV(const T& fov) {
	m_fov = fov;
}

template<class T>
void camera<T>::set_aspect(const T& aspect) {
	m_aspect = aspect;
}

template<class T>
void camera<T>::set_nearplane(const T& nearplane) {
	m_nearplane = nearplane;
}

template<class T>
void camera<T>::set_farplane(const T& farplane) {
	m_farplane = farplane;
}

template<class T>
const camera<T>& camera<T>::operator=(const camera<T>& other) {
	m_eye = other.m_eye;
	m_center = other.m_center;
	m_up = other.m_up;
	m_fov = other.m_fov;
	m_aspect = other.m_aspect;
	m_nearplane = other.m_nearplane;
	m_farplane = other.m_farplane;
	return *this;
}

#endif
