#include "mesh.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#ifdef _DEBUG
size_t vertex_t::count = 0;
size_t halfedge_t::count = 0;
size_t facet_t::count = 0;
#endif

/// YOUR TODO LIST:

vecd gen_normal_facet(facet_t* f){
    vecd normal;
    // gen a warning if facet not triangle
    if(f->degree() != 3){
        std::cerr << "WARNING: Not a triangle" << std::endl;
    }
    halfedge_t* h = f->halfedge();
    halfedge_t* p = h->prev();
    halfedge_t* n = h->next();
 
    // if A, B, C are the vertices of a triangle, area = 0.5*AB*BC
    // AB = pointB- pointA; BC = pointC - pointB
    vecd v1 = h->vertex()->position() - n->vertex()->position();
    vecd v2 = p->vertex()->position() - n->vertex()->position();

    return 0.5 * v2.cross(v1);
}

vecd gen_normal_vertex(vertex_t* v){
    /* Returns the normalized normal vecd for the vertex v */
    vecd normal;
    halfedge_t* h = v->halfedge();
    do{
        // sum of all normals of adjacent faces
        h = h->opposite()->next();
        if(h->facet() != nullptr){
            normal += h->facet()->normal();
        }
    } while(h != v->halfedge());

    normal.normalize();
    return normal;
}

// TASK 3:	Implement the edge_t data structure. This is used later in a map
// to retrieve edges. By convention, the smaller vertex index is stored in v1
// and the larger one in v2.
struct edge_t {
    edge_t(size_t _v1, size_t _v2) {
        // TODO -- constructor. The smaller value of _v1,_v2 goes to v1, the
        // larger to v2
        v1 = _v1 > _v2 ? _v2 : _v1; // one-liner instead of if-else
        v2 = _v1 > _v2 ? _v1 : _v2; // never gonna be =
    }
    size_t v1, v2;
    bool operator==(const edge_t& other) const {
        // Two edges are identical if
        // both their indices are identical.
        if (this->v1 == other.v1 && this->v2 == other.v2) {
            return true;
        }
        return false; // TODO
    }
};

// Now implement this data structure to allow edge_t to be used in
// std::unordered_map
namespace std {
template <> struct hash< edge_t > {
    std::size_t operator()(const edge_t& e) const {
        /* Use Fermats theorem. a^3 + b^3 = c^3 + d^3 has no solution in Z.
        */
        return e.v1 * e.v1 * e.v1 + e.v2 * e.v2 * e.v2;
    }
};
}

bool mesh_t::build_mesh(const std::vector< vecd >& in_vertex, /// input positions
                        const std::vector< vecd >& in_texcoord, /// input texcoord, if any
                        const std::vector< std::vector< size_t > >&
                            in_facet // input facets, std::vector<size_t> per face
                        ) {
    if (in_vertex.empty()) {
        std::cerr << "Error - mesh contains no vertices" << std::endl;
        return false;
    }
    if (in_facet.empty()) {
        std::cerr << "Error - mesh contains no facets" << std::endl;
        return false;
    }
    m_texcoord = !in_texcoord.empty();
    if (in_texcoord.size() != in_vertex.size() && m_texcoord) {
        std::cerr << "Warning - mismatch between texture coordinates and vertices "
                     "(ignored)."
                  << std::endl;
        m_texcoord = false;
    }

    // check all faces for consistency
    for (size_t f = 0; f < in_facet.size(); f++) {
        for (size_t n = 0; n < in_facet[n].size(); n++) {
            if (in_facet[f][n] >= in_vertex.size()) {
                std::cerr << "Error - invalid vertex reference" << std::endl;
                return false;
            }
        }
    }

    clear();

    // TASK 4a
    std::vector< vertex_t* > vertex_list;
    vertex_list.reserve(in_vertex.size());

    for (size_t n = 0; n < in_vertex.size(); n++) {
        // TODO - For each vertex, create a new vertex. Add this new vertex to
        // the vertex_list.Then, fill the position and (if available) the texture
        // coordinate.

        vertex_t* curr_vert = new_vertex(); // add vertex to mesh, return pointer
        vertex_list.push_back(curr_vert);

        curr_vert->position() = in_vertex[n];
        if (m_texcoord) {
            curr_vert->texcoord() = in_texcoord[n];
        }
    }

    // We'll keep track of the edges already visited in this list. They are not
    // yet halfedges, so edge v1,v2 is the same as edge v2,v1
    std::unordered_map< edge_t, halfedge_t* > edge_list;

    // a storage for the facet normals
    std::vector<vecd> normal_list;
    normal_list.reserve(in_facet.size());

    facet_t* curr_face;
    for (size_t n = 0; n < in_facet.size(); n++) {
        // Generate a new facet in the mesh.
        curr_face = new_facet();
        // Keep track of the first, current, and previous halfedge generated for
        // this face.
        halfedge_t *first, *curr, *prev;
        first = curr = prev = nullptr;

        // TASK 4c: foreach edge
        size_t n_edges = in_facet[n].size();
        for (size_t ii = 0; ii < n_edges; ii++) {
            // generate edges from facet
            size_t v1 = in_facet[n][ii];
            size_t v2 = in_facet[n][(ii + 1) % n_edges]; // to wrap around and get to
                                                        // zero for last vertex

            halfedge_t *he_parallel, *he_anti_parallel;
            edge_t edge(v1, v2);
            // locate edge in edge_list
            std::unordered_map< edge_t, halfedge_t* >::iterator map_it =
                edge_list.find(edge);
            if (map_it == edge_list.end()) {
                // if edge not in edge_list
                // create opposite pair of halfedges, update pointers as per
                // problem statement, keep track of first, curr, prev
                he_parallel = new_halfedge();
                he_anti_parallel = new_halfedge();

                if (ii == 0) {
                    first = he_parallel;
                }
                prev = curr;
                curr = he_parallel;

                edge_list[edge] = he_anti_parallel; // add to map

                // fill the pointers
                he_parallel->opposite() = he_anti_parallel;
                he_parallel->facet() = curr_face;
                he_parallel->vertex() = vertex_list[v2];
                // the opposite vertex points to the outgoing halfedge
                he_parallel->vertex()->halfedge() = he_anti_parallel;

                he_anti_parallel->opposite() = he_parallel;
                he_anti_parallel->vertex() = vertex_list[v1];
                he_anti_parallel->vertex()->halfedge() = he_parallel;
            } else {
                // else keep track of first, curr, prev
                if (ii == 0) {
                    first = map_it->second;
                }
                prev = curr;
                curr = map_it->second;

                curr->facet() = curr_face;
            }
            // update facet pointers
            curr_face->halfedge() = curr;
            // update prev, next pointers of halfedge
            if (curr != nullptr) {
                curr->prev() = prev;
            }
            if (prev != nullptr) {
                prev->next() = curr;
            }
        }
        first->prev() = curr;
        curr->next() = first;

        prev = curr = nullptr; // We go to a new facet

        // all edges for this face have been generated. Find the normals
        curr_face->normal() = gen_normal_facet(curr_face);
    }

    // TASK 5b establish prev, next for boundary halfedges
    halfedge_iterator he_it;
    for (he_it = this->halfedge_begin(); he_it != this->halfedge_end(); he_it++){
        halfedge_t* h = &(*he_it);
        if(h->on_border()){
            h->next() = h->next_around_vertex(); 
            h->prev() = h->prev_around_vertex();
        }
    }

    // Gen vertex normals
    for (size_t n = 0; n < in_vertex.size(); n++) {
        vertex_list[n]->normal() = gen_normal_vertex(vertex_list[n]);
        // std::cout << vertex_list[n]->id << " Normal " << vertex_list[n]->normal() << std::endl;
    }

    // Normalize facet normals
    facet_iterator f_it;
    for (f_it = this->facet_begin(); f_it != this->facet_end(); f_it++) {
        facet_t* f = &(*f_it);
        f->normal().normalize();
        std::cout << f->normal() <<  std::endl;
    }

    vertex_list.clear();
    edge_list.clear();
    return true;
}

// TASK 5a
halfedge_t* halfedge_t::next_around_vertex(void) {
    // TODO -- return next halfedge around the vertex of the halfedge

    // There can be no more than two boundary halfedges
    // around a vertex. So the next one is unique 
    // See: https://gist.github.com/wamiq-reyaz/604914d8626e2c7356d68f7933937445
    
    halfedge_t* h = this->opposite();
    vertex_t* v = this->vertex();
    do {
        // go around vertex edges till we hit another boundary half-edge
        h = h->next();
        if(h->vertex() == v){
            h = h->opposite();
        }
    } while (!h->on_border()); // we've hit jackpot

    return h; 
}

halfedge_t* halfedge_t::prev_around_vertex(void) {
    halfedge_t* h = this->opposite();
    vertex_t* v = h->vertex();
    do {
        h = h->next();
        if(h->opposite()->vertex() == v){
            h = h->opposite();
        }
    } while (!h->on_border());

    return h; 
}

// TASK 6
size_t vertex_t::degree(void) const {
    // TODO -- compute the degree of this vertex and return it
    // just iterate over all edges, and keep count
    size_t degree = 0;
    halfedge_t* h = (halfedge_t*) this->halfedge();
    do{
        h = h->opposite()->next();
        degree++;
    }while(h != this->halfedge());

    return degree; 
}

bool vertex_t::on_border(void) const {
    // TODO -- return true if vertex is on a border, false otherwise
    halfedge_t* h = (halfedge_t*) this->halfedge();
    do{
        if(h->opposite()->on_border()){
            return true;
        }
        h = h->opposite()->next();
    }while(h != this->halfedge());

    return false; 
}

bool halfedge_t::on_border(void) const {
    // TODO -- return true if this halfedge is on a border
    if (this->facet() == nullptr) {
        return true;
    }
    return false; 
}

size_t facet_t::degree(void) const {
    // TODO -- compute the degree of this facet and return, i.e., return 3 for a
    // triangle, 4 for a quadrilateral
    size_t degree = 0;
    halfedge_t* h = (halfedge_t*) this->halfedge();
    do{
        h = h->next();
        degree++;
    }while(h != this->halfedge());
    return degree; 
}

bool facet_t::on_border(void) const {
    // TODO -- return true if the facet is on a border, false otherwise
    halfedge_t* h = (halfedge_t*) this->halfedge();
    do{
        h = h->next();
        if(h->opposite()->on_border()){
            return true;
        }
    }while(h != this->halfedge());

    return false; 
}

bool facet_t::is_triangle(void) const {
    // TODO -- return true if the facet is a triangle, false otherwise
    if(this->degree() == 3){
        return true;
    }
    return false; 
}

void mesh_t::dbgdump(void) {
#ifdef _DEBUG
    std::cout << "HALFEDGES" << std::endl;
    for (halfedge_const_iterator it = halfedge_cbegin(); it != halfedge_cend(); it++) {
        int n = (it->next() != nullptr ? int(it->next()->id) : (-1));
        int p = (it->prev() != nullptr ? int(it->prev()->id) : (-1));
        int o = (it->opposite() != nullptr ? int(it->opposite()->id) : (-1));
        int f = (it->facet() != nullptr ? int(it->facet()->id) : (-1));
        int v = (it->vertex() != nullptr ? int(it->vertex()->id) : (-1));
        std::cout << "  H" << it->id << ": p=" << p << ", n=" << n << ", o=" << o
                  << ", v=" << v << ", f=" << f << std::endl;
    }
    std::cout << "FACETS" << std::endl;
    for (facet_const_iterator it = facet_cbegin(); it != facet_cend(); it++) {
        int h = (it->halfedge() != nullptr ? int(it->halfedge()->id) : (-1));
        const vecd& N(it->normal());
        std::cout << "  F" << it->id << ": h=" << h << ", N=" << N << std::endl;
    }
    std::cout << "VERTICES" << std::endl;
    for (vertex_const_iterator it = vertex_cbegin(); it != vertex_cend(); it++) {
        int h = (it->halfedge() != nullptr ? int(it->halfedge()->id) : (-1));
        const vecd& N(it->normal());
        const vecd& P(it->position());
        const vecd& C(it->color());
        const vecd& T(it->texcoord());
        std::cout << "  V" << it->id << ": h=" << h << ", P=" << P << ", N=" << N
                  << ", C=" << C << ", T=" << T << std::endl;
    }
#endif
}

/// IMPLEMENTATION OF VERTEX_T STRUCTURE
/// ------------------------------------------------------------------------------

// support for shared_ptr on this
vertex_t* vertex_t::pointer(void) { return this; }

vertex_t::vertex_t(void) {
    m_halfedge = nullptr;
#ifdef _DEBUG
    id = count++;
#endif
}

vertex_t::vertex_t(const vertex_t& other) { *this = other; }

void vertex_t::clear(void) { m_halfedge = nullptr; }

vertex_t::~vertex_t(void) { clear(); }

halfedge_t*& vertex_t::halfedge(void) { return m_halfedge; }

const halfedge_t* vertex_t::halfedge(void) const { return m_halfedge; }

vecd& vertex_t::position(void) { return m_position; }

vecd& vertex_t::normal(void) { return m_normal; }

vecd& vertex_t::color(void) { return m_color; }

vecd& vertex_t::texcoord(void) { return m_texcoord; }

const vecd& vertex_t::position(void) const { return m_position; }

const vecd& vertex_t::normal(void) const { return m_normal; }

const vecd& vertex_t::color(void) const { return m_color; }

const vecd& vertex_t::texcoord(void) const { return m_texcoord; }

const vertex_t& vertex_t::operator=(const vertex_t& other) {
    m_position = other.m_position;
    m_normal = other.m_normal;
    m_color = other.m_color;
    m_texcoord = other.m_texcoord;
    m_halfedge = other.m_halfedge;
#ifdef _DEBUG
    id = other.id;
#endif
    return *this;
}

void vertex_t::reset_count(void) {
#ifdef _DEBUG
    count = 0;
#endif
}

/// IMPLEMENTATION OF HALFEDGE_T STRUCTURE
/// ----------------------------------------------------------------------------

halfedge_t* halfedge_t::pointer(void) { return this; }

halfedge_t::halfedge_t(void) {
    m_opposite = nullptr;
    m_next = nullptr;
    m_prev = nullptr;
    m_facet = nullptr;
    m_vertex = nullptr;
#ifdef _DEBUG
    id = count++;
#endif
}

halfedge_t::halfedge_t(const halfedge_t& other) { *this = other; }

void halfedge_t::clear(void) {
    m_opposite = nullptr;
    m_next = nullptr;
    m_prev = nullptr;
    m_facet = nullptr;
    m_vertex = nullptr;
}

halfedge_t::~halfedge_t(void) { clear(); }

halfedge_t*& halfedge_t::opposite(void) { return m_opposite; }

halfedge_t*& halfedge_t::next(void) { return m_next; }

halfedge_t*& halfedge_t::prev(void) { return m_prev; }

facet_t*& halfedge_t::facet(void) { return m_facet; }

vertex_t*& halfedge_t::vertex(void) { return m_vertex; }

const halfedge_t* halfedge_t::opposite(void) const { return m_opposite; }

const halfedge_t* halfedge_t::next(void) const { return m_next; }

const halfedge_t* halfedge_t::prev(void) const { return m_prev; }

const facet_t* halfedge_t::facet(void) const { return m_facet; }

const vertex_t* halfedge_t::vertex(void) const { return m_vertex; }

const halfedge_t& halfedge_t::operator=(const halfedge_t& other) {
    m_opposite = other.m_opposite;
    m_next = other.m_next;
    m_prev = other.m_prev;
    m_facet = other.m_facet;
    m_vertex = other.m_vertex;
#ifdef _DEBUG
    id = other.id;
#endif
    return *this;
}

void halfedge_t::reset_count(void) {
#ifdef _DEBUG
    count = 0;
#endif
}

/// IMPLEMENTATION OF FACET_T STRUCTURE
/// -------------------------------------------------------------------------------

facet_t* facet_t::pointer(void) { return this; }

facet_t::facet_t(void) {
    m_halfedge = nullptr;
#ifdef _DEBUG
    id = count++;
#endif
}

facet_t::facet_t(const facet_t& other) { *this = other; }

void facet_t::clear(void) { m_halfedge = nullptr; }

facet_t::~facet_t(void) { clear(); }

vecd& facet_t::normal(void) { return m_normal; }

const vecd& facet_t::normal(void) const { return m_normal; }

halfedge_t*& facet_t::halfedge(void) { return m_halfedge; }

const halfedge_t* facet_t::halfedge(void) const { return m_halfedge; }

const facet_t& facet_t::operator=(const facet_t& other) {
    m_halfedge = other.m_halfedge;
    m_normal = other.m_normal;
#ifdef _DEBUG
    id = other.id;
#endif
    return *this;
}

void facet_t::reset_count(void) {
#ifdef _DEBUG
    count = 0;
#endif
}

/// IMPLEMENTATION OF MESH
/// --------------------------------------------------------------------------------------------

mesh_t::mesh_t(void) {}

mesh_t::mesh_t(const mesh_t& other) { *this = other; }

mesh_t::~mesh_t(void) { clear(); }

void mesh_t::clear(void) {
    m_vertices.clear();
    m_halfedges.clear();
    m_facets.clear();
    vertex_t::reset_count();
    halfedge_t::reset_count();
    facet_t::reset_count();
    m_texcoord = false;
}

vertex_t* mesh_t::new_vertex(void) {
    m_vertices.push_back(vertex_t());
    return m_vertices.back().pointer();
}

halfedge_t* mesh_t::new_halfedge(void) {
    m_halfedges.push_back(halfedge_t());
    return m_halfedges.back().pointer();
}

facet_t* mesh_t::new_facet(void) {
    m_facets.push_back(facet_t());
    return m_facets.back().pointer();
}

mesh_t::vertex_iterator mesh_t::vertex_begin(void) { return m_vertices.begin(); }

mesh_t::vertex_iterator mesh_t::vertex_end(void) { return m_vertices.end(); }

mesh_t::halfedge_iterator mesh_t::halfedge_begin(void) { return m_halfedges.begin(); }

mesh_t::halfedge_iterator mesh_t::halfedge_end(void) { return m_halfedges.end(); }

mesh_t::facet_iterator mesh_t::facet_begin(void) { return m_facets.begin(); }

mesh_t::facet_iterator mesh_t::facet_end(void) { return m_facets.end(); }

mesh_t::vertex_const_iterator mesh_t::vertex_cbegin(void) const {
    return m_vertices.cbegin();
}
mesh_t::vertex_const_iterator mesh_t::vertex_cend(void) const {
    return m_vertices.cend();
}
mesh_t::halfedge_const_iterator mesh_t::halfedge_cbegin(void) const {
    return m_halfedges.cbegin();
}

mesh_t::halfedge_const_iterator mesh_t::halfedge_cend(void) const {
    return m_halfedges.cend();
}

mesh_t::facet_const_iterator mesh_t::facet_cbegin(void) const {
    return m_facets.cbegin();
}

mesh_t::facet_const_iterator mesh_t::facet_cend(void) const { return m_facets.cend(); }

size_t mesh_t::nVertices(void) const {
    return m_vertices.size(); // const time operation in C++11
}

size_t mesh_t::nHalfedges(void) const { return m_halfedges.size(); }

size_t mesh_t::nFacets(void) const { return m_facets.size(); }

const mesh_t& mesh_t::operator=(const mesh_t& other) {
    m_halfedges = other.m_halfedges;
    m_facets = other.m_facets;
    m_vertices = other.m_vertices;
    m_texcoord = other.m_texcoord;
    return *this;
}

bool mesh_t::has_texcoord(void) const { return m_texcoord; }

bool mesh_t::load(const std::string& name) {
    std::ifstream input(name.c_str());
    if (input.fail()) {
        std::cerr << "Error opening file " << name << std::endl;
        return false;
    }

    std::vector< vecd > in_vertex;
    std::vector< vecd > in_texcoord;
    std::vector< std::vector< size_t > > in_facet;

    while (!input.eof()) {
        std::string line;
        std::getline(input, line);
        std::istringstream input_line(line);
        std::string token;
        input_line >> token;
        if (!token.empty()) {
            if (token == "v") {
                double x, y, z;
                input_line >> x >> y >> z;
                in_vertex.push_back(vecd(x, y, z));
                continue;
            }
            if (token == "f") {
                std::vector< size_t > fct;
                while (!input_line.eof()) {
                    std::string s;
                    input_line >> s;
                    if (!s.empty()) {
                        std::istringstream vtx_in(s);
                        size_t index;
                        vtx_in >> index;
                        fct.push_back(index - 1);
                    }
                }
                if (fct.size() > 2)
                    in_facet.push_back(fct);
                else {
                    std::cerr << "Warning: face has only " << fct.size()
                              << "vertices (ignored)." << std::endl;
                }
                continue;
            }
            if (token == "vt") {
                double s, t;
                input_line >> s >> t;
                in_texcoord.push_back(vecd(s, t));
                continue;
            }
        }
    }
    input.close();
    return build_mesh(in_vertex, in_texcoord, in_facet);
}

bool mesh_t::check_mesh(void) {
    bool pass = true;

    // Every vertex must have an outgoing halfedge
    std::cout << "vertex->halfedge          : ";
    for (vertex_const_iterator it = vertex_cbegin(); it != vertex_cend(); it++) {
        if (it->halfedge() == nullptr) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test2;
        }
    }
    std::cout << "PASS" << std::endl;

test2:
    // Every facet must have an associated halfedge
    std::cout << "facet->halfedge           : ";
    for (facet_const_iterator it = facet_cbegin(); it != facet_cend(); it++) {
        if (it->halfedge() == nullptr) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test3;
        }
    }
    std::cout << "PASS" << std::endl;

test3:
    // Every halfedge must have an opposite halfedge
    std::cout << "halfedge->opposite        : ";
    for (halfedge_const_iterator it = halfedge_cbegin(); it != halfedge_cend(); it++) {
        if (it->opposite() == nullptr) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test4;
        }
    }
    std::cout << "PASS" << std::endl;

test4:
    // Every halfedge must have a next halfedge
    std::cout << "halfedge->next            : ";
    for (halfedge_const_iterator it = halfedge_cbegin(); it != halfedge_cend(); it++) {
        if (it->next() == nullptr) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test5;
        }
    }
    std::cout << "PASS" << std::endl;

test5:
    // Every halfedge must have a previous
    std::cout << "halfedge->prev            : ";
    for (halfedge_const_iterator it = halfedge_cbegin(); it != halfedge_cend(); it++) {
        if (it->prev() == nullptr) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test6;
        }
    }
    std::cout << "PASS" << std::endl;

test6:
    // Every halfedge must have a vertex
    std::cout << "halfedge->vertex          : ";
    for (halfedge_const_iterator it = halfedge_cbegin(); it != halfedge_cend(); it++) {
        if (it->vertex() == nullptr) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test7;
        }
    }
    std::cout << "PASS" << std::endl;

test7:
    // opposite->opposite = same halfedge
    std::cout << "opposite->opposite        : ";
    for (halfedge_iterator it = halfedge_begin(); it != halfedge_end(); it++) {
        if (it->opposite()->opposite() != it->pointer()) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test8;
        }
    }
    std::cout << "PASS" << std::endl;

test8:
    // next->prev = same halfedge
    std::cout << "next->prev                : ";
    for (halfedge_iterator it = halfedge_begin(); it != halfedge_end(); it++) {
        if (it->next()->prev() != it->pointer()) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test9;
        }
    }
    std::cout << "PASS" << std::endl;

test9:
    // prev->next = same halfedge
    std::cout << "prev->next                : ";
    for (halfedge_iterator it = halfedge_begin(); it != halfedge_end(); it++) {
        if (it->prev()->next() != it->pointer()) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test10;
        }
    }
    std::cout << "PASS" << std::endl;

test10:
    // halfedge->next->opposite->vertex = vertex OR halfedge->facet == nullptr
    std::cout << "opposite vertex           : ";
    for (halfedge_iterator it = halfedge_begin(); it != halfedge_end(); it++) {
        if (it->next()->opposite()->vertex() != it->vertex() && it->facet() != nullptr) {
            std::cout << "FAIL" << std::endl;
            pass = false;
            goto test11;
        }
    }
    std::cout << "PASS" << std::endl;

test11:
    // halfedge->facet->halfedge->[next]+ = halfedge (halfedge can be reached by
    // going to the face, then circulating around face
    std::cout << "circulate face            : ";
    for (halfedge_iterator it = halfedge_begin(); it != halfedge_end(); it++) {
        if (it->facet() != nullptr) {
            halfedge_t* curr = it->facet()->halfedge();
            halfedge_t* first = curr;
            do {
                curr = curr->next();
            } while (first != curr && it->pointer() != curr);
            if (curr != it->pointer()) {
                std::cout << "FAIL" << std::endl;
                pass = false;
                goto test12;
            }
        }
    }
    std::cout << "PASS" << std::endl;

test12:
    return pass;
}