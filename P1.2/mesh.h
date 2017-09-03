#ifndef __MESH_H__
#define __MESH_H__

#include"vec.h"
#include<list>
#include<memory>
#include<vector>
#include<string>



class halfedge_t;	// forward declaration so vertex_t can use halfedge_t, as halfedge_t is defined only after vertex_t

class vertex_t {
public:
	vertex_t(void);													///< default constructor
	vertex_t(const vertex_t& other);								///< copy constructor
	~vertex_t(void);												///< destructor
	
	vertex_t* pointer(void);										///< get this pointer (not really necessary, but nice to have)

	// TODO -- why do we return a reference to a pointer? Be prepared to explain to the TA.
	halfedge_t*& halfedge(void);									///< get the associated halfedge, RW
	const halfedge_t* halfedge(void) const;							///< get the associated halfedge, RO

	void clear(void);												///< clear this data (does not deallocate any memory)

	vecd& position(void);											///< RW access to position
	vecd& normal(void);												///< RW access to normal
	vecd& color(void);												///< RW access to color
	vecd& texcoord(void);											///< RW access to texture coordinate

	const vecd& position(void) const;								///< RO access to position
	const vecd& normal(void) const;									///< RO access to normal
	const vecd& color(void) const;									///< RO access to color
	const vecd& texcoord(void) const;								///< RO access to texture coordinate
	
	size_t degree(void) const;										///< compute the degree of this vertex
	bool on_border(void) const;										///< true if vextex on border, false otherwise
			
	const vertex_t& operator=(const vertex_t& other);				///< assignment operator

	static void reset_count(void);									///< DEBUG feature: reset vertex count
#ifdef _DEBUG
public:		size_t				id;									///< DEBUG feature: vertex ID
protected:	static size_t		count;								///< DEBUG feature: vertex count
#endif

protected:	
	vecd			m_position;										///< position of the vertex
	vecd			m_normal;										///< normal of the vertex
	vecd			m_color;										///< color of the vertex
	vecd			m_texcoord;										///< texture coordinate of the vertex
	halfedge_t*		m_halfedge;										///< OUTGOING halfedge
};



class facet_t;	// forward declaration so halfedge_t can use facet_t, as facet_t is defined only after halfedge_t

class halfedge_t {
public:
	halfedge_t(void);												///< default constructor
	halfedge_t(const halfedge_t& other);							///< copy constructor
	~halfedge_t(void);												///< destructor

	halfedge_t* pointer(void);										///< get this pointer

	void clear(void);												///< clear this data (does not deallocate any memory)
	
	halfedge_t*& opposite(void);									///< get the opposite halfedge, RW
	halfedge_t*& next(void);										///< get the next halfedge around the facet, RW
	halfedge_t*& prev(void);										///< get the previous halfedge around the facet, RW
	facet_t*& facet(void);											///< get the incident facet, RW
	vertex_t*& vertex(void);										///< get the associated vertex, RW

	const halfedge_t* opposite(void) const;							///< get the opposite halfedge, RO
	const halfedge_t* next(void) const;								///< get the next halfedge around the facet, RO
	const halfedge_t* prev(void) const;								///< get the previous halfedge around the facet, RO
	const facet_t* facet(void) const;								///< get the incident face, RO
	const vertex_t* vertex(void) const;								///< get the associated vertex, RO

	halfedge_t* next_around_vertex(void);							///< get the next halfedge around the associated vertex, result should not be modified!
	halfedge_t* prev_around_vertex(void);							///< get the previous halfedge around the associated vertex, result should not be modified!

	bool on_border(void) const;										///< true if halfedge on border, false otherwise

	const halfedge_t& operator=(const halfedge_t& other);			///< assignment operator

	static void reset_count(void);									///< DEBUG feature: reset halfedge count
#ifdef _DEBUG
public:		size_t				id;									///< DEBUG feature: halfedge ID
protected:	static size_t		count;								///< DEBUG feature: halfedge count
#endif

protected:
	halfedge_t*		m_opposite;										///< opposite halfedge, does always exist!
	halfedge_t*		m_next;											///< next halfedge around the facet
	halfedge_t*		m_prev;											///< previous halfedge around the facet
	facet_t*		m_facet;										///< incident facet
	vertex_t*		m_vertex;										///< vertex AT THE BEGINNING of the halfedge
};



class facet_t {
public:
	facet_t(void);													///< default constructor
	facet_t(const facet_t& other);									///< copy constructor
	~facet_t(void);													///< destructor

	facet_t* pointer(void);											///< get this pointer

	void clear(void);												///< clear this data (does not deallocate any memory)

	size_t degree(void) const;										///< degree of the facet = number of vertices
	bool on_border(void) const;										///< true if facet at a border, false otherwise
	bool is_triangle(void) const;									///< true if facet is a triangle, false otherwise

	vecd& normal(void);												///< RW access to the face normal
	const vecd& normal(void) const;									///< RO access to the face normal

	halfedge_t*& halfedge(void);									///< get the associated halfedge, RW
	const halfedge_t* halfedge(void) const;							///< get the associated halfedge, RO

	const facet_t& operator=(const facet_t& other);					///< assignment operator

	static void reset_count(void);									///< DEBUG feature: reset facet count
#ifdef _DEBUG
public:		size_t				id;									///< DEBUG feature: facet ID
protected:	static size_t		count;								///< DEBUG feature: facet count
#endif

protected:
	halfedge_t*	m_halfedge;											///< associated halfedge
	vecd		m_normal;											///< face normal
};



class mesh_t {
public:
	mesh_t(void);													///< default constructor
	mesh_t(const mesh_t& other);									///< copy constructor
	~mesh_t(void);													///< destructor

	void clear();													///< clear this mesh and deallocate all memory

	vertex_t*		new_vertex(void);								///< create a new vertex and return its pointer
	halfedge_t*		new_halfedge(void);								///< create a new halfedge and return its pointer
	facet_t*		new_facet(void);								///< create a new facet and return its pointer

	/// Iterators
	typedef std::list<vertex_t>::iterator			vertex_iterator;
	typedef std::list<halfedge_t>::iterator			halfedge_iterator;
	typedef std::list<facet_t>::iterator			facet_iterator;
	typedef std::list<vertex_t>::const_iterator		vertex_const_iterator;
	typedef std::list<halfedge_t>::const_iterator	halfedge_const_iterator;
	typedef std::list<facet_t>::const_iterator		facet_const_iterator;

	vertex_iterator			vertex_begin(void);
	vertex_iterator			vertex_end(void);
	halfedge_iterator		halfedge_begin(void);
	halfedge_iterator		halfedge_end(void);
	facet_iterator			facet_begin(void);
	facet_iterator			facet_end(void);

	vertex_const_iterator	vertex_cbegin(void) const;
	vertex_const_iterator	vertex_cend(void) const;
	halfedge_const_iterator	halfedge_cbegin(void) const;
	halfedge_const_iterator halfedge_cend(void) const;
	facet_const_iterator	facet_cbegin(void) const;
	facet_const_iterator	facet_cend(void) const;

	size_t nVertices(void) const;									///< number of vertices
	size_t nHalfedges(void) const;									///< number of halfedges
	size_t nFacets(void) const;										///< number of facets
	bool has_texcoord(void) const;									///< true if the mesh has texture coordinates, false otherwise

	const mesh_t& operator=(const mesh_t& other);					///< assignment operator

	bool load(const std::string& name);								///< load a simple obj file

	void dbgdump(void);												///< DEBUG feature: if in debug mode, print the mesh information	

	bool check_mesh(void);											///< TODO -- your implementation must pass this check!

protected:
	std::list<vertex_t>		m_vertices;								///< the vertices
	std::list<halfedge_t>	m_halfedges;							///< the halfedges
	std::list<facet_t>		m_facets;								///< the facets
	bool					m_texcoord;								///< does the mesh have texture coordinates?

	// Called by load, will build the halfedge data structure. TODO -- implement this!
	bool build_mesh(const std::vector<vecd>& in_vertex, const std::vector<vecd>& in_texcoord, const std::vector<std::vector<size_t> >& in_facets);
};

#endif