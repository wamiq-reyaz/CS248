#include "catch.hpp"

#include "mesh.h"

#include <cmath>

typedef std::vector< std::string > string_vector;

TEST_CASE("Build Mesh", "[build]") {
    // Make sure all the obj files load and return pass

    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    // suppress output from mesh_t.check_mesh()
    std::cout.setstate(std::ios_base::failbit);

    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);
        REQUIRE(test_mesh.check_mesh() == true);
    }
    // if we are here, we passed
    WARN("Passed: mesh_t.load() build mesh and passes mesh_t.check_mesh()");
}

TEST_CASE("Number of vertices", "[nVertices]") {
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");
    size_t n_vertices_array[4] = {8, 5, 1986, 2048};

    size_t counter = 0;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);
        REQUIRE(test_mesh.nVertices() == n_vertices_array[counter]);
        counter++;
    }
    WARN("Passed: mesh_t.load() loads all Vertices");
}

TEST_CASE("Number of facets", "[nFacets]") {
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");
    size_t n_facets_array[4] = {12, 4, 3968, 4096};

    size_t counter = 0;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);
        REQUIRE(test_mesh.nFacets() == n_facets_array[counter]);
        counter++;
    }
    WARN("Passed: mesh_t.load() loads all Facets");
}

TEST_CASE("Number of Halfedges", "[nFacets]") {
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");
    size_t n_he_array[4] = {36, 16, 11904, 12288};

    size_t counter = 0;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);
        REQUIRE(test_mesh.nHalfedges() == n_he_array[counter]);
        counter++;
    }
    WARN("Passed: mesh_t.load() loads all halfedges");
}

TEST_CASE("next_around_vertex", "[nav]") {
    // See if the next for boundary points to the correct half-edge
    mesh_t test_mesh;
    test_mesh.load("Boundary.obj");

    mesh_t::halfedge_iterator he_it;
    for (he_it = test_mesh.halfedge_begin(); he_it != test_mesh.halfedge_end(); he_it++) {
        halfedge_t edge = *he_it;
        if (edge.id == 3)
            REQUIRE(edge.next()->id == 15);
        if (edge.id == 15)
            REQUIRE(edge.next()->id == 11);
        if (edge.id == 11)
            REQUIRE(edge.next()->id == 7);
        if (edge.id == 7)
            REQUIRE(edge.next()->id == 3);
    }
    WARN("Passed: next_around_vertex points to correct half-edge");
}

TEST_CASE("prev_around_vertex", "[pav]") {
    // See if the next for boundary points to the correct half-edge
    mesh_t test_mesh;
    test_mesh.load("Boundary.obj");

    mesh_t::halfedge_iterator he_it;
    for (he_it = test_mesh.halfedge_begin(); he_it != test_mesh.halfedge_end(); he_it++) {
        halfedge_t edge = *he_it;
        if (edge.id == 3)
            REQUIRE(edge.prev()->id == 7);
        if (edge.id == 15)
            REQUIRE(edge.prev()->id == 3);
        if (edge.id == 11)
            REQUIRE(edge.prev()->id == 15);
        if (edge.id == 7)
            REQUIRE(edge.prev()->id == 11);
    }
    WARN("Passed: prev_around_vertex points to correct half-edge");
}

TEST_CASE("Boundary Halfedge Facet", "[heBoundaryFacet]") {
    // See if the next for boundary points to the correct half-edge
    mesh_t test_mesh;
    test_mesh.load("Boundary.obj");

    mesh_t::halfedge_iterator he_it;
    for (he_it = test_mesh.halfedge_begin(); he_it != test_mesh.halfedge_end(); he_it++) {
        halfedge_t edge = *he_it;
        if (edge.id == 3)
            REQUIRE(edge.facet() == nullptr);
        if (edge.id == 15)
            REQUIRE(edge.facet() == nullptr);
        if (edge.id == 11)
            REQUIRE(edge.facet() == nullptr);
        if (edge.id == 7)
            REQUIRE(edge.facet() == nullptr);
    }
    WARN("Passed: All Boundary half-edge facets point to nullptr");
}

TEST_CASE("Vertex Degree", "vDeg"){
    // TODO  
}

TEST_CASE("Vertex On Border", "[vBord]"){  
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::vertex_iterator v_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for (v_it = test_mesh.vertex_begin(); v_it != test_mesh.vertex_end(); v_it++) {
            vertex_t v = *v_it;
            if(*it == "Boundary.obj" && (v.id == 1 || v.id == 2
                                        || v.id == 3 || v.id == 4)){

                REQUIRE(v.on_border() == true);
                WARN("Vertex " << v.id << " of Boundary.obj on border");
            }
            else{
                REQUIRE(v.on_border() == false);
            }
        }
    }
    WARN("Passed: All boundary vertices detected");
}

TEST_CASE("Halfedge On Border", "[heBord]"){
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::halfedge_iterator he_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for (he_it = test_mesh.halfedge_begin(); he_it != test_mesh.halfedge_end(); he_it++) {
            halfedge_t edge = *he_it;
            if(*it == "Boundary.obj" && (edge.id == 3 || edge.id == 7
                                        || edge.id == 11 || edge.id == 15)){

                REQUIRE(edge.on_border() == true);
                WARN("Half-edge " << edge.id << " of Boundary.obj on border");
            }
            else{
                REQUIRE(edge.on_border() == false);
            }
        }
    }
    WARN("Passed: All boundary half-edges detected");
}

TEST_CASE("Facet Degree", "[fDeg]"){
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::facet_iterator facet_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for(facet_it = test_mesh.facet_begin(); facet_it != test_mesh.facet_end(); facet_it++){
            facet_t f = *facet_it;

            REQUIRE(f.degree() == 3);
        }
    }
    WARN("Passed: Facet Degree correctly determined");
}

TEST_CASE("Facet On Border", "[fBord]"){
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::facet_iterator facet_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for(facet_it = test_mesh.facet_begin(); facet_it != test_mesh.facet_end(); facet_it++){
            facet_t f = *facet_it;
            if(*it == "Boundary.obj" && (f.id == 0 || f.id == 1
                                        || f.id == 2 || f.id == 3)){

                REQUIRE(f.on_border() == true);
                WARN("Facet " << f.id << " of Boundary.obj on border");
            }
            else{
                REQUIRE(f.on_border() == false);
            }            
        }
    }
    WARN("Passed: Border facets correctly determined");  
}

TEST_CASE("Facet is Triangle", "[fTri]"){
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::facet_iterator facet_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for(facet_it = test_mesh.facet_begin(); facet_it != test_mesh.facet_end(); facet_it++){
            facet_t f = *facet_it;
            REQUIRE(f.is_triangle() == true);
        }
    }  
    WARN("Passed: All triangle facets correctly determined");
}

TEST_CASE("Vertex normals normalized", "[vNormalNormalized]") {
    // See if the vertex normal norm is close to 1
    mesh_t test_mesh;

    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::vertex_iterator v_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {

        test_mesh.load(*it);
        for (v_it = test_mesh.vertex_begin(); v_it != test_mesh.vertex_end(); v_it++) {
            vertex_t vertex = *v_it;
            REQUIRE(fabs(vertex.normal().length2() - 1) < 1e-5);
        }
    }
    WARN("Passed: Vertex normal normalized");
}

TEST_CASE("Facet normals normalized", "[fNormalNormalized]") {
    // See if the vertex normal norm is close to 1
    mesh_t test_mesh;

    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::facet_iterator facet_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {

        test_mesh.load(*it);
        for(facet_it = test_mesh.facet_begin(); facet_it != test_mesh.facet_end(); facet_it++){
            facet_t f = *facet_it;
            REQUIRE(fabs(f.normal().length2() - 1) < 1e-5);
        }
    }
    WARN("Passed: Facet normal normalized");
}
