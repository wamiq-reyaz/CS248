#include"vec.h"
#include"mat.h"
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int main(int argc, char** argv) {
	// TODO -- test every vector and matrix function. Submit your code with tests in here.

	//crappy test for everything
	vecf zero_vector; 
	vecf unit_vector(1);
	cout << zero_vector; 	
	vecf random_vector(88,2,3,4);
	vecf a(random_vector);
	cout << random_vector(0) << random_vector(1)<< random_vector(2)<< random_vector(3)<< endl;
	cout << a(0) << endl;


	cout << "Length2 of zero is " << zero_vector.length2() << endl;
	cout << "Length of unit is " << unit_vector.length(1) << endl;
	cout << "Length2 of rand is " << random_vector.length2(2) << endl;
	cout << "Length of rand is " << random_vector.length(2) << endl;

	// dot product of unit and zero
	float d;
	d = random_vector.dot(unit_vector, 4);
	cout << "the dot product is " << d << endl;

	return 0;	
}
