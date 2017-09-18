#include"quat.h"
#include"mat.h"

#include<iostream>

// #include <glm/vec3.hpp> // glm::vec3
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
// NOTE: glm stores matrices column wise
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
    // glm::vec3 translate(1,1,1);
    // glm::mat4x4 ax = glm::translate(translate);
    // cout << "from glm: \n" << glm::to_string(ax) << endl;

    // cout << "Translation(3,1,2,0):\n" << matd::Translation(vecd(3, 1, 2, 0)) << endl;

    matf aa; aa.identity();;
    // cout << "Identity matrix" << endl;
    // cout << "Subdeterminant 1,3:\n" << aa.subdeterminant(1,3) << endl;
    // cout << "Determinant \n" << aa.determinant() << endl;
    // cout << "Rotation about x \n" << matf::Rotation(30, vecf(1/sqrt(2), 1/sqrt(2), 0)) << endl;
    // glm::vec3 scale( 1.0f, 2.0f,3.0f);
    // glm::mat4x4 ax = glm::scale(scale);
    // cout << "scale  by 1,2,3 \n" << matf::Scale(vecf(1, 2, 3)) << endl;

    // cout << "from glm: \n" << glm::to_string(ax) << endl;

    float a;
    cout << "adjoint:\n" << aa.adjoint() << endl;
    cout << "Inverse:\n" << aa.inverse(&a) << endl;
    cout << "Inverse_transpose:\n" << aa.inverse_transpose(&a) << endl;


    //#################QUARTERNIOINS###########################

    // // quat to matrix
    // quat<float> q(1,2,3,4);
    // matf aa(q);
    // cout << aa << endl;

    // // operator= mat(quat)
    // quat<float> q(1,2,3,4);
    // matf ab = q;
    // cout << "This tests mat = quat" << endl;
    // cout << ab << endl;
    // cout << "The address of equated matrix " << &ab << endl; 
    // cout << "The address of quaternion " << &q << endl; 

    // ortho
    // cout << "the orthographic projection" << endl;
    // cout << matd::Ortho(1, 2,3,4,5,6) << endl;
    // glm::mat4x4 orth = glm::ortho(1,2,3,4,5,6);
    // cout << "from glm:" << glm::to_string(orth) << endl;

    // cout << "the perspective projection" << endl;
    // cout << matd::Perspective(5, 8,34,18) << endl;
    // glm::mat4x4 pers = glm::perspective(5.0f,8.0f,34.0f,18.0f);
    // cout << "from glm:" << glm::to_string(pers) << endl;

    // cross product matrices
        // vecf a(1,0,0), b(0,1,0);
    // cout << a.cross(b) << endl;
    // cout << matf::Crossproduct(a) * b << endl;

    // frustum
    // cout << "the frustum" << endl;
    // cout << matd::Frustum(5, 8,34,18, 5, 6) << endl;
    // glm::mat4x4 frus = glm::frustum(5.0f,8.0f,34.0f,18.0f, 5.0f, 6.0f);
    // cout << "from glm:" << glm::to_string(frus) << endl;

    //lookat
    // cout << "the lookat" << endl;
    // vecf eye(20,1,1);
    // glm::vec3 e(20.0f,1.0f,1.0f);

    // vecf center(0,0,0);
    // glm::vec3 c(0.0f,0.0f,0.0f);

    // vecf up(0,1,0);
    // glm::vec3 u(0.0f,1.0f,0.0f);

    // cout << matf::Lookat(eye, center, up) << endl;
    // glm::mat4x4 lookat = glm::lookAt(e,c,u);
    // cout << "from glm:\n" << glm::to_string(lookat) << endl;

    return 0;
}