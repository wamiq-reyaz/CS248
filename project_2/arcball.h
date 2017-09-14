#ifndef __VCC_ARCBALL_H__
#define __VCC_ARCBALL_H__

#include<algorithm>		// for std::min and std::max
#include"vec.h"			// Your implementation of 4D vectors
#include"quat.h"		// Your implementation of quaternions
#include"mat.h"			// Your implementation of matrices
#include"camera.h"		// Your implementation of the camera

template<class T>
class arcball {
public:
	arcball(const int& winwidth=T(0), const int& winheight=T(0));						///< default and initialized constructor
	arcball(const arcball<T>& other);													///< copy constructor
	~arcball(void);																		///< destructor

	void					update_window(const int& winwidth, const int& winheight);	///< update to new window dimensions
	void					click(const int& x, const int& y);							///< called when a mouse click event happens
	void					drag(const int& x, const int& y);							///< called when the mouse is dragged
	void					release(void);												///< called when the mouse button is released
	const bool&				is_clicked(void) const;										///< true iff mouse button is clicked
	quat<T>					get_orientation(void) const;								///< return the orientation of the object
	const arcball<T>&		operator=(const arcball<T>& other);							///< assignment operator
protected:
	vec<T>					m_click;													///< storage for the 3D click position
	vec<int>				m_view_dim;													///< 2D view dimensions, winwidth x winheight
	quat<T>					m_orientation;												///< quaternion representing the orientation of the object
	quat<T>					m_old_orientation;											///< old orientation before the current click-drag-release
	bool					m_bIsClicked;												///< true if clicked

	vec<T>					get_sphere_position(const int& x, const int& y) const;		///< returns the 3D position on the sphere
};

typedef arcball<float>	arcballf;
typedef arcball<double>	arcballd;

template<class T>
arcball<T>::arcball(const int& winwidth, const int& winheight) : m_bIsClicked(false) {
	update_window(winwidth, winheight);
}

template<class T>
arcball<T>::arcball(const arcball<T>& other) {
	*this = other;
}

template<class T>
arcball<T>::~arcball(void) {
	m_bIsClicked = false;
}

template<class T>
const bool& arcball<T>::is_clicked(void) const {
	return m_bIsClicked;
}

template<class T>
const arcball<T>& arcball<T>::operator=(const arcball<T>& other) {
	m_click = other.m_click;
	m_view_dim = other.m_view_dim;
	m_orientation = other.m_orientation;
	m_old_orientation = other.m_old_orientation;
	return *this;
}

// ========================================================================================================================
// BEGIN TODO: Implement the following
// ========================================================================================================================

template<class T>
void arcball<T>::update_window(const int& winwidth, const int& winheight) {
	// TODO: store winwidth, winheight in m_view_dim. If m_viewdim(n)==0, store 1 instead	
}

template<class T>
void arcball<T>::click(const int& x, const int& y) {
	m_bIsClicked = true;
	// TODO: click event - store 3d position corresponding to click at x,y in m_click	
}

template<class T>
vec<T> arcball<T>::get_sphere_position(const int& x, const int& y) const {
	// TODO: given x,y, get a corresponding position on the 3D sphere.
	// 1) convert x,y to normalized device coordinates; clip coordinates to [-1,1]^3
	// 2) if xy outside sphere, project onto sphere
	// 3) Compute third component
	// 4) return result
	return vec<T>(); // TODO: replace this line
}

template<class T>
void arcball<T>::drag(const int& x, const int& y) {
	// TODO: implement the drag event. After a click at m_clicked, the mouse is dragged to xy.
	// 1) compute sphere position for xy
	// 2) convert m_click and current sphere position into quaternions
	// 3) using quaternion math, compute a rotation from m_click to drag event
	//    mind the previous orientation in m_old_orientation
	// 4) store the result in m_orientation
	assert("arcball::drag() - is not clicked" && m_bIsClicked);	
}

template<class T>
void arcball<T>::release(void) {
	m_bIsClicked = false;
	// TODO: update m_old_orientation with m_orientation now that the mouse button is released
}

template<class T>
quat<T> arcball<T>::get_orientation(void) const {
	// TODO: return the current orientation
	return quat<T>(); // TODO: replace this line
}

// ========================================================================================================================
// END TODO
// ========================================================================================================================

#endif
