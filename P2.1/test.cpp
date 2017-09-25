#include "../common/catch.hpp"
#include"quat.h"
#include"mat.h"

#include<glm/gtx/transform.hpp>
#include<glm/gtx/string_cast.hpp>

#include<iostream>
#include<cstdlib> // for random
#include<ctime> // for rand/srand


using namespace std;

// ranges of floats and doubles
const float LO = -800;
const float HI = 800;

// sizes
const size_t QUAT_LENGTH = 4;
const size_t ROWS = 4;
const size_t COLUMNS = 4;

// shorter names
typedef quat<float> quatf;
typedef quat<double> quatd;

template<class T>
// inline, otherwise line numbers are messed up
inline void check_quaternion_equal(quat<T> q1, quat<T> q2){
    CHECK(q1[0] == q2[0]);
    CHECK(q1[1] == q2[1]);
    CHECK(q1[2] == q2[2]);
    CHECK(q1[3] == q2[3]);
}

TEST_CASE("Quarternions," "[quat]"){
    SECTION("Multiplication"){
        quatf q1 = quatf(0.0f, 3.0f, 4.0f, 3.0f);
        quatf q2 = quatf(4.0f, 3.9f, -1.0f, -3.0f);
        quatf expected_product = quatf(1.3f, 3.0f, 36.7f, -6.6f);

        quatf prod = q1 * q2;

        check_quaternion_equal(prod, expected_product);
    }

    SECTION("Inverse"){
        quatf q1 = quatf(1.0f, 2.0f, 3.0f, 4.0f);
        quatf q1_inverse = q1.inverse();

        cout << q1_inverse << endl;
        quatf expected = quatf(1/30.0f, -1/15.0f, -1/10.0f, -2/15.0f);
        check_quaternion_equal(q1_inverse, expected);
    }

    SECTION("Conjugate"){
        quatf q1 = quatf(1.0f, 0.0f, 0.0f, 0.0f);
        quatf q2 = quatf(1.0f, 3.0f, 5.5f, 6.8f);
        quatf q3 = quatf(0.0f, 3.4f, 5.6f, 8.8f);

        quatf q1_conj = q1.conjugate();
        quatf q2_conj = q2.conjugate();
        quatf q3_conj = q3.conjugate();

        quatf q1_expected_conj(1.0f, 0.0f, 0.0f, 0.0f);
        quatf q2_expected_conj(1.0f, -3.0f, -5.5f, -6.8f);
        quatf q3_expected_conj(0.0f, -3.4f, -5.6f, -8.8f);

        check_quaternion_equal(q1_conj, q1_expected_conj);
        check_quaternion_equal(q2_conj, q2_expected_conj);
        check_quaternion_equal(q3_conj, q3_expected_conj);
    }

    SECTION("Operator *="){
        quatf q1 = quatf(0.0f, 3.0f, 4.0f, 3.0f);
        quatf* original_address = &q1;
        quatf q2 = quatf(4.0f, 3.9f, -1.0f, -3.0f);

        q1 *= q2;

        quatf expected_product = quatf(1.3f, 3.0f, 36.7f, -6.6f);

        check_quaternion_equal(q1, expected_product);
        CHECK(original_address == & q1);
    }
}

template<class T>
// inline, otherwise line numbers are messed up
inline void check_matrix_equal(mat<T> m1, mat<T> m2){
    for(size_t ii = 0; ii < 4; ii++){
        for(size_t jj = 0; jj < 4; jj++){
            CHECK(m1(ii, jj) == m2(ii, jj));
        }
    }
}

TEST_CASE("Matrix Extensions", "[mat_ext]"){
    const matf identity(1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1);

    const matf     zero(0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0,
                        0, 0, 0, 0);

    SECTION("Zero"){
        check_matrix_equal(matf::Zero(), zero);
    }

    SECTION("Identity"){
        check_matrix_equal(matf::Identity(), identity);
    }

    SECTION("Translation"){
        vecf v1(1.0f, 2.0f, 3.0f);
        matf expected1(1, 0, 0, 1.0f,
                      0, 1, 0, 2.0f,
                      0, 0, 1, 3.0f,
                      0, 0, 0, 1.0f);

        vecf v2(-1.0f, 0.0f, 2.0f);
        matf expected2(1, 0, 0, -1.0f,
                       0, 1, 0, 0.0f,
                       0, 0, 1, 2.0f,
                       0, 0, 0, 1.0f);

        check_matrix_equal(matf::Translation(v1), expected1);
        check_matrix_equal(matf::Translation(v2), expected2);
    }

    SECTION("Rotation"){
        vecf v1(1.0f, 1.0f, -1.0f);
        v1.normalize();
        // for a rotation of 30 deg
        matf expected1(0.9106836, 0.3333333, 0.2440169, 0,
                       0.-0.2440169, 0.9106836, -0.3333333, 0,
                       -0.3333333, 0.2440169, 0.9106836, 0,
                       0.000000, 0.0000000, 0.000000, 1);

        
        check_matrix_equal(matf::Rotation(30,v1), expected1);

        vecf v2(0.0f, 0.2f, 3.3f);
        v2.normalize();
        //for rotation of 360 deg, we expect identity
        check_matrix_equal(matf::Rotation(360, v2), identity);
    }

    // Test cases from https://github.com/google/mathfu/blob/master/unit_tests/matrix_test/matrix_test.cpp
    SECTION("Perspective"){
        matf expected1(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, -1, 0,
                       0, 0, -1, 0);

        // cout << matf::Perspective(atan(1)*2, 1,0,1);
        check_matrix_equal(expected1, matf::Perspective(atan(1)*2, 1, 0, 1));

        //wide fov
        matf expected2(0.5, 0, 0, 0,
                       0, 0.5, 0, 0,
                       0, 0, -1, 0,
                       0, 0, -1, 0);
        
        check_matrix_equal(expected2, matf::Perspective(atan(2)*2, 1, 0, 1));

        // narow fov
        matf expected3(10, 0, 0, 0,
                       0, 10, 0, 0,
                       0, 0, -1, 0,
                       0, 0, -1, 0);

        check_matrix_equal(expected3, matf::Perspective(atan(0.1)*2, 1,0,1));

        //deeper view frustum

        // CHECK THIS
        // matf expected4(1, 0, 0, 0,
        //                0, 1, 0, 0,
        //                0, 0, -0.5, 2,
        //                0 ,0 , -1, 0);

        // cout << matf::Perspective(atan(1)*2, 1, -2, 2);
        // check_matrix_equal(expected4, matf::Perspective(atan(1)*2, 1, -2, 2));
    }

    SECTION("LookAt"){
        // looking along x
        vecf eye(0, 0, 0), center(1, 0, 0), up(0, 1, 0);
        matf expected(0, 0, 1, 0,
                      0, 1, 0, 0,
                     -1, 0, 0, 0,
                      0, 0, 0, 1);
    
        check_matrix_equal(expected, matf::Lookat(eye, center, up));
    }

    SECTION("Ortho"){
        // normalized
        matf expected1(1, 0, 0, -1,
                      0, 1, 0, -1,
                      0, 0, 1, 1,
                      0, 0, 0, 1);
        
        check_matrix_equal(expected1, matf::Ortho(0, 2, 0, 2, 2, 0));

        // narrow 
        matf expected2(1, 0, 0, -2,
                       0, 1, 0, -1,
                       0, 0, 1,  1,
                       0, 0, 0,  1);

        check_matrix_equal(expected2, matf::Ortho(1, 3, 0, 2, 2, 0));
    }

    SECTION("Scale"){
        vecf s1(1, 2, 3);
        matf expected1(1, 0, 0, 0,
                      0, 2, 0, 0,
                      0, 0, 3, 0,
                      0, 0, 0, 1);
        
        check_matrix_equal(expected1, matf::Scale(s1));

        vecf s2(1, 1, 54);

        matf expected2(1, 0, 0, 0,
                       0, 1, 0, 0,
                       0, 0, 54, 0,
                       0, 0, 0, 1);

        check_matrix_equal(expected2, matf::Scale(s2));
    }

    SECTION("Frustum"){
        matf expected(3.333333, 0.000000, 4.333333, 0.00000,
                       0.000000, -0.625000, -3.250000, 0.000000,
                       0.000000, 0.000000, -11.00000, -60.000000,
                       0.000000, 0.000000, -1.00000, 0.0000000);
        
        check_matrix_equal(expected, matf::Frustum(5.0f,8.0f,34.0f,18.0f, 5.0f, 6.0f));
    }

    SECTION("Cross Product"){
        vecf v(1, 2, 3);
        matf expected(0, -3, 2, 0,
                      3, 0, -1, 0,
                      -2, 1, 0, 0,
                      0, 0, 0, 0);
    }
}
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
    // vecdcout << "Identity\n" << matd::Identity() << endl;
    // cout << "Translation(1,1,1,0):\n" << matf::Translation(vecf(1, 1, 1, 0)) << endl;
    // glm::vec3 translate(1,1,1);
    // glm::mat4x4 ax = glm::translate(translate);
    // cout << "from glm: \n" << glm::to_string(ax) << endl;

    // cout << "Translation(3,1,2,0):\n" << matd::Translation(vecd(3, 1, 2, 0)) << endl;

    // matf aa(1,2,3,4,1,5,7,3,9,10,11,1,13,4,15,6); // aa.identity();;
   // aa = aa*3;
    // matf rot = matf::Rotation(30, vecf(1,0,0));
    // cout << "Identity matrix" << endl;
    // cout << "Subdeterminant 1,3:\n" << rot.subdeterminant(3,2) << endl;
    // cout << std::endl << "Determinant \n" << rot.adjoint() << endl;
    // cout << "Rotation about x \n" << matf::Rotation(30, vecf(1/sqrt(2), 1/sqrt(2), 0)) << endl;
    // glm::vec3 scale( 1.0f, 2.0f,3.0f);
    // glm::mat4x4 ax = glm::scale(scale);
    // cout << "scale  by 1,2,3 \n" << matf::Scale(vecf(1, 2, 3)) << endl;

    // cout << "from glm: \n" << glm::to_string(ax) << endl;

    // cout << "Subdeterminant(1,3):\n" << aa.subdeterminant(1,1) << endl;
    // float a = aa.determinant();
    // cout << "adjoint:\n" << aa.adjoint() << endl;
    // cout << "Inverse:\n" << aa.inverse(&a) << endl;
    // cout << "Inverse_transpose:\n" << aa.inverse_transpose(&a) << endl;

    // // matf rot = matf::Rotation(30, vecf(1,0,0));
    // cout << "roatation matrix for 30 deg about x " << rot << endl;
    // glm::tvec3<float> y(1,0,0);
    // glm::mat4x4 glm_rot = glm::rotate(float(30), y);
    // cout << "glm says " << glm::to_string(glm_rot);
    // float det_rot = rot.determinant();
    // cout << "it's determinant " << det_rot << endl;
    //  cout << "it's adjoint " << rot.adjoint() << endl;
    // cout << "it's inverse " << rot.inverse(&det_rot) << endl;
    // cout << "their product " << (const matf)rot.inverse(&det_rot) * rot << endl;

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


