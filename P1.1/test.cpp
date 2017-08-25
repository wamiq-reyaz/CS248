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
const float eps = 1e-5;

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
    for(int ii = 0; ii < 4; ii++){
        a[ii] = LO + static_cast<float> (rand() /
                            static_cast<float>(RAND_MAX / (HI - LO))); 
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
    for(int ii = 0; ii < 4; ii++){
        a[ii] = LO + static_cast<double> (rand() /
                            static_cast<double>(RAND_MAX/ (HI - LO))); // 64 is arbitrary
    }
    // fill the vector
    vecd temp(a[0], a[1], a[2], a[3]);
    return temp;
    }

float rand_f(void){
    return LO + static_cast<float> (rand() /
                static_cast<float>(RAND_MAX/ (HI - LO))); // div prevents overflow (generally)
}

double rand_d(void){
    return LO + static_cast<double> (rand() /
                static_cast<double>(RAND_MAX/ (HI - LO)));
}

template<class T>
void check(vec<T> v, T expected[]){
/* Args: vec<T> v, the vector to be checked
*         T expected[], the expected element values
*  Return : void
*  This function checks the elements vs the expected 
*  value of the elements
*/
    for(int ii = 0; ii < 4; ii++){
        CHECK(fabs(v(ii) - expected[ii]) < eps); // - 1 because indexing begins at 0
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
    for(int ii = 1; ii <= 4; ii++){
        CHECK(fabs(v.length2(ii) - 
         length_array[ii - 1]) < eps); 
    }
}

template<class T>
void test_length(vec<T> v, T length_array[]){
    // Like test_length2 but for length
    for(int ii = 1; ii <= 4; ii++){
        CHECK(fabs(v.length(ii) - 
         length_array[ii - 1]) < eps);
    }
}

//==========================================================================//

/* This is the collection of tests for the vector class
*  The tests name and argument is in the TEST_CASE
*  arguments
*/

TEST_CASE("Vectors", "[vec]"){
    //seed for the PRNG
    srand(100);

    SECTION("Default Constructor"){
        // init some vectors needed for testing
        vecf vf_init_default; // by default all elements should be zero
        vecd vd_init_default;

        CHECK(&vf_init_default != NULL);
        CHECK(&vd_init_default != NULL);

        SECTION("Zero initialization")
        // test that the 4 elements are zero
        for(int ii = 0; ii < 4; ii++){
            CHECK(vd_init_default(ii) == 0);
            CHECK(vd_init_default(ii) == 0);
        }
    }   

    SECTION("General Constructor"){
        // init some vectors needed for testing
        vecf vf_init_1(1); // just populate first element with 1
        vecd vd_init_1(1);

        CHECK(&vf_init_1 != NULL);
        CHECK(&vd_init_1 != NULL);

        SECTION("First Element Intialization")
        // test that the first element is 1
        CHECK(vd_init_1(0) == 1);
        CHECK(vd_init_1(0) == 1);

        // and the rest of the elements are 0
        for(int ii = 1; ii < 4; ii++){
            CHECK(vd_init_1(ii) == 0);
            CHECK(vd_init_1(ii) == 0);
        }   

        // now test with full lengths
        vecf vf_init_1_2_3_4(1, 2, 3, 4);
        vecd vd_init_1_2_3_4(1, 2, 3, 4);

        for(int ii = 0; ii < 4; ii++){
            CHECK(vd_init_1_2_3_4(ii) == ii + 1); // ii goes from 0 to 3
            CHECK(vd_init_1_2_3_4(ii) == ii + 1); // so add 1
        }
    }

    SECTION("Copy Constructor"){
        vecf vf_init_1_2_3_4(1, 2, 3, 4); 
        vecd vd_init_1_2_3_4(1, 2, 3, 4);

        // create vector that copies vx_init_1_2_3_4
        vecf vf_copy(vf_init_1_2_3_4); 
        vecd vd_copy(vd_init_1_2_3_4);

        CHECK(&vf_init_1_2_3_4 != NULL);
        CHECK(&vd_init_1_2_3_4 != NULL);

        // test that the elements in the two vectors are equal
        for(int ii = 0; ii < 4; ii++){
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
        CHECK(vf_zero.length2(0) - 0 < eps);
        CHECK(vf_unit.length2(0) - 0 < eps);
        CHECK(vf_sqrt2.length2(0) - 0 < eps);
        CHECK(vf_sqrt2_neg.length2(0) - 0 < eps);
        CHECK(vf_sqrt30.length2(0) - 0 < eps);
        CHECK(vd_unit.length2(0) - 0 < eps);
        CHECK(vd_sqrt2.length2(0) - 0 < eps);
        CHECK(vd_sqrt2_neg.length2(0) - 0 < eps);
        CHECK(vd_sqrt30.length2(0) - 0 < eps);

        /*
        * We perform the tests for the vec<float> first
        */

        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vf_zero.length2(ii)) - 0 < eps); // no matter the no of elements, length is zero
        }

        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vf_unit.length2(ii)) - 1 < eps); // length is always 1
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

        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vd_zero.length2(ii)) - 0 < eps); 
        }

        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vd_unit.length2(ii)) - 1 < eps); 
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
        CHECK(vf_zero.length(0) - 0 < eps);
        CHECK(vf_unit.length(0) - 0 < eps);
        CHECK(vf_sqrt2.length(0) - 0 < eps);
        CHECK(vf_sqrt2_neg.length(0) - 0 < eps);
        CHECK(vf_sqrt30.length(0) - 0 < eps);
        CHECK(vd_unit.length(0) - 0 < eps);
        CHECK(vd_sqrt2.length(0) - 0 < eps);
        CHECK(vd_sqrt2_neg.length(0) - 0 < eps);
        CHECK(vd_sqrt30.length(0) - 0 < eps);

        /*
        * We perform the tests for the vec<float> first
        */
        
        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vf_zero.length(ii) - 0) < eps); 
        }

        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vf_unit.length(ii) - 1) < eps); 
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

        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vd_zero.length2(ii) - 0) < eps); 
        }

        for(int ii = 1; ii <= 4; ii++){
            CHECK(fabs(vd_unit.length2(ii) - 1) < eps); 
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
            check(a_vecf , expected);
        }

        // zero double 
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double expected[4]; // intialized to zero
            a_vecd = v * 0;
            check(a_vecd , expected);
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
            check(a_vecf , expected);
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
            check(a_vecf , expected);
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
            check(a_vecd , expected);
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
            check(a_vecd , expected);
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
            check(a_vecf , expected);
        }

        // zero double 
        for(int ii = 0; ii < 100; ii++){
            vecd v = rand_vecd(), a_vecd;
            double expected[4]; // intialized to zero
            a_vecd = v * 0;
            check(a_vecd , expected);
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
            check(a_vecf , expected);
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
            check(a_vecf , expected);
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
            check(a_vecd , expected);
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
            check(a_vecd , expected);
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
            check(a_vecf, expected);
        }
    }

    SECTION("Operator =="){
        // all vectors must be equal to themselves
        for(int ii = 0; ii < 100; ii++){
            vecf v = rand_vecf(), a_vecf;
            a_vecf = v;
            CHECK((a_vecf == v) == true); // the comparison of a, v should yield true
        }
    }
    
}

