#include"model.h"
#include<numeric>
#include<algorithm>

model::model(const std::string& name) : m_bIsOk(false) {
	load(name);
}

model::model(const model& other) {
	*this = other;
}

model::~model(void) {
	clear();
}

void model::clear(void) {
	m_vertices.clear();
	m_colors.clear();
	m_normals.clear();
	m_texcoords.clear();
	m_bIsOk = false;
	m_blurb.clear();
	m_name.clear();
	m_group_names.clear();
	m_group_name_to_index.clear();
	m_group_faces.clear();	
}

const bool& model::is_ok(void) const {
	return m_bIsOk;
}

bool model::load(const std::string& name) {
	clear();
	if (name.empty()) {
		m_blurb = std::string("model::load() - name was empty");
		return false;
	}
	std::ifstream input(name.c_str());
	if (input.fail()) {
		m_blurb = std::string("model::load('") + name + std::string("') - file not found");
		return false;
	}

	std::vector<face_t> faces;

	m_name = name;
	std::string curgroup = name;
	
	size_t curgroupID = m_group_name_to_index[curgroup] = m_group_names.size();
	m_group_names.push_back(name);
	m_group_faces.push_back(std::vector<int>());

	while (!input.eof()) {
		std::string line;
		std::getline(input, line);
		std::istringstream line_input(line);
		std::string keyword;
		line_input >> keyword;
		if (keyword == "v") {
			vecf v;
			line_input >> v(0) >> v(1) >> v(2);
			for (size_t n = 0; n < 3; n++) m_vertices.push_back(v(n));
			float val;
			vecf clr;
			size_t pos = 0;
			while (line_input >> val && pos < 3) { // check for colors
				clr(pos++) = val;
			}
			if (pos != 3 && pos != 0) {
				m_blurb = std::string("model::load() - parse error on line'") + line + std::string("'");
				return false;
			}
			else {
				if (pos == 3) {
					for (int n = 0; n < 3; n++) m_colors.push_back(clr(n));
				}
			}
			continue;
		}
		if (keyword == "vt") {
			vecf v;
			line_input >> v(0) >> v(1);
			for (size_t n = 0; n < 2; n++) m_texcoords.push_back(v(n));
			continue;
		}
		if (keyword == "vn") {
			vecf v;
			line_input >> v(0) >> v(1) >> v(2);
			for (size_t n = 0; n < 3; n++) m_normals.push_back(v(n));
			continue;
		}
		if (keyword == "o") {
			line_input >> m_name;
			continue;
		}
		if (keyword == "f"){
			face_t f;
			struct custom_whitespace : std::ctype<char> {
				custom_whitespace() : std::ctype<char>(get_table()) {}
				static mask const* get_table(void) {
					static mask rc[table_size];
					rc['/']  = std::ctype_base::space;
					rc[' ']  = std::ctype_base::space;
					rc['\n'] = std::ctype_base::space;
					rc['\t'] = std::ctype_base::space;
					return &rc[0];
				}
			};

			line_input.imbue(std::locale(line_input.getloc(), new custom_whitespace));

			// figure out version			
			std::vector<int> data;
			int val;
			while (line_input >> val) {
				data.push_back(val-1);
			}
			switch (data.size()) {
			case 3:
				// f vtx vtx vtx
				for (int n = 0; n < 3; n++) f.vtx_index[n] = data[n];
				break;
			case 6:
			{
				size_t pos = line.find_first_of("/");
				if (pos != std::string::npos) {
					if (line[pos + 1] == '/') {
						// f vtx//nml vtx//nml vtx//nml
						for (int n = 0; n < 3; n++) {
							f.vtx_index[n] = data[2 * n];
							f.nml_index[n] = data[2 * n + 1];
						}
					}
					else {
						// f vtx/tex vtx/tex vtx/tex
							for (int n = 0; n < 3; n++) {
								f.vtx_index[n] = data[2 * n];
								f.tex_index[n] = data[2 * n + 1];
							}
					}
				}
				else {
					m_blurb = std::string("model::load() - parse error on line'") + line + std::string("'");
					return false;
				}
				break;
			}
			case 9:
				// f vtx/tex/nml vtx/tex/nml vtx/tex/nml
				for (int n = 0; n < 3; n++) {
					f.vtx_index[n] = data[3 * n];
					f.tex_index[n] = data[3 * n + 1];
					f.nml_index[n] = data[3 * n + 2];
				}
				break;
			default:
				m_blurb = std::string("model::load() - parse error on line '") + line + std::string("'");
				return false;
			}
			faces.push_back(f);									// store this face in preliminary storage

			continue;
		} // endif keyword == "f"
	} // end while

	m_bIsOk = generate_clean_model(faces);
	return m_bIsOk;	
}

bool model::remove_degenerate_faces(std::vector<face_t>& faces) {
	std::vector<size_t> deg_faces;
	bool result = false;
	for (size_t n = 0; n < faces.size(); n++) {
		for (int i = 0; i < 3; i++) {
			if (faces[n].vtx_index[i] == faces[n].vtx_index[(i + 1) % 3] || faces[n].vtx_index[i] == faces[n].vtx_index[(i + 2) % 3]) {
				deg_faces.push_back(n);
				result = true;
			}
		}
	}
	if (result) {
		if (!m_blurb.empty()) m_blurb += std::string("\n");
		m_blurb += std::string("model::load() - face(s) with identical vertices removed");
		size_t count = 0;
		for (size_t k = 0; k < deg_faces.size(); k++) {
			faces.erase(faces.begin() + (deg_faces[k] - count));
			count++;
		}
	}
	// TODO: update groups
	return result;
}

bool model::remove_invalid_faces(std::vector<face_t>& faces) {
	std::vector<size_t> inv_faces;
	bool result = false;
	for (size_t n = 0; n < faces.size(); n++) {
		for (int i = 0; i < 3; i++) {
			if (faces[n].vtx_index[i] >= int(nVertices())) {
				inv_faces.push_back(n);
				result = true;
			}
			if (faces[n].tex_index[i] != -1 && faces[n].tex_index[i] >= int(m_texcoords.size() / 2)) {
				inv_faces.push_back(n);
				result = true;
			}
			if (faces[n].nml_index[i] != -1 && faces[n].nml_index[i] >= int(m_normals.size() / 3)) {
				inv_faces.push_back(n);
				result = true;
			}
		}
	}
	if (result) {
		if (!m_blurb.empty()) m_blurb += std::string("\n");
		m_blurb += std::string("model::load() - face(s) with invalid references removed");
		size_t count = 0;
		for (size_t k = 0; k < inv_faces.size(); k++) {
			faces.erase(faces.begin() + (inv_faces[k] - count));
			count++;
		}
	}
	// TODO: update groups
	return result;
}

void model::split_nodes(std::vector<face_t>& faces) {
	// 1. enumerate nodes and generate faces
	struct node_t {
		node_t(int _vtx, int _tex, int _nml) : vtx(_vtx), tex(_tex), nml(_nml) {}
		int vtx, tex, nml;
		bool operator<(const node_t& other) const {
			if (vtx < other.vtx) return true;
			if (vtx > other.vtx) return false;
			if (tex < other.tex) return true;
			if (tex > other.tex) return false;
			return nml < other.nml;
		}
	};
	std::map<node_t, unsigned int> node_list;
	unsigned int count = 0;
	for (size_t n = 0; n < faces.size(); n++) {
		for (size_t k = 0; k < 3; k++) {
			node_t thisNode(faces[n].vtx_index[k], faces[n].tex_index[k], faces[n].nml_index[k]);
			if (node_list.find(thisNode) == node_list.end()) {
				node_list[thisNode] = count;
				m_faces.push_back(count);
				count++;
			}
			else {
				m_faces.push_back(node_list[thisNode]);
			}
		}
	}
	// 2. build nodes
	std::vector<float> new_vertices;
	std::vector<float> new_normals;
	std::vector<float> new_texcoords;
	for (std::map<node_t, unsigned int>::const_iterator it = node_list.begin(); it != node_list.end(); it++) {
		if (it->first.vtx != -1) {
			if (new_vertices.size()<3*(it->second+1)) new_vertices.resize(3 * (it->second + 1));
			for (int k = 0; k < 3; k++) {
				new_vertices[3 * it->second + k] = m_vertices[3 * it->first.vtx + k];
			}
		}
		if (it->first.tex != -1) {
			if (new_texcoords.size()<2*(it->second+1)) new_texcoords.resize(2 * (it->second + 1));
			for (int k = 0; k < 2; k++) {
				new_texcoords[2 * it->second + k] = m_texcoords[2 * it->first.tex + k];
			}
		}
		if (it->first.nml != -1) {
			if (new_normals.size()<3*(it->second+1)) new_normals.resize(3 * (it->second + 1));
			for (int k = 0; k < 3; k++) {
				new_normals[3 * it->second + k] = m_normals[3 * it->first.nml + k];
			}
		}				
	}
	m_vertices = new_vertices;
	m_texcoords = new_texcoords;
	m_normals = new_normals;
}

bool model::generate_clean_model(std::vector<face_t>& faces) {
	remove_degenerate_faces(faces);			// remove faces with multiple vertices
	remove_invalid_faces(faces);			// remove faces containing invalid references
	if (faces.empty()) {
		m_blurb = std::string("model::generate_clean_model() - no faces found");
		return false;
	}
	split_nodes(faces);						// split nodes into vertices and store information in m_faces
	return true;
}

size_t model::nVertices(void) const {
	return m_vertices.size()/3;
}

size_t model::nFaces(void) const {
	return m_faces.size()/3;
}

bool model::has_colors(void) const {
	return !m_colors.empty();
}

bool model::has_normals(void) const {
	return !m_normals.empty();
}
bool model::has_texture(void) const {
	return !m_texcoords.empty();
}


const std::string& model::get_blurb(void) const {
	return m_blurb;
}

const float* model::get_vertex_data(void) const {
	return m_vertices.data();
}

const float* model::get_color_data(void) const {
	return m_colors.data();
}

const float* model::get_normal_data(void) const {
	return m_normals.data();
}

const float* model::get_texcoord_data(void) const {
	return m_texcoords.data();
}

const int* model::get_face_data(void) const {
	return m_faces.data();
}

void model::get_minmax(vecf& _min, vecf& _max) {
	for (int k = 0; k < 3; k++) {
		_min(k) = std::numeric_limits<float>::infinity();
		_max(k) = -std::numeric_limits<float>::infinity();
	}
	for (size_t n = 0; n < m_vertices.size(); n += 3) {
		for (size_t k = 0; k < 3; k++) {
			_min(k) = std::min<float>(m_vertices[n + k], _min(k));
			_max(k) = std::max<float>(m_vertices[n + k], _max(k));
		}
	}
}

void model::rescale(float scale) {
	vecf _min, _max;
	get_minmax(_min, _max);	
	vecf delta;
	for (size_t k = 0; k < 3; k++) delta(k) = _max(k) - _min(k);
	float maxdelta = std::max<float>(delta(0), std::max<float>(delta(1), delta(2)));
	scale = scale / maxdelta;
	for (size_t n = 0; n < m_vertices.size(); n++) {
		m_vertices[n] *= scale;
	}

}

void model::center(void) {
	vecf _min, _max;
	get_minmax(_min, _max);
	vecf center;
	for (size_t k = 0; k < 3; k++) center(k) = 0.5f*(_max(k)+ _min(k));
	for (size_t n = 0; n < m_vertices.size(); n+=3) {
		for (size_t k = 0; k < 3; k++) {
			m_vertices[n + k] -= center(k);
		}
	}
}
	
const model& model::operator=(const model& other) {
	m_vertices = other.m_vertices;
	m_colors = other.m_colors;
	m_normals = other.m_normals;
	m_texcoords = other.m_texcoords;
	m_bIsOk = other.m_bIsOk;
	m_group_names = other.m_group_names;
	m_group_name_to_index = other.m_group_name_to_index;
	m_group_faces = other.m_group_faces;
	m_blurb = other.m_blurb;
	m_name = other.m_name;
	return *this;
}

void model::generate_colors(void) {
	if (m_colors.size() != m_vertices.size()) m_colors.resize(m_vertices.size());
	if (has_normals()) {
		for (size_t n = 0; n < m_normals.size(); n++) {
			m_colors[n] = 0.5f*m_normals[n] + 0.5f;
		}
	}
	else {
		vecf _min, _max;
		get_minmax(_min, _max);
		vecf s;
		for (int k = 0; k < 3; k++) {
			s(k) = _max(k) - _min(k);
			if (s(k) != 0.0f) s(k) = 1.0f / s(k);
		}
		for (size_t n = 0; n < m_vertices.size(); n += 3) {
			for (size_t k = 0; k < 3; k++) {
				m_colors[n + k] = (m_vertices[n + k] - _min(k))*s(k);
			}
		}
	}
}

void model::generate_normals(void) {
	std::vector<vecf> new_normals;
	new_normals.resize(m_vertices.size(), vecf(0.0f, 0.0f, 0.0f));
	for (size_t n = 0; n < m_faces.size(); n += 3) {
		vecf pos[3];
		for (size_t k = 0; k < 3; k++) {
			for (size_t j = 0; j < 3; j++) {
				pos[k](j) = m_vertices[3 * m_faces[n + k] + j];
			}
		}
		pos[1] -= pos[0];
		pos[2] -= pos[0];
		vecf N = pos[1].cross(pos[2]);
		for (size_t k = 0; k < 3; k++) {
			new_normals[m_faces[n + k]] += N;
		}
	}
	m_normals.clear();
	for (size_t n = 0; n < new_normals.size(); n++) {
		new_normals[n].normalize();
		for (size_t k = 0; k < 3; k++) m_normals.push_back(new_normals[n](k));
	}
}