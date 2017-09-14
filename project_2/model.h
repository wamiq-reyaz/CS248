#ifndef __VCC_MODEL_H__
#define __VCC_MODEL_H__

#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<cassert>
#include<vector>
#include<map>
#include"vec.h"

class model {
public:
	model(const std::string& name = std::string());
	model(const model& other);
	~model(void);
	void							clear(void);
	const bool&						is_ok(void) const;
	bool							load(const std::string& name);
	size_t							nVertices(void) const;		
	size_t							nFaces(void) const;
	bool							has_colors(void) const;
	bool							has_normals(void) const;
	bool							has_texture(void) const;
	const std::string&				get_blurb(void) const;
	const float*					get_vertex_data(void) const;
	const float*					get_color_data(void) const;
	const float*					get_normal_data(void) const;
	const float*					get_texcoord_data(void) const;
	const int*						get_face_data(void) const;

	void							generate_colors(void);
	void							generate_normals(void);

	void							rescale(float scale);
	void							center(void);

	const model&					operator=(const model& other);

protected:
	std::vector<std::string>		m_group_names;
	std::map<std::string, size_t>	m_group_name_to_index;
	std::vector<std::vector<int> >	m_group_faces;
	std::vector<float>				m_vertices;
	std::vector<float>				m_colors;
	std::vector<float>				m_normals;
	std::vector<float>				m_texcoords;
	std::vector<int>				m_faces;
	bool							m_bIsOk;
	std::string						m_blurb;
	std::string						m_name;		

	struct face_t{
		face_t(void) {
			for (int i = 0; i < 3; i++) vtx_index[i] = tex_index[i] = nml_index[i] = -1;
		}
		int vtx_index[3];
		int tex_index[3];
		int nml_index[3];
	};
	
	bool							generate_clean_model(std::vector<face_t>& faces);
	bool							remove_degenerate_faces(std::vector<face_t>& faces);		// remove faces with identical vertices
	bool							remove_invalid_faces(std::vector<face_t>& faces);			// remove faces with non-existent references
	void							remove_empty_groups(void);
	void							split_nodes(std::vector<face_t>& faces);					// split notes to vertices
	void							get_minmax(vecf& _min, vecf& _max);							// retrieves range of vertices
};

#endif
