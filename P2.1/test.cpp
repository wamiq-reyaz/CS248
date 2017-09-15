#include"quat.h"

#include<iostream>

int main(int argc, char** argv){
    quat<float> q1(1,0,1,0), q2(1,0.5,0.5,0.75);
    vecf v1;
    std::cout << "Self multipliation and printing "<< q1*q2 << " " << v1 << std::endl;
    std::cout << "conjugate of 1,0,1,0 " << q1.conjugate() << std::endl;
    std::cout << "conjugate of 1, .5, .5, .75 " << q2.conjugate() << std::endl;
    q1 *= q2;
    std::cout << "operator *= with above quarternions "<< q1 << " " << std::endl;

    std::cout << "inverse of q1 " << q1.inverse() << std::endl;
    std::cout << "q1 * q1.inverse " << q1*q1.inverse() << std::endl; 

    std::cout << "inverse of q2 " << q2.inverse() << std::endl;
    std::cout << "q2 * q2.inverse " << q2*q2.inverse() << std::endl; 
    return 0;
}