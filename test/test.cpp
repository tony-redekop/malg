#include "matrix2d.hpp"
#include <iostream>
#include <cassert>

int main() {
  std::cout << "TEST 0 : INSTANTIATE" << std::endl;

  // TEST 0 : case 0 : instantiate a Matrix2D<float> with user-supplied initializer 
  {
    malg::Matrix2D<float> mA(100, 50, 3.14);
    assert(mA[0][0] == (float)3.14 && mA[99][49] == (float)3.14);
    std::cout << "TEST 0 : case 0 : PASS" << std::endl;
  }

  // TEST 0 : case 1 : instantiate and value-initialize a Matrix2D<int>
  {
    malg::Matrix2D<int> mA(100, 50);
    assert(mA[0][0] == 0 && mA[99][49] == 0);
    std::cout << "TEST 0 : case 1 : PASS" << std::endl;
  }

  // TEST 0 : case 2 : instantiate and list-intialize a Matrix2D<double>
  {
    malg::Matrix2D<double> mA = {{1.0, 3.2, 6.0},
                                  {4.2, 6.1, 9.9}};
    assert(mA[0][0] == 1.0 && mA[0][1] == 3.2 && mA[0][2] == 6.0 &&
            mA[1][0] == 4.2 && mA[1][1] == 6.1 && mA[1][2] == 9.9); 
    std::cout << "TEST 0 : case 2 : PASS" << std::endl;
  }

  // TEST 0 : case 3 : instantiate a Matrix2D<int> with R < 1 or C < 1
  {
    try {
      // we expect an exception
      malg::Matrix2D<int> mD(0,1);
      std::cout << "TEST 0 : case 3 : FAIL" << std::endl;
    }
    catch(std::exception &e) {
      std::cout << "TEST 0 : case 3 : PASS" << std::endl;
    }
  }

  std::cout << "TEST 0 : COMPLETE" << std::endl << std::endl;
  std::cout << "TEST 1 : MULTIPLY" << std::endl;

  // TEST 1 : case 0 : matrix * matrix with compatible R and C dimensions 
  {
    malg::Matrix2D<int> mA = 
      {{0,0,1,0},
      {1,0,0,0},
      {0,0,0,1},
      {0,1,0,0}};
    malg::Matrix2D<int> mB = 
      {{0,1},
      {2,3},
      {4,5},
      {6,7}};
    malg::Matrix2D<int> mC =  mA * mB;
    assert(mC[0][0] == 4 && mC[0][1] == 5 &&
            mC[1][0] == 0 && mC[1][1] == 1 &&
            mC[2][0] == 6 && mC[2][1] == 7 &&
            mC[3][0] == 2 && mC[3][1] == 3);
    std::cout << "TEST 1 : case 0 : PASS" << std::endl;
  }

  // TEST 1 : case 1 : matrix * matrix with incompatible dimensions 
  {
    try { 
      malg::Matrix2D<int> mA = 
        {{0,0,1,0},
          {1,0,0,0},
          {0,0,0,1},
          {0,1,0,0}};
      malg::Matrix2D<int> mB = 
        {{0,1},
          {2,3},
          {4,5},
          {6,7}};
      // we expect an exception
      malg::Matrix2D<int> mD = mB * mA;
      std::cout << "TEST 1 : case 1 : FAIL" << std::endl;
    } 
    catch(std::exception &e) {
      std::cout << "TEST 1 : case 1 : PASS" << std::endl;
    }
  }

  // TEST 1 : case 2 : scalar * matrix
  {
    int scalar = 2;
    malg::Matrix2D<int> mA = 
      {{0,1},
        {3,4}};
    malg::Matrix2D<int> mB = 2 * mA;
    assert(mB[0][0] == 0 && mB[0][1] == 2 &&
            mB[1][0] == 6 && mB[1][1] == 8);
    std::cout << "TEST 1 : case 2 : PASS" << std::endl;
  }

  std::cout << "TEST 1 : COMPLETE" << std::endl << std::endl;
  std::cout << "TEST 2 : TRANSPOSE" << std::endl;

  // TEST 2 : case 0 : transpose square matrix in place
  {
    malg::Matrix2D<bool> mA = 
      {{1,0,1,0},
       {1,0,0,0},
       {0,0,0,1},
       {0,1,0,0}};
    mA.transpose();
    assert(mA[0][0] == 1 && mA[0][1] == 1 && mA[0][2] == 0 && mA[0][3] == 0 &&
           mA[1][0] == 0 && mA[1][1] == 0 && mA[1][2] == 0 && mA[1][3] == 1 &&
           mA[2][0] == 1 && mA[2][1] == 0 && mA[2][2] == 0 && mA[2][3] == 0 &&
           mA[3][0] == 0 && mA[3][1] == 0 && mA[3][2] == 1 && mA[3][3] == 0);
    std::cout << "TEST 2 : case 0 : PASS" << std::endl;
  }

  std::cout << std::endl << "ALL TESTS COMPLETE" << std::endl;
  std::cout << "See test.cpp for details" << std::endl;
  std::cout << "Antonio Redekop" << std::endl;
  return 0;
}