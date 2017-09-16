#include"quat.h"
#include"mat.h"

#include<iostream>

// #include <glm/vec3.hpp> // glm::vec3
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtx/string_cast.hpp> 

using namespace std;
int main(int argc, char** argv){
    // quat<float> q1(1,0,1,0), q2(1,0.5,0.5,0.75);
    // vecf v1;
    // std::cout << "Self multipliation and printing "<< q1*q2 << " " << v1 << std::endl;
    // std::cout << "conjugate of 1,0,1,0 " << q1.conjugate() << std::endl;
    // std::cout << "conjugate of 1, .5, .5, .75 " << q2.conjugate() << std::endl;
    // q1 *= q2;
    // std::cout << "operator *= with above quarternions "<< q1 << " " << std::endl;

    // std::cout << "inverse of q1 " << q1.inverse() << std::endl;
    // std::cout << "q1 * q1.inverse " << q1*q1.inverse() << std::endl; 

    // std::cout << "inverse of q2 " << q2.inverse() << std::endl;
    // std::cout << "q2 * q2.inverse " << q2*q2.inverse() << std::endl; 

    // ++++++++++++++++++++++++++++++++MAT.H+++++++++++++++++++++++++++//
    // cout << "Zero:\n" << matf::Zero() << endl;
    // cout << "Identity\n" << matd::Identity() << endl;
    // cout << "Translation(1,1,1,0):\n" << matf::Translation(vecf(1, 1, 1, 0)) << endl;
    // cout << "Translation(3,1,2,0):\n" << matd::Translation(vecd(3, 1, 2, 0)) << endl;

    matf aa; aa.identity();
    float a =56;
    // cout << "Identity matrix" << endl;
    // cout << "Subdeterminant 1,3:\n" << aa.subdeterminant(1,3) << endl;
    // cout << "Determinant \n" << aa.determinant() << endl;
    cout << "Rotation about x \n" << matf::Rotation(30, vecf(1/sqrt(2), 1/sqrt(2), 0)) << endl;
    glm::vec3 myRotationAxis( 1/sqrt(2), 1/sqrt(2), 0.0f);
    glm::mat4x4 ax = glm::rotate( float(M_PI/6), myRotationAxis);
    cout << "from glm: \n" << glm::to_string(ax) << endl;

    // cout << "adjoint:\n" << aa.adjoint() << endl;
    // cout << "Inverse:\n" << aa.inverse(&a) << endl;
    // cout << "Inverse_transpose:\n" << aa.inverse_transpose(&a) << endl;
    return 0;
}