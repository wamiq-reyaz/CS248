#include "catch.hpp"

#include "mesh.h"

#include<unordered_map>
#include <cmath>

typedef std::vector< std::string > string_vector;

const double EPS = 1e-5;

TEST_CASE("Build Mesh", "[build]") {
    // Make sure all the obj files load and return pass

    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
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
    model_names.push_back("Cylinder.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");
    size_t n_vertices_array[5] = {8, 5, 1090, 1986, 2048};

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
    model_names.push_back("Cylinder.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");
    size_t n_facets_array[5] = {12, 4, 2176, 3968, 4096};

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

TEST_CASE("Number of Halfedges", "[nHalf]") { 
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Cylinder.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");
    size_t n_he_array[5] = {36, 16, 6528, 11904, 12288};

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

TEST_CASE("Vertex Degree", "[vDeg]"){
    // TODO
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");
    model_names.push_back("Cylinder.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    // setup a map from the degree to the number of vertices having that
    // degree. We'll decrement it each time we hit a vertex matching the degree
    // The final result must be zero
    std::unordered_map<size_t, size_t> box_degree_to_num_vertices;
    box_degree_to_num_vertices[4] = 4;
    box_degree_to_num_vertices[5] = 4;

    std::unordered_map<size_t, size_t> boundary_degree_to_num_vertices;
    boundary_degree_to_num_vertices[3] = 4;
    boundary_degree_to_num_vertices[4] = 1;

    std::unordered_map<size_t, size_t> cylinder_degree_to_num_vertices;
    cylinder_degree_to_num_vertices[5] = 128;
    cylinder_degree_to_num_vertices[6] = 960;
    cylinder_degree_to_num_vertices[64] = 2;

    std::unordered_map<size_t, size_t> sphere_degree_to_num_vertices;
    sphere_degree_to_num_vertices[5] = 128;
    sphere_degree_to_num_vertices[6] = 1856;
    sphere_degree_to_num_vertices[64] = 2;

    std::unordered_map<size_t, size_t> torus_degree_to_num_vertices;
    torus_degree_to_num_vertices[6] = 2048;

    mesh_t::vertex_iterator v_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for (v_it = test_mesh.vertex_begin(); v_it != test_mesh.vertex_end(); v_it++) {
            vertex_t v = *v_it;
            if(*it == "Box.obj"){
                // Here, if we hit a vertex with the required degree
                // we decrement the count of the map
                box_degree_to_num_vertices[v.degree()]--;
            }
            else if(*it == "Boundary.obj"){
                boundary_degree_to_num_vertices[v.degree()]--;
            }
            else if(*it == "Cylinder.obj"){
                cylinder_degree_to_num_vertices[v.degree()]--;
            }
            else if(*it == "Sphere.obj"){
                sphere_degree_to_num_vertices[v.degree()]--;
            }
            else if(*it == "Torus.obj"){
                torus_degree_to_num_vertices[v.degree()]--;
            }
        }
    }

    // All counts should hit zero
    REQUIRE(box_degree_to_num_vertices[3] == 0);
    REQUIRE(box_degree_to_num_vertices[4] == 0);

    REQUIRE(boundary_degree_to_num_vertices[4] == 0);
    REQUIRE(boundary_degree_to_num_vertices[5] == 0);

    REQUIRE(cylinder_degree_to_num_vertices[5] == 0);
    REQUIRE(cylinder_degree_to_num_vertices[6] == 0);
    REQUIRE(cylinder_degree_to_num_vertices[64] == 0);

    REQUIRE(sphere_degree_to_num_vertices[5] == 0);
    REQUIRE(sphere_degree_to_num_vertices[6] == 0);
    REQUIRE(sphere_degree_to_num_vertices[64] == 0);

    REQUIRE(torus_degree_to_num_vertices[6] == 0);

    WARN("Passed: All vertices' degree detected"); 
}

TEST_CASE("Vertex On Border", "[vBord]"){  
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
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
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
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
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
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
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
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
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
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

TEST_CASE("Vertex Normals", "[vNorm]"){  
    // TODO
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");

    // We'll set up a map from the vertex ids to the normal vector
    // We then check if the id matches the vector within floating point errors

    std::unordered_map<size_t, vecd> boundary_normal;
    // for boundary, everything points in +Z direction
    boundary_normal[0] = vecd(0, 0, 1);
    boundary_normal[1] = vecd(0, 0, 1);
    boundary_normal[2] = vecd(0, 0, 1);
    boundary_normal[3] = vecd(0, 0, 1);
    boundary_normal[4] = vecd(0, 0, 1);

    std::unordered_map<size_t, vecd> box_normal;
    // this one is a lot more tricky
    box_normal[0] = vecd(-0.333333, -0.666667, 0.666667);
    box_normal[1] = vecd(-0.816497, -0.408248, -0.408248);
    box_normal[2] = vecd(0.333333, -0.666667, -0.666667);
    box_normal[3] = vecd(0.816497, -0.408248, 0.408248);
    box_normal[4] = vecd(-0.666667, 0.666667, 0.333333);
    box_normal[5] = vecd(0.408248, 0.408248, 0.816497);
    box_normal[6] = vecd(0.666667, 0.666667, -0.333333);
    box_normal[7] = vecd(-0.408248, 0.408248, -0.816497);

    mesh_t::vertex_iterator v_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for (v_it = test_mesh.vertex_begin(); v_it != test_mesh.vertex_end(); v_it++) {
            vertex_t v = *v_it;
            if(*it == "Boundary.obj"){
                REQUIRE(v.normal() - boundary_normal[v.id] == vecd(0));
            }
            if(*it == "Box.obj"){
                REQUIRE(v.normal() - box_normal[v.id] == vecd(0));
            }
        }
    }
    WARN("Passed: All boundary vertices detected");
}

TEST_CASE("Facet Normals", "[fNorm]"){
    // TODO
    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");

    // We'll set up a map from the vertex ids to the normal vector
    // We then check if the id matches the vector within floating point errors

    std::unordered_map<size_t, vecd> boundary_normal;
    // for boundary, everything points in +Z direction
    boundary_normal[0] = vecd(0, 0, 1);
    boundary_normal[1] = vecd(0, 0, 1);
    boundary_normal[2] = vecd(0, 0, 1);
    boundary_normal[3] = vecd(0, 0, 1);

    std::unordered_map<size_t, vecd> box_normal;
    // the faces point in +- x, y, z directions
    box_normal[0] = vecd(0, -1, 0);
    box_normal[1] = vecd(0, -1, 0);
    box_normal[2] = vecd(0, 1, 0);
    box_normal[3] = vecd(0, 1, 0);
    box_normal[4] = vecd(0, 0, 1);
    box_normal[5] = vecd(0, 0, 1);
    box_normal[6] = vecd(1, 0, 0);
    box_normal[7] = vecd(1, 0, 0);
    box_normal[8] = vecd(0, 0, -1);
    box_normal[9] = vecd(0, 0, -1);
    box_normal[10] = vecd(-1, 0, 0);
    box_normal[11] = vecd(-1, 0, 0);

    mesh_t::facet_iterator facet_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {
        mesh_t test_mesh;
        test_mesh.load(*it);

        for(facet_it = test_mesh.facet_begin(); facet_it != test_mesh.facet_end(); facet_it++){
            facet_t f = *facet_it;
            if(*it == "Boundary.obj"){
                REQUIRE(f.normal() - boundary_normal[f.id] == vecd(0));
            }
            if(*it == "Box.obj"){
                REQUIRE(f.normal() - box_normal[f.id] == vecd(0));
            }
        }
    }  
    WARN("Passed: All triangle facets correctly determined");
}

TEST_CASE("Vertex normals normalized", "[vNormalNormalized]") {
    // See if the vertex normal norm is close to 1
    mesh_t test_mesh;

    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
    model_names.push_back("Sphere.obj");
    model_names.push_back("Torus.obj");

    mesh_t::vertex_iterator v_it;
    for (string_vector::iterator it = model_names.begin(); it != model_names.end();
         it++) {

        test_mesh.load(*it);
        for (v_it = test_mesh.vertex_begin(); v_it != test_mesh.vertex_end(); v_it++) {
            vertex_t vertex = *v_it;
            REQUIRE(fabs(vertex.normal().length2() - 1) < EPS);
        }
    }
    WARN("Passed: Vertex normal normalized");
}

TEST_CASE("Facet normals normalized", "[fNormalNormalized]") {
    // See if the vertex normal norm is close to 1
    mesh_t test_mesh;

    string_vector model_names;
    model_names.push_back("Box.obj");
    model_names.push_back("Boundary.obj");model_names.push_back("Cylinder.obj");
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
