#include"vec.h"
//#include"mat.h"
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
	cout << a(0) << endl;


	cout << "Length2 of zero is " << zero_vector.length2() << endl;
	cout << "Length of unit is " << unit_vector.length(1) << endl;
	cout << "Length2 of rand is " << random_vector.length2(2) << endl;
	cout << "Length of rand is " << random_vector.length(2) << endl;

	// dot product of unit and zero
	float d;
	d = random_vector.dot(unit_vector, 4);
	cout << "the dot product is " << d << endl;

	// the operator +=
	random_vector += unit_vector;
	cout << random_vector << endl;

	// the operator -=
	random_vector -= unit_vector;
	random_vector -= unit_vector;
	random_vector -= unit_vector;
	cout << random_vector << endl;

	vecf new_vector(1,2,4,6);
	double multiplier = 2.50;

	cout << 6 * new_vector << endl;
	new_vector *= 0.5;
	cout << new_vector << endl;
	new_vector += 6;
	cout << new_vector << endl;

	vecd vector_div(2, 4, 8, 16);
	vector_div /= 2;
	cout << vector_div << endl;
	return 0;	
}