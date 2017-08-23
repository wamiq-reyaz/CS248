#include"vec.h"
#include"mat.h"
#include<stdio.h>
#include<stdlib.h>
using namespace std;

int main(int argc, char** argv) {
	// TODO -- test every vector and matrix function. Submit your code with tests in here.

	//crappy test for everything
	// vecd zero_vector; 
	// vecf unit_vector(1);
	// cout << zero_vector; 	
	// vecf random_vector(88,2,3,4);
	// vecf a(random_vector);
	// cout << a(0) << endl;


	// cout << "Length2 of zero is " << zero_vector.length2() << endl;
	// cout << "Length of unit is " << unit_vector.length(1) << endl;
	// cout << "Length2 of rand is " << random_vector.length2(2) << endl;
	// cout << "Length of rand is " << random_vector.length(2) << endl;

	// // dot product of unit and zero
	// float d;
	// d = random_vector.dot(unit_vector, 4);
	// cout << "the dot product is " << d << endl;

	// // the operator +=
	// random_vector += unit_vector;
	// cout << random_vector << endl;

	// // the operator -=
	// random_vector -= unit_vector;
	// random_vector -= unit_vector;
	// random_vector -= unit_vector;
	// cout << "we print here" << endl;
	// cout << random_vector << endl;

	// // operator *
	// vecf new_vector(1,2,4,6);
	// double multiplier = 2.50;
	// //cout << 6 * new_vector << endl;

	// //operator *=
	// new_vector *= 0.5;
	// cout << new_vector << endl;

	// //operator +=
	// new_vector += 6;
	// cout << new_vector << endl;

	// // operator /=
	// vecd vector_div(2, 4, 8, 16);
	// vector_div /= 2;
	// cout << vector_div << endl;
	// cout << vector_div / 4 << endl;

	// // operator /
	// vecd thingy(1,2, 3, 4);
	// cout << thingy / 2 << endl;

	// // operator = 
	// vecd b(0, 0, 0, 0);
	// thingy = b;
	// cout << thingy << endl;

	// //operator ==
	// cout << (zero_vector == b) << endl;

	// //operator !=
	// cout << "this tests whether zero vector != b (another zero vector)" << endl;
	// cout << (zero_vector != b) << endl; 

	// // operator -
	// cout << "negation of (1, 0, 5, 7)" << endl;
	// vecf negation(1, 0, 5, 7);
	// cout << -negation << endl;

	// // normalize
	// vecf ones_f(1, 1, 1, 1);
	// vecd ones_d(1, 2, 3, 4);

	// cout << "normalizing the first 3 components of (1,1,1,1)" << endl;
	// ones_f.normalize(3);
	// cout << ones_f << endl;

	// // cross product
	// vecf x(1, 0, 0);
	// vecf y(0, 1, 0);
	// cout << "crossing x * y. answer must be (0,0,1)" << endl;
	// cout << x.cross(y) << endl;
	// cout << "y*x" << endl;
	// cout << y.cross(x) << endl;

	// // operator +
	// cout << "adding x +y" << endl;
	// cout << x + y << endl;


	// //operator -
	// cout << "x - y" << endl;
	// cout << x - y << endl;

	// // operator >>
	// vecf aaaa;
	// //cin >> aaaa;
	// //cout << aaaa << "was this what you entered?" << endl;

	// // void constructor
	// matf zeros;
	// cout << zeros;

	matf random(1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6);
	cout << random;

	matf new_mat(random);
	cout << "random should be the same matrix as above" << endl;
	cout << new_mat;
	cout << "sum with itself" << endl;
	cout << new_mat + new_mat << endl;




	return 0;		
}
