#include"quat.h"

#include<iostream>

int main(int argc, char** argv){
    quat<float> q1(1,0,1,0), q2(1,0.5,0.5,0.75);
    vecf v1;
    std::cout << q1*q1 << " " << v1 << std::endl;
    return 0;
}