#include "matrix2d.hpp"
#include <iostream>
#include <cassert>

int main() 
{
  { // TEST 1 - value initialized matrices of size R x C 
    malg::Matrix2D<int> mA(100, 50);
    assert(mA[0][0] == 0 && mA[99][49] == 0);
    std::cout << "TEST 1 : PASSED" << std::endl;
  }
  { // TEST 2 - list initialized matrices 
    malg::Matrix2D<float> mA = {{1.0, 3.2, 6.0},
                                {4.2, 6.0, 9.9}};
    assert(mA[0][1] == (float)3.2 && mA[1][1] == (float)6.0);
    std::cout << "TEST 2 : PASSED" << std::endl;
  }
  { // TEST 3 - matrix multiplication (matrix * matrix)
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
    //  4 5
    //  0 1
    //  6 7
    //  2 3
    assert(mC[0][0] == 4 && mC[0][1] == 5 &&
           mC[1][0] == 0 && mC[1][1] == 1 &&
           mC[2][0] == 6 && mC[2][1] == 7 &&
           mC[3][0] == 2 && mC[3][1] == 3);
    // now test with incompatible dimensions
    try {
      malg::Matrix2D<int> mD = mB * mA;
    }
    catch(std::exception &e) {
      // we expect an exception
      std::cout << "TEST 3 : PASSED" << std::endl;
    }
  }
  { // TEST 4 - matrix multiplication (scalar * matrix)
    int scalar = 2;
    malg::Matrix2D<int> mA = 
      {{0,1},
       {3,4}};
    malg::Matrix2D<int> mB = 2 * mA;
    assert(mB[0][0] == 0 && mB[0][1] == 2 &&
           mB[1][0] == 6 && mB[1][1] == 8);
    std::cout << "TEST 4 : PASSED" << std::endl;
  }
  { // TEST 5 - transpose square matrix in place
    malg::Matrix2D<bool> mA = 
      {{1,0,1,0},
       {1,0,0,0},
       {0,0,0,1},
       {0,1,0,0}};
    //  1 1 0 0 
    //  0 0 0 1
    //  1 0 0 0
    //  0 0 1 0 
    mA.transpose();
    assert(mA[0][0] == 1 && mA[0][1] == 1 && mA[0][2] == 0 && mA[0][3] == 0 &&
           mA[1][0] == 0 && mA[1][1] == 0 && mA[1][2] == 0 && mA[1][3] == 1 &&
           mA[2][0] == 1 && mA[2][1] == 0 && mA[2][2] == 0 && mA[2][3] == 0 &&
           mA[3][0] == 0 && mA[3][1] == 0 && mA[3][2] == 1 && mA[3][3] == 0);
    std::cout << "TEST 5 : PASSED" << std::endl;
  }

  std::cout << "ALL TESTS COMPLETE" << std::endl;
  std::cout << "See test.cpp for details" << std::endl;
  std::cout << "Antonio Redekop" << std::endl;
  return 0;
}