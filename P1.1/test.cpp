#define CATCH_CONFIG_MAIN
#include "catch.hpp"

// the actual functions are defined here
#include "mat.h"
#include "vec.h"

// some other libraries are needed as well
#include<cmath>
#include<cstdlib> // for random
#include<ctime> // for rand/srand

// use this as a threshold
const float EPS = 1e-5;

// Vector Length
const int VEC_LEN = 4;

// Matrix dim
const int MAT_H = 4; //height
const int MAT_W = 4; //width
const int MAT_LEN = MAT_H * MAT_W;

//===================================================================//

// ranges of floats and doubles
const float LO = -800;
const float HI = 800;

vecf rand_vecf(void){
/* Arg: void
*  Return: vecf
* This helper function returns a vector of random
* floats
*/
    float a[4]; // elements are floats
    for(int ii = 0; ii < VEC_LEN; ii++){
        //
        a[ii] = LO + static_cast<float> (rand() /
                            static_cast<float>(RAND_MAX / (HI - LO))); // TODO insert SE link
    }
    // fill the vector
    vecf temp(a[0], a[1], a[2], a[3]);
    return temp;
}

vecd rand_vecd(void){
/* Arg: void
*  Return: vecd
* This helper function returns a vector of random
* doubles
*/  

    double a[4]; // elements are doubles
    for(int ii = 0; ii < VEC_LEN; ii++){
        a[ii] = LO + static_cast<double> (rand() /
                            static_cast<double>(RAND_MAX/ (HI - LO))); 
    }
    // fill the vector
    vecd temp(a[0], a[1], a[2], a[3]);
    return temp;
    }

float rand_f(void){
    return LO + static_cast<float> (rand() /
                static_cast<float>(RAND_MAX/ (HI - LO))); 
}

double rand_d(void){
    return LO + static_cast<double> (rand() /
                static_cast<double>(RAND_MAX/ (HI - LO)));
}

template<class T>
// inline helps get the line number during testing
inline void check_vec(vec<T> v, T expected[]){ 
/* Args: vec<T> v, the vector to be checked
*         T expected[], the expected element values
*  Return : void
*  This function checks the elements vs the expected 
*  value of the elements
*/  

        for(int ii = 0; ii < VEC_LEN; ii++){
            CHECK(fabs(v(ii) - expected[ii]) < EPS); 
        }
}

/* helper functions for looping over arrays of 
* length 4 and comparing lengths
*/

template<class T>
void test_length2(vec<T> v, T length_array[]){
/* The length_array is an array of expected lengths
* such that length_array[n] is expected when v.length(n)
* is called.
* We check for each length(n) vs expected length
*/
    for(int ii = 1; ii <= VEC_LEN; ii++){
        CHECK(fabs(v.length2(ii) - 
         length_array[ii - 1]) < EPS); 
    }
}

template<class T>
void test_length(vec<T> v, T length_array[]){
    // Like test_length2 but for length
    for(int ii = 1; ii <= VEC_LEN; ii++){
        CHECK(fabs(v.length(ii) - 
         length_array[ii - 1]) < EPS);
    }
}

//==========================================================================//

/* This is the collection of tests for the vector class
*  The test's name is in the SECTION arguments
*/

TEST_CASE("Vectors", "[vec]"){
    //seed for the PRNG
    srand(time(0));

    SECTION("Default Constructor"){
        // init some vectors needed for testing
        vecf vf_init_default; // by default all elements should be zero
        vecd vd_init_default;

        SECTION("Zero initialization")
        // test that the 4 elements are zero
        for(int ii = 0; ii < VEC_LEN; ii++){
            CHECK(vf_init_default(ii) == 0);
            CHECK(vd_init_default(ii) == 0);
        }
    }   

    SECTION("General Constructor"){
        // init some vectors needed for testing
        vecf vf_init_1(1); // just populate first element with 1
        vecd vd_init_1(1);

        SECTION("First Element Intialization")
        // test that the first element is 1
        CHECK(vf_init_1(0) == 1);
        CHECK(vd_init_1(0) == 1);

        // and the rest of the elements are 0
        for(int ii = 1; ii < VEC_LEN; ii++){
            CHECK(vf_init_1(ii) == 0);
            CHECK(vd_init_1(ii) == 0);
        }   

        // now test with full lengths
        vecf vf_init_1_2_3_4(1, 2, 3, 4);
        vecd vd_init_1_2_3_4(1, 2, 3, 4);

        for(int ii = 0; ii < VEC_LEN; ii++){
            CHECK(vf_init_1_2_3_4(ii) == ii + 1); // ii goes from 0 to 3
            CHECK(vd_init_1_2_3_4(ii) == ii + 1); // so add 1
        }
    }

    SECTION("Copy Constructor"){
        vecf vf_init_1_2_3_4(1, 2, 3, 4); 
        vecd vd_init_1_2_3_4(1, 2, 3, 4);

        // create vector that copies vx_init_1_2_3_4
        vecf vf_copy(vf_init_1_2_3_4); 
        vecd vd_copy(vd_init_1_2_3_4);

        // test that the elements in the two vectors are equal
        for(int ii = 0; ii < VEC_LEN; ii++){
            CHECK(vf_copy(ii) == vf_init_1_2_3_4(ii));
            CHECK(vd_copy(ii) == vd_init_1_2_3_4(ii));
        }
    }

    SECTION("length2"){
        // TODO: make a function that does all this

        vecf vf_zero; //length 0
        vecf vf_unit(1); // unit vector
        vecf vf_sqrt2(1,1); // sqrt2
        vecf vf_sqrt2_neg(1,1); // sqrt2 with negative components
        vecf vf_sqrt30(1,2,3,4); // sqrt30
        
        vecd vd_zero;
        vecd vd_unit(1);
        vecd vd_sqrt2(1,1);
        vecd vd_sqrt2_neg(1,1);
        vecd vd_sqrt30(1,2,3,4); // sqrt30

        // length2(0) should return 0
        CHECK(vf_zero.length2(0) - 0 < EPS);
        CHECK(vf_unit.length2(0) - 0 < EPS);
        CHECK(vf_sqrt2.length2(0) - 0 < EPS);
        CHECK(vf_sqrt2_neg.length2(0) - 0 < EPS);
        CHECK(vf_sqrt30.length2(0) - 0 < EPS);
        CHECK(vd_unit.length2(0) - 0 < EPS);
        CHECK(vd_sqrt2.length2(0) - 0 < EPS);
        CHECK(vd_sqrt2_neg.length2(0) - 0 < EPS);
        CHECK(vd_sqrt30.length2(0) - 0 < EPS);

        /*
        * We perform the tests for the vec<float> first
        */

        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vf_zero.length2(ii)) - 0 < EPS); // no matter the no of elements, length is zero
        }

        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vf_unit.length2(ii)) - 1 < EPS); // length is always 1
        }

        float vf_sqrt2_length2[4] = {1, 2, 2, 2}; // array holding the 4 expectred lengths  
        test_length2(vf_sqrt2, vf_sqrt2_length2);

        // lengths for the negative vector are the same
        float vf_sqrt2_neg_length2[4] = {1, 2, 2, 2}; 
        test_length2(vf_sqrt2_neg, vf_sqrt2_neg_length2);

        // the vector (1,2,3,4)
        float vf_sqrt30_length2[4] = {1, 5, 14, 30};
        test_length2(vf_sqrt30, vf_sqrt30_length2);

        /*
        * We now perform the tests for vec<double>
        */

        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vd_zero.length2(ii)) - 0 < EPS); 
        }

        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vd_unit.length2(ii)) - 1 < EPS); 
        }

        /* We create arrays holding lengths of vectors so that they 
        *  can be accessed using loops
        */

        double vd_sqrt2_length2[4] = {1, 2, 2, 2};
        test_length2(vd_sqrt2, vd_sqrt2_length2);

        // lengths for the negative vector are the same
        double vd_sqrt2_neg_length2[4] = {1, 2, 2, 2}; 
        test_length2(vd_sqrt2_neg, vd_sqrt2_neg_length2);

        // the vector (1,2,3,4)
        double vd_sqrt30_length2[4] = {1, 5, 14, 30};
        test_length2(vd_sqrt30, vd_sqrt30_length2);
    }

    SECTION("length"){
        vecf vf_zero; 
        vecf vf_unit(1);
        vecf vf_sqrt2(1,1); 
        vecf vf_sqrt2_neg(1,1);
        vecf vf_sqrt30(1,2,3,4);
        
        vecd vd_zero;
        vecd vd_unit(1);
        vecd vd_sqrt2(1,1);
        vecd vd_sqrt2_neg(1,1);
        vecd vd_sqrt30(1,2,3,4);

        // length(0) should return 0
        CHECK(vf_zero.length(0) - 0 < EPS);
        CHECK(vf_unit.length(0) - 0 < EPS);
        CHECK(vf_sqrt2.length(0) - 0 < EPS);
        CHECK(vf_sqrt2_neg.length(0) - 0 < EPS);
        CHECK(vf_sqrt30.length(0) - 0 < EPS);
        CHECK(vd_unit.length(0) - 0 < EPS);
        CHECK(vd_sqrt2.length(0) - 0 < EPS);
        CHECK(vd_sqrt2_neg.length(0) - 0 < EPS);
        CHECK(vd_sqrt30.length(0) - 0 < EPS);

        /*
        * We perform the tests for the vec<float> first
        */
        
        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vf_zero.length(ii) - 0) < EPS); 
        }

        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vf_unit.length(ii) - 1) < EPS); 
        }

        /* We create arrays holding lengths of vectors so that they 
        *  can be accessed using loops
        */

        float vf_sqrt2_length[4] = {1, sqrt(2), sqrt(2), sqrt(2)}; 
        test_length(vf_sqrt2, vf_sqrt2_length);

        // lengths for the negative vector are the same
        float vf_sqrt2_neg_length[4] = {1, sqrt(2), sqrt(2), sqrt(2)}; 
        test_length(vf_sqrt2_neg, vf_sqrt2_neg_length);

        // the vector (1,2,3,4)
        float vf_sqrt30_length[4] = {1, sqrt(5), sqrt(14), sqrt(30)};
        test_length(vf_sqrt30, vf_sqrt30_length);

        /*
        * We now perform the tests for vec<double>
        */

        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vd_zero.length2(ii) - 0) < EPS); 
        }

        for(int ii = 1; ii <= VEC_LEN; ii++){
            CHECK(fabs(vd_unit.length2(ii) - 1) < EPS); 
        }

        /* We create arrays holding lengths of vectors so that they 
        *  can be accessed using loops
        */

        double vd_sqrt2_length[4] = {1, sqrt(2), sqrt(2), sqrt(2)}; 
        test_length(vd_sqrt2, vd_sqrt2_length);

        // lengths for the negative vector are the same
        double vd_sqrt2_neg_length[4] = {1, sqrt(2), sqrt(2), sqrt(2)}; 
        test_length(vd_sqrt2_neg, vd_sqrt2_neg_length);

        // the vector (1,2,3,4)
        double vd_sqrt30_length[4] = {1, sqrt(5), sqrt(14), sqrt(30)};
        test_length(vd_sqrt30, vd_sqrt30_length);
    }

    SECTION("Scalar multiplication *"){
        /* The cases to test are zero, negative numbers, positive numbers
        */

        // test zero float first 
        for(int ii = 0; ii < 100; ii++){
            // intialize vectors
            vecf v = rand_vecf(), a_vecf;
            float expected[4]; // intialized to zero
            a_vecf = v * 0;
            check_vec(a_vecf , expected);
        }

        // zero double 
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double expected[4]; // intialized to zero
            a_vecd = v * 0;
            check_vec(a_vecd , expected);
        }

        SECTION("Negative floats")
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            float randf = rand_f(); // an number to *
            float expected[4]; 

            a_vecf = v * -randf; // notice -randf

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) * -randf;
            }
            check_vec(a_vecf , expected);
        }

        SECTION("Postive floats")
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            float randf = rand_f(); // an number to *
            float expected[4]; 

            a_vecf = v * randf;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) * randf;
            }
            check_vec(a_vecf , expected);
        }

        SECTION("Negative doubles")
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double randd = rand_d(); // an number to *
            double expected[4]; 

            a_vecd = v * -randd; // notice -randf

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) * -randd;
            }
            check_vec(a_vecd , expected);
        }

        SECTION("Postive doubles")
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double randd = rand_d(); // an number to *
            double expected[4]; 

            a_vecd = v * randd;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) * randd;
            }
            check_vec(a_vecd , expected);
        }
    }

    SECTION("Scalar division"){
        /* The cases to test are zero, negative numbers, positive numbers
        */

        // test zero float first 
        for(int ii = 0; ii < 100; ii++){
            // intialize vectors
            vecf v = rand_vecf(), a_vecf;
            float expected[4]; // intialized to zero
            a_vecf = v * 0;
            check_vec(a_vecf , expected);
        }

        // zero double 
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double expected[4]; // intialized to zero
            a_vecd = v * 0;
            check_vec(a_vecd , expected);
        }

        SECTION("Negative floats")
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            float randf = rand_f(); // an number to /
            float expected[4]; 

            a_vecf = v / -randf; // notice -randf

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) / -randf;
            }
            check_vec(a_vecf , expected);
        }

        SECTION("Postive floats")
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            float randf = rand_f(); // an number to /
            float expected[4]; 

            a_vecf = v / randf;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) / randf;
            }
            check_vec(a_vecf , expected);
        }

        SECTION("Negative doubles")
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double randd = rand_d(); 
            double expected[4]; 

            a_vecd = v / -randd; // notice -randf

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) / -randd;
            }
            check_vec(a_vecd , expected);
        }

        SECTION("Postive doubles")
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double randd = rand_d();
            double expected[4]; 

            a_vecd = v / randd;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj) / randd;
            }
            check_vec(a_vecd , expected);
        }
    }

    SECTION("Operator ="){
        for(int ii = 0; ii < 100; ii++){
            //floats
            vecf v = rand_vecf(), a_vecf;
            float expected[4];

            a_vecf = v;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj);
            }
            check_vec(a_vecf, expected);
        }

        for(int ii = 0; ii < 100; ii++){
            //doubles
            vecd v = rand_vecd(), a_vecd;
            double expected[4];

            a_vecd = v;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v(jj);
            }
            check_vec(a_vecd, expected);
        }
    }

    SECTION("Operator =="){
        // all vectors must be equal to themselves
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            a_vecf = v;
            CHECK((a_vecf == v) == true); // the comparison of a, v should yield true
        }

        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            a_vecd = v;
            CHECK((a_vecd == v) == true); // the comparison of a, v should yield true
        }
    }

    SECTION("Operator !="){
        //floats
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            a_vecf = v;
            // randomly perturb the values of a_vecf
            a_vecf(ii % 4) = a_vecf(ii % 4) + rand_f();
            CHECK((a_vecf != v) == true); // the comparison of a, v should yield true
        }

        //doubles
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            a_vecd = v;
            // randomly perturb the values of a_vecd
            a_vecd(ii % 4) = a_vecd(ii % 4) + rand_d();
            CHECK((a_vecd != v) == true); // the comparison of a, v should yield true
        }
    }

    SECTION("Operator -"){
        //floats
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            float expected[4];
            a_vecf = -v;

            for(int jj =0; jj < 4; jj++){
                expected[jj] = -v(jj);
            }

            check_vec(a_vecf, expected);
        }

        //doubles
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double expected[4];
            a_vecd = -v;

            for(int jj =0; jj < 4; jj++){
                expected[jj] = -v(jj);
            }

            check_vec(a_vecd, expected);
        }
    }
    
    SECTION("Operator +"){
        //floats
        for(int ii = 0; ii < 100; ii++){
            vecf v1 = rand_vecf(), v2 = rand_vecf(), a_vecf;
            float expected[4];
            a_vecf = v1 + v2;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) + v2(jj);
            }

            check_vec(a_vecf, expected);
        }

        //doubles
        for(int ii = 0; ii < 100; ii++){
            vecd v1 = rand_vecd(), v2 = rand_vecd(), a_vecd;
            double expected[4];
            a_vecd = v1 + v2;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) + v2(jj);
            }

            check_vec(a_vecd, expected);
        }
    }

    SECTION("Operator -(other"){
        //float
        vecf v1 = rand_vecf(), v2 = rand_vecf(), a_vecf;
        float expected[4];
        a_vecf = v1 - v2;

        for(int jj = 0; jj < 4; jj++){
            expected[jj] = v1(jj) - v2(jj);
        }
        
        check_vec(a_vecf, expected);

        //doubles
        for(int ii = 0; ii < 100; ii++){
            vecd v1 = rand_vecd(), v2 = rand_vecd(), a_vecd;
            double expected[4];
            a_vecd = v1 - v2;

            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) - v2(jj);
            }

            check_vec(a_vecd, expected);
        }
    }

    SECTION("Operator +="){
        /* This section needs to check the correctness of the addition
        *  and also that the returned object is the same, and not a copy
        *  by verifying pointer does not change
        */

        //floats
        for(int ii = 0; ii < 100; ii++){
            vecf v1 = rand_vecf(), v2 = rand_vecf();
            float expected[4];
            vecf* v1_orig_addr = &v1; //original address of v1

            // find expected values first. Otherwise the vec<T> is going to
            // change
            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) + v2(jj);
            }

            v1 += v2;        

            check_vec(v1, expected);

            //also check that v1 still is the same
            CHECK(&v1 == v1_orig_addr);
        }

        //doubles
        for(int ii = 0; ii < 100; ii++){
            vecd v1 = rand_vecd(), v2 = rand_vecd();
            double expected[4];
            vecd* v1_orig_addr = &v1; //original address of v1

            // find expected values first. Otherwise the vec<T> is going to
            // change
            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) + v2(jj);
            }

            v1 += v2;        

            check_vec(v1, expected);

            //also check that v1 still is the same
            CHECK(&v1 == v1_orig_addr);
        }
    }

    SECTION("Operator *="){
        /* *= takes a scalar and a vector so we need only one vector
            and a scalar
        */
        //floats
        for(int ii = 0; ii < 100; ii++){
            vecf v1 = rand_vecf();
            float expected[4], a_f = rand_f(); //a_f is the scalar, generated randomly
            vecf* v1_orig_addr = &v1; //original address of v1

            // find expected values first. Otherwise the vec<T> is going to
            // change
            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) * a_f;
            }

            v1 *= a_f;        

            check_vec(v1, expected);

            //also check that v1 still is the same
            CHECK(&v1 == v1_orig_addr);
        }

        //doubles
        for(int ii = 0; ii < 100; ii++){
            vecd v1 = rand_vecd();
            double expected[4], a_d = rand_d();
            vecd* v1_orig_addr = &v1; //original address of v1

            // find expected values first. Otherwise the vec<T> is going to
            // change
            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) * a_d;
            }

            v1 *= a_d;        

            check_vec(v1, expected);

            //also check that v1 still is the same
            CHECK(&v1 == v1_orig_addr);
        }
    }

    SECTION("Operator -="){
        //floats
        for(int ii = 0; ii < 100; ii++){
            vecf v1 = rand_vecf(), v2 = rand_vecf();
            float expected[4];
            vecf* v1_orig_addr = &v1; //original address of v1

            // find expected values first. Otherwise the vec<T> is going to
            // change
            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) - v2(jj);
            }

            v1 -= v2;        

            check_vec(v1, expected);

            //also check that v1 still is the same
            CHECK(&v1 == v1_orig_addr);
        }

        //doubles
        for(int ii = 0; ii < 100; ii++){
            vecd v1 = rand_vecd(), v2 = rand_vecd();
            double expected[4];
            vecd* v1_orig_addr = &v1; //original address of v1

            // find expected values first. Otherwise the vec<T> is going to
            // change
            for(int jj = 0; jj < 4; jj++){
                expected[jj] = v1(jj) - v2(jj);
            }

            v1 -= v2;        

            check_vec(v1, expected);

            //also check that v1 still is the same
            CHECK(&v1 == v1_orig_addr);
        }
    }

    SECTION("Dot product"){
        /* This section is gonna focus on only few examples with
        *  pre-known answers. Instead, for testing we use the properties
        * of dot product on random vectors. These are the properties:
        * 1 Commutativity
        * 2 Distributivity
        */

        SECTION("Known examples"){
            // float
            vecf xf(1), yf(0,1), zf(0, 0, 1), zerof;
            CHECK(xf.dot(yf) == 0);
            CHECK(xf.dot(zf) == 0);
            CHECK(yf.dot(zf) == 0);

            CHECK(xf.dot(zerof) == 0);
            CHECK(xf.dot(zerof) == 0);
            CHECK(yf.dot(zerof) == 0);

            //double
            vecd xd(1), yd(0,1), zd(0, 0, 1), zerod;
            CHECK(xd.dot(yd) == 0);
            CHECK(xd.dot(zd) == 0);
            CHECK(yd.dot(zd) == 0);

            CHECK(xd.dot(zerod) == 0);
            CHECK(xd.dot(zerod) == 0);
            CHECK(yd.dot(zerod) == 0);
        }

        SECTION("Commutativity"){
            /* We'll simply see if v1*v2 = v2.v1
            */
            // float
            for(int ii = 0; ii < 100; ii++){
                vecf v1 = rand_vecf(), v2 = rand_vecf();
                CHECK(fabs(v1.dot(v2) - v2.dot(v1)) < EPS); //the orders been changed
            }

            //double
            for(int ii = 0; ii < 100; ii++){
                vecd v1 = rand_vecd(), v2 = rand_vecd();
                CHECK(fabs(v1.dot(v2) - v2.dot(v1)) < EPS); 
            }
        }

        SECTION("Distributivity"){
            /* TODO: A lot of these tests fail because of floating point issues
            */
            // float
            for(int ii = 0; ii < 100; ii++){
                vecf v1 = rand_vecf(), v2 = rand_vecf(), v3 = rand_vecf();
                CHECK(fabs(v1.dot(v2 + v3) - (v1.dot(v2) + v1.dot(v3))) < EPS); //the RHS has been expanded
            }

            //double
            for(int ii = 0; ii < 100; ii++){
                vecd v1 = rand_vecd(), v2 = rand_vecd(), v3 = rand_vecd();
                CHECK(fabs(v1.dot(v2 + v3) - (v1.dot(v2) + v1.dot(v3))) < EPS); 
            }
        }
    }

    SECTION("Cross Product"){
        /* Again we are gonna use some known cases and a lot of cases for properties
        * The properties are:
        * 1 Anticommutativity
        * 2 Distributivity
        */
        SECTION("Known examples"){
            //floats
            vecf xf(1), yf(0,1), zf(0, 0, 1), zerof;
            CHECK(xf.cross(yf) == zf);
            CHECK(xf.cross(zf) == -yf);
            CHECK(yf.cross(zf) == xf);

            CHECK(xf.cross(zerof) == zerof);
            CHECK(xf.cross(zerof) == zerof);
            CHECK(yf.cross(zerof) == zerof);

            //double
            vecd xd(1), yd(0,1), zd(0, 0, 1), zerod;
            CHECK(xd.cross(yd) == zd);
            CHECK(xd.cross(zd) == -yd);
            CHECK(yd.cross(zd) == xd);

            CHECK(xd.cross(zerod) == zerod);
            CHECK(xd.cross(zerod) == zerod);
            CHECK(yd.cross(zerod) == zerod);
        }

        SECTION("Anticommutativity"){
            /* We'll simply see if v1*v2 = -v2*v1
            * or v2*v1 + v2.v1 == vec<T> zero
            */
            vecf zero_f;
            vecd zero_d;
            // float
            for(int ii = 0; ii < 100; ii++){
                vecf v1 = rand_vecf(), v2 = rand_vecf();
                CHECK(v1.cross(v2) + v2.cross(v1) == zero_f); //the orders been changed
            }

            //double
            for(int ii = 0; ii < 100; ii++){
                vecd v1 = rand_vecd(), v2 = rand_vecd();
                CHECK(v1.cross(v2) + v2.cross(v1) < zero_d); 
            }
        }

        SECTION("Distributivity"){
            vecf zero_f;
            vecd zero_d;
            //float
            for(int ii = 0; ii < 100; ii++){
                vecf v1 = rand_vecf(), v2 = rand_vecf(), v3 = rand_vecf();
                CHECK(v1.cross(v2 + v3) - (v1.cross(v2) + v1.cross(v3)) 
                                                            == zero_f); //the RHS has been expanded
            }

            //double
            for(int ii = 0; ii < 100; ii++){
                vecd v1 = rand_vecd(), v2 = rand_vecd(), v3 = rand_vecd();
                CHECK(v1.cross(v2 + v3) - (v1.cross(v2) + v1.cross(v3)) 
                                                            == zero_d); 
            }
        }
    }

    SECTION("Normalize"){
        /* We use a list of known vectors to test
        */

        //float
        //zero vector
        vecf zero;
        zero.normalize();
        for(int ii = 0; ii < VEC_LEN; ii++){
            CHECK(zero(ii) == 0);
        }

        vecf one(1);
        // first element is 1. Rest are 0
        float expected[4] = {1,0,0,0};
        for(int ii =0; ii < 5; ii++){ // four possible options 0 - 4
            one.normalize(ii); // doing this does not change the vector
            check_vec(one, expected); // expected also remains constant throughout the loop
        }
        
        //vec(1,1)
        float expected_1_1[5][4] = {{1,1,0,0},
                                    {1,1,0,0},
                                    {1/sqrt(2), 1/sqrt(2), 0, 0},
                                    {1/sqrt(2), 1/sqrt(2), 0, 0},
                                    {1/sqrt(2), 1/sqrt(2), 0, 0}};

        for(int ii = 0; ii < 5; ii++){
            vecf a(1,1);
            a.normalize(ii);
            check_vec(a, expected_1_1[ii]);
        }

        //vec(1,1,1)       
        float expected_1_1_1[5][4] = {{1,1,1,0},
                                    {1,1,1,0},
                                    {1/sqrt(2), 1/sqrt(2), 1, 0},
                                    {1/sqrt(3), 1/sqrt(3), 1/sqrt(3), 0},
                                    {1/sqrt(3), 1/sqrt(3), 1/sqrt(3), 0}};

        for(int ii = 0; ii < 5; ii++){
            vecf b(1,1, 1);
            b.normalize(ii);
            check_vec(b, expected_1_1_1[ii]);
        }

        //vec(1,1,1)       
        float expected_1_1_1_1[5][4] = {{1,1,1,1},
                                    {1,1,1,1},
                                    {1/sqrt(2), 1/sqrt(2), 1, 1},
                                    {1/sqrt(3), 1/sqrt(3), 1/sqrt(3), 1},
                                    {1/sqrt(4), 1/sqrt(4), 1/sqrt(4), 1/sqrt(4)}};

        for(int ii = 0; ii < 5; ii++){
        vecf c(1,1, 1, 1);
        c.normalize(ii);
        check_vec(c, expected_1_1_1_1[ii]);
        }
        
    }
}

//===================================================================//
// HELPER FUNCTION for matrices

matf rand_matf(void){
/* Arg: void
*  Return: matf
* This helper function returns a matrix of random
* floats
*/
    float a[MAT_LEN]; 
    for(int ii = 0; ii < MAT_LEN; ii++){
        a[ii] = LO + static_cast<float> (rand() /
                            static_cast<float>(RAND_MAX / (HI - LO))); 
    }

    matf temp(a[0],  a[1],  a[2],  a[3],
              a[4],  a[5],  a[6],  a[7],
              a[8],  a[9],  a[10], a[11],
              a[12], a[13], a[14], a[15]);
    return temp;
}

matd rand_matd(void){
/* Arg: void
*  Return: matd
* This helper function returns a matrix of random
* doubles
*/  

    double a[MAT_LEN]; 
    for(int ii = 0; ii < MAT_LEN; ii++){
        a[ii] = LO + static_cast<double> (rand() /
                            static_cast<double>(RAND_MAX/ (HI - LO))); 
    }

    matd temp(a[0],  a[1],  a[2],  a[3],
              a[4],  a[5],  a[6],  a[7],
              a[8],  a[9],  a[10], a[11],
              a[12], a[13], a[14], a[15]);
    return temp;
}


template<class T>
// inline helps get the line number during testing
inline void check_mat(mat<T> m, T expected[]){ 
/* Args: mat<T> m
*        T expected[]
*  Return : void
*  This function checks the elements vs the expected 
*  value of the elements
*/
    for(int ii = 0; ii < MAT_H; ii++){
        for(int jj = 0; jj < MAT_W; jj++){
            CHECK(fabs(m(ii, jj) - expected[4* ii + jj]) < EPS);
        } 
    }
}

//==========================================================================//

/* This is the collection of tests for the matrix class
*  The test's name is in the SECTION arguments
*/

TEST_CASE("Matrices", "[mat]"){
    //seed for the PRNG
    srand(time(0));

    SECTION("Constructor"){
        SECTION("Default Constructor"){
            matf mf_init_default; // zero elements
            matd md_init_default;   
            
            // test that the 16 elements are zero
            for(int ii = 0; ii < MAT_H; ii++){ // ii runs along height
                for(int jj = 0; jj < MAT_W; jj++){ // jj width
                    CHECK(mf_init_default(ii, jj) == 0);
                    CHECK(md_init_default(ii, jj) == 0);
                }
            }
        }
    
        SECTION("General Constructor"){
            matf mf_init_1(1);
            matd md_init_1(1);

            CHECK(mf_init_1(0, 0) == 1);
            CHECK(md_init_1(0, 0) == 1);

            // rest of the elements should be zero
            for(int ii = 1; ii < MAT_H; ii++){ // indices begin from 1
                for(int jj = 1; jj < MAT_W; jj++){
                    CHECK(mf_init_1(ii, jj) == 0);
                    CHECK(md_init_1(ii, jj) == 0);
                }
            }

            // check for full length
            matf ma_f(0,  1,  2,  3,
                      4,  5,  6,  7,
                      8,  9,  10, 11,
                      12, 13, 14, 15);

            matd mb_d(0,  1,  2,  3,
                      4,  5,  6,  7,
                      8,  9,  10, 11,
                      12, 13, 14, 15);

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    CHECK(ma_f(ii, jj) == float(4* ii + jj));
                    CHECK(mb_d(ii, jj) == double(4* ii + jj));
                }
            }
        }
    
        SECTION("Copy Constructor"){
            /* We initialize random matrices and check whether copied
            *  are equal to the original matrix
            */
            for(int kk = 0; kk < 100; kk++){
                // create random matrices
                matf ma_f = rand_matf();
                matd mb_d = rand_matd();

                // these values are expected
                float expected_f[MAT_LEN];
                double expected_d[MAT_LEN];

                for(int ii = 0; ii < MAT_H; ii++){
                    for(int jj = 0; jj < MAT_W; jj++){
                        expected_f[ii*4 + jj] = ma_f(ii, jj);
                        expected_d[ii*4 + jj] = mb_d(ii, jj);
                    }
                }

                // fill new matrices 
                matf ma_f_copy(ma_f);
                matd mb_d_copy(mb_d);              

                // check they are equal
                check_mat(ma_f_copy, expected_f);
                check_mat(mb_d_copy, expected_d);
            }
        }
    }

    SECTION("Identity"){
        //expected values
        float I_f[16] = {1 ,0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1};

        double I_d[16] = {1 ,0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1};

        for(int kk = 0; kk < 100; kk++){
            matf ma_f = rand_matf();
            matd mb_d = rand_matd();

            //convert to identity
            ma_f.identity();
            mb_d.identity();

            check_mat(ma_f, I_f);
            check_mat(mb_d, I_d);
        }
    }

    SECTION("Transpose"){
        // We are gonna try just a few cases
        //zero matrix
        matf ma_f; 
        matd mb_d;
        
        matf ma_f_T = ma_f.transpose();
        matd ma_d_T = mb_d.transpose();
        // can't use comparison operator
        // that hasn't been checked yet
        for(int ii = 0; ii < MAT_H; ii++){
            for(int jj = 0; jj < MAT_W; jj++){
                CHECK(ma_f(ii, jj)  == 0);
                CHECK(mb_d(ii, jj) == 0);
            }
        }

        //Some known matrix
        matf mc_f(1,  2,  3,  4,
                  5,  6,  7,  8,
                  9,  10, 11, 12,
                  13, 14, 15, 16);

        matd md_d(1,  2,  3,  4,
                  5,  6,  7,  8,
                  9,  10, 11, 12,
                  13, 14, 15, 16);

        float expected_f[16] = {1, 5, 9,  13,
                                2, 6, 10,  14,
                                3, 7, 11, 15, 
                                4, 8, 12, 16};

        double expected_d[16] = {1, 5,  9,  13,
                                 2, 6,  10,  14,
                                 3, 7,  11, 15, 
                                 4, 8 , 12, 16};

        check_mat(mc_f.transpose(), expected_f);
        check_mat(md_d.transpose(), expected_d);

    }

    SECTION("Operator +(other)"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2 = rand_matf(), a_matf;
            float expected[16];
            a_matf = m1 + m2;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) + m2(ii, jj);
                }
            }
            check_mat(a_matf, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2 = rand_matd(), a_matd;
            double expected[16];
            a_matd = m1 + m2;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) + m2(ii, jj);
                }
            }
            check_mat(a_matd, expected);
        }
    }

    SECTION("Operator -(other)"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2 = rand_matf(), a_matf;
            float expected[16];
            a_matf = m1 - m2;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) - m2(ii, jj);
                }
            }
            check_mat(a_matf, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2 = rand_matd(), a_matd;
            double expected[16];
            a_matd = m1 - m2;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) - m2(ii, jj);
                }
            }
            check_mat(a_matd, expected);
        }
    }

    SECTION("Operator *(scalar"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2_f;
            float scalar = rand_f();
            float expected[16];
            m2_f = m1 * scalar;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) * scalar;
                }
            }
            check_mat(m2_f, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2_d;
            double scalar = rand_f();
            double expected[16];
            m2_d = m1 * scalar;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) * scalar;
                }
            }
            check_mat(m2_d, expected);
        }
    }

    SECTION("Operator -(void)"){
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2_f;
            float expected[16];
            m2_f = -m1;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = -m1(ii, jj);
                }
            }
            check_mat(m2_f, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2_d;
            double expected[16];
            m2_d = -m1;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = -m1(ii, jj);
                }
            }
            check_mat(m2_d, expected);
        }
    }

    SECTION("Operator +="){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2 = rand_matf();
            float expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) + m2(ii, jj);
                }
            }
            m1 += m2;

            check_mat(m1, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2 = rand_matd();
            double expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) + m2(ii, jj);
                }
            }
            m1 += m2;

            check_mat(m1, expected);
        }
    }

    SECTION("Operator -=(other)"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2 = rand_matf();
            float expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) - m2(ii, jj);
                }
            }
            m1 -= m2;

            check_mat(m1, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2 = rand_matd();
            double expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) - m2(ii, jj);
                }
            }
            m1 -= m2;

            check_mat(m1, expected);
        }
    }

    SECTION("Operator *=(other)"){
        matf a(0,1,2,3,
               4,5,6,7,
               8,9,10,11,
               12,13,14,15);

        matf b(56, 62, 68, 74,
               152, 174, 196, 218,
               248, 286, 324, 362,
               344, 398, 452, 506);

        a *= a;

        CHECK((a == b) == true);
    }

    SECTION("Operator *(vec)"){
        // We use the property that k(Iv) = kv. k is a real number
        // I the identity matrix
        for(int kk = 0; kk < 100; kk++){
            //floats
            matf I(1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1);
         
            float k = rand_f();
    
            vecf v = rand_vecf();
            vecf ans = v * k;
    
            float expected[4]; // answer is a vector
            for(int ii = 0; ii < 4; ii++){
                expected[ii] = k * v(ii);
            }
    
            check_vec(ans, expected);
            }
            //doubles
            matd I(1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                0,0,0,1);
         
            double k = rand_d();
    
            vecd v = rand_vecd();
            vecd ans = v * k;
    
            double expected[4]; // answer is a vector
            for(int ii = 0; ii < 4; ii++){
                expected[ii] = k * v(ii);
            }
    
            check_vec(ans, expected);
    }

    SECTION("Operator ==(other)"){
        // all matrices must be equal to themselves
        for(int ii = 0; ii < 100; ii++){
            matf m = rand_matf(), a_matf;
            a_matf = m;
            CHECK((a_matf == m) == true); // the comparison of a, m should yield true
        }

        for(int ii = 0; ii < 100; ii++){
            matd m = rand_matd(), a_matd;
            a_matd = m;
            CHECK((a_matd == m) == true); // the comparison of a, m should yield true
        }
    }

    SECTION("Operator *(other)"){
        /* We test against a known matrix multiplication
        *  identity property IA = A; 
        */

        SECTION("Known result"){
            matf a(0.5, 3.5, 2, 1,
                   1.6, -0.5, 6, 8,
                   7, 230.56, 88, 96.96,
                   840.26, -444, 26, 10);

            matf b(10.1, 20.1, 23, 25.6,
                   25.6, 25.9, 25.9, 15.9,
                   75.9, 586.9, 254.9, 21.0,
                   -99.99, 0.96, 0.001, 0.1);

            // we now use matrix equality, as that has been already 
            // checked
            matf result(146.46, 1275.46, 611.951, 110.55,
                        -341.16, 3548.29, 1553.258, 159.81,
                        2957.2056, 57852.4856, 28563.80096, 5702.8,
                        -1906.274, 20658.626, 14453.79, 14998.1);

            matf c = (const matf) a * b;            
        }

        SECTION("Identity Property"){
            // the identity matrix
            matf id(1,0,0,0,  
                0,1,0,0,
                0,0,1,0,
                0,0,0,1);

            for(int ii = 0; ii < 100; ii++){
                // gen random vectors for testing 
                matf m1 = rand_matf();
                matf m2 = m1;

                m1 = (const matf) id * m1;

                CHECK((m1 == m2 ) == true);
            }
           

            matf random(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);

            matf d = (const matf) id * random;
        }
    }

    SECTION("Operator *=(scalar"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf();
            float scalar = rand_f();
            float expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) * scalar;
                }
            }
            m1 *= scalar;

            check_mat(m1, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd();
            double scalar = rand_f();
            double expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) * scalar;
                }
            }
            m1 *= scalar;

            check_mat(m1, expected);
        }
    }

    SECTION("Operator /=(scalar"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf();
            float scalar = rand_f();
            float expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) / scalar;
                }
            }
            m1 /= scalar;

            check_mat(m1, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd();
            double scalar = rand_f();
            double expected[16];

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) / scalar;
                }
            }
            m1 /= scalar;

            check_mat(m1, expected);
        }
    }

    SECTION("Operator /(scalar"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2_f;
            float scalar = rand_f();
            float expected[16];
            m2_f = m1 / scalar;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) / scalar;
                }
            }
            check_mat(m2_f, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2_d;
            double scalar = rand_f();
            double expected[16];
            m2_d = m1 / scalar;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj) / scalar;
                }
            }
            check_mat(m2_d, expected);
        }
    }

    SECTION("Operator =(other)"){
        // floats
        for(int kk = 0; kk < 100; kk++){
            matf m1 = rand_matf(), m2_f;
            float expected[16];
            m2_f = m1;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj);
                }
            }
            check_mat(m2_f, expected);
        }

        // doubles
        for(int kk = 0; kk < 100; kk++){
            matd m1 = rand_matd(), m2_d;
            double expected[16];
            m2_d = m1;

            for(int ii = 0; ii < MAT_H; ii++){
                for(int jj = 0; jj < MAT_W; jj++){
                    expected[4 * ii +jj] = m1(ii, jj);
                }
            }
            check_mat(m2_d, expected);
        }
    }

    SECTION("Operator !=(other)"){
        //floats
        for(int ii = 0; ii < 100; ii++){
            matf m = rand_matf(), a_matf;
            a_matf = m;
            // randomly perturb the values of a_vecf
            a_matf(ii % 4, ii % 4) = a_matf(ii % 4, ii %4) + rand_f();
            CHECK((a_matf != m) == true); 
        }

        //doubles
        for(int ii = 0; ii < 100; ii++){
            matd m = rand_matd(), a_matd;
            a_matd = m;
            // randomly perturb the values of a_vecd
            a_matd(ii % 4, ii % 4) = a_matd(ii % 4, ii %4) + rand_d();
            CHECK((a_matd != m) == true); 
        }
    }
}
