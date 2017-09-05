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
	std::cout << aa.check_mesh() << std::endl;
	return 0;
}
