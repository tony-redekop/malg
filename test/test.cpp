#include "matrix2d.hpp"
#include <iostream>
#include <cassert>

malg::Matrix2D<int> test_move() {
  malg::Matrix2D<int> m(1000, 1000, 666);
  // note: testing move semantics in this case requires compiler
  // flag -fno-elide-constructors to disable copy ellision
  // see top-most CMakeLists.txt for further notes 
  return m;
}

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
  // TEST 2 : case 1 : transpose non-square matrix in place
  {
    malg::Matrix2D<int> mA = 
      {{11,12,13,14},
       {21,22,23,24}};
    mA.transpose(); 
    assert(mA[0][0] == 11 && mA[0][1] == 21 &&
           mA[1][0] == 12 && mA[1][1] == 22 &&
           mA[2][0] == 13 && mA[2][1] == 23 &&
           mA[3][0] == 14 && mA[3][1] == 24);
    std::cout << "TEST 2 : case 1 : PASS" << std::endl;
  }
  std::cout << "TEST 2 : COMPLETE\n" << std::endl;
  // TEST 3 : case 0 : deep-copy a matrix 
  {
    malg::Matrix2D<double>* mA = nullptr;
    try {
      mA = new malg::Matrix2D<double>{  
        {666.0, 187.3, 19.2},
        {123.4, 777.0, 3.14}};
      // our copy-constructor implements deep-copy
      malg::Matrix2D<double> mB{*mA};
      // we delete mA to prove that mB is a deep copy
      delete mA;
      assert(mB[0][0] == 666.0 && mB[0][1] == 187.3 && mB[0][2] == 19.2 &&
             mB[1][0] == 123.4 && mB[1][1] == 777.0 && mB[1][2] == 3.14);
    }
    catch(std::bad_alloc& e) {
      if(mA) {
        delete mA;
      }
      std::cout << "failed to allocate memory for TEST 3 : case 0" << std::endl;
    }
    std::cout << "TEST 3 : case 0 : PASS" << std::endl;
  }
  // TEST 3 : case 1 : copy assignment of matrix
  {
    malg::Matrix2D<unsigned>* mA = nullptr;
    try {
      mA = new malg::Matrix2D<unsigned>{
        {10, 20, 69},
        {40, 50, 60}};
      malg::Matrix2D<unsigned> mB = 
        {{1, 2, 3},
         {4, 5, 6}};
      mB = *mA;
      // we delete mA to prove that mB is a deep copy by assignment
      delete mA;
      assert(mB[0][0] == 10 && mB[0][1] == 20 && mB[0][2] == 69 &&
            mB[1][0] == 40 && mB[1][1] == 50 && mB[1][2] == 60);
    }
    catch(std::bad_alloc& e) {
      if(mA) {
        delete mA;
      }
      std::cout << "failed to allocate memory for TEST 3 : case 1" << std::endl;
    }
    std::cout << "TEST 3 : case 1 : PASS" << std::endl;
  }
  // TEST 3 : case 2 : test move semantics 
  {
    // to test move constructor, uncomment cout statment in move constructor
    malg::Matrix2D<int> mA{test_move()};
    assert(mA[100][100] == 666);

    // to test move assignment, uncomment cout statement in move assignment operator
    malg::Matrix2D<int> mB = {{50588, 2200, 360}};
    assert(mB[0][0] == 50588 && mB[0][1] == 2200 && mB[0][2] == 360);
    malg::Matrix2D<int> temp = std::move(mA);
    mA = std::move(mB);
    mB = std::move(temp);
    assert(mA[0][0] == 50588 && mA[0][1] == 2200 && mA[0][2] == 360);
    std::cout << "TEST 3 : case 2 : PASS" << std::endl;
  }
  std::cout << "TEST 3 : COMPLETE" << std::endl;
  std::cout << std::endl << "ALL TESTS COMPLETE" << std::endl;
  std::cout << "See test.cpp for details" << std::endl;
  std::cout << "Antonio Redekop" << std::endl;
  return 0;
}