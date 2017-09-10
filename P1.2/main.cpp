#include"vec.h"
#include"mat.h"

#include"mesh.h"

#include"catch.hpp"

#include<stdio.h>
#include<stdlib.h>


std::string ParseArgs(int argc, char**argv){
	if(argc != 2){
		std::cout << "Usage: ./main <Model>";
		exit(EXIT_FAILURE);
	}
	else return std::string(argv[1]);
}

int main(int argc, char** argv) {
	// TODO dump the mesh to a file.
	mesh_t test_mesh;
	test_mesh.load(ParseArgs(argc, argv));
	test_mesh.dbgdump();

	// check mesh
	std::cout << test_mesh.check_mesh() << std::endl;

	test_mesh.clear();
	return 0;
}
