#include"vec.h"
#include"mat.h"

#include"mesh.h"

#include<stdio.h>
#include<stdlib.h>


int main(int argc, char** argv) {
	// TODO -- test every vector and matrix function. Submit your code with tests in here.
	mesh_t aa;
	aa.load(argv[1]);
	aa.dbgdump();
	

	/* This tests for the 
	*/
	#ifdef _DEBUG
	mesh_t::halfedge_iterator he_it;
	for(he_it = aa.halfedge_begin(); he_it != aa.halfedge_end(); he_it++){
		halfedge_t edge = *he_it;
		if(edge.on_border()){
			// std::cout << edge.id << " on boundary " << std::endl;
		}
	}
	#endif

	#ifdef _DEBUG
	mesh_t::vertex_iterator v_it;
	for(v_it = aa.vertex_begin(); v_it != aa.vertex_end(); v_it++){
		// vertex_t v = *v_it;
		// std::cout << " Degree: " << v.degree() << std::endl;

		// if(v.on_border()){
		// 	std::cout << "on border: " << v.id << std::endl;
		// }
	}

	mesh_t::facet_iterator facet_it;
	for(facet_it = aa.facet_begin(); facet_it != aa.facet_end(); facet_it++){
		facet_t f = *facet_it;
			// std::cout << f.id << " Degree: " << f.degree() << std::endl;
			//std::cout << f.id << " On Border: " << f.on_border() << std::endl;
	}


	#endif

	// std::cout << aa.check_mesh() << std::endl;
	aa.clear();
	return 0;
}
