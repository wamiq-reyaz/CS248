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
	mesh_t::halfedge_iterator he_it;

	/* This tests for the 
	*/
	for(he_it = aa.halfedge_begin(); he_it != aa.halfedge_end(); he_it++){
		halfedge_t edge = *he_it;
		if(edge.on_border()){
			std::cout << edge.id << " on boundary " << std::endl;
		}
	}

	std::cout << aa.check_mesh() << std::endl;
	return 0;
}
