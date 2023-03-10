#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include <new>
#include <stdexcept>
#include <initializer_list>
#include <vector>
#include <iostream>

namespace malg {

/**
 * class template allows matrix values of generic scalar types (int, float, etc).
 * example: Matrix2D<bool>, Matrix2D<double>
 *
 * we implement matrix as an array of row pointers pointing to a position in array of values.
 * abstractly, we can think of the matrix as a 2D array holding an R x C 'pool' of values.
 *
 * we access a row pointer with ptr->[i], where i is the desired 0-indexed row.
 * we access any value in the pool with ptr->[i][j], where j is the desired 0-indexed col.
 *
 * unlike a 'vector of vectors' or traditional C++ 2D array, this implementation 
 * contiguously allocates memory AND allows dynamic allocation of matrices at runtime.
 * we can also list-initialize matrices and access elements using [i][j] syntax.
 *
 */
template <typename T> 
class Matrix2D 
{
  public:
    Matrix2D() : ptr_(nullptr), nrows_(0), ncols_(0) {}
    // contiguously allocates memory for R x C matrix
    // value-initializes or fills matrix with user-supplied value
    Matrix2D(unsigned nrows, unsigned ncols, const T val = T());
    // instantiates matrix using list initialization
    Matrix2D(std::initializer_list<std::initializer_list<T>> listlist); 
    // copy constructor
    Matrix2D(const Matrix2D<T>& m);
    // move constructor
    Matrix2D(Matrix2D<T>&& m);
    // move assignment
    Matrix2D& operator=(Matrix2D<T>&& m);
    // copy assignment
    Matrix2D<T>& operator=(const Matrix2D<T>& m);
    // destroy all humans
    ~Matrix2D();

    // transpose a square matrix in-place    
    void transpose();
    // index matrix using clean [i][j] syntax
    const T* operator[](unsigned row);
    // matrix + matrix
    const Matrix2D<T> operator+(const Matrix2D<T>& right) const;
    // matrix * matrix
    const Matrix2D<T> operator*(const Matrix2D<T>& right) const;
    // scalar * matrix 
    // friend declaration allows use of built-in types as left-hand operand.
    // operator must be defined here and not outside of class template.
    // see https://isocpp.org/wiki/faq/templates#template-friends
    friend const Matrix2D<T> operator*(const T left, const Matrix2D<T>& right) 
    {
      Matrix2D<T> mC = Matrix2D<T>(right.nrows_, right.ncols_); 
      for(unsigned i=0; i < right.nrows_; i++) {
        for(unsigned j=0;  j < right.ncols_; j++) {
          mC.ptr_[i][j] = left * right.ptr_[i][j]; 
        }
      }
      return mC;
    };

  private:
    // allocates memory contiguously & returns a pointer to first element of row array
    T** constructArray(unsigned nrows, unsigned ncols);
    // populates array with values from initializer list
    void fill(const std::initializer_list<std::initializer_list<T>>&);
    void fill(const T val);
    // pointer to first element of row array
    T** ptr_;
    unsigned nrows_;
    unsigned ncols_;
};

template<typename T>
Matrix2D<T>::Matrix2D(unsigned nrows, unsigned ncols, const T val) 
{
  if(!nrows) {
    throw std::invalid_argument("invalid number of rows \n");
  }
  if(!ncols) {
    throw std::invalid_argument("invalid number of columns \n");
  }
  ptr_ = constructArray(nrows, ncols);
  nrows_ = nrows;
  ncols_ = ncols;
  this->fill(val);
}

template<typename T>
Matrix2D<T>::Matrix2D(std::initializer_list<std::initializer_list<T>> listlist) : 
  Matrix2D((int)listlist.size(), (int)(listlist.begin())->size()) 
{
  fill(listlist);
}

template<typename T>
Matrix2D<T>::Matrix2D(const Matrix2D<T>& m) :
  ptr_{constructArray(m.nrows_, m.ncols_)}, nrows_{m.nrows_}, ncols_{m.ncols_} 
{
  // ptr[0] points to the beginning of our value pool.
  // memory for value pool has already been initialized, 
  // therefore we use std::copy, not std::uninitialized_copy
  std::copy(m.ptr_[0], m.ptr_[0]+(m.nrows_ * m.ncols_), ptr_[0]);
}

template<typename T>
Matrix2D<T>::Matrix2D(Matrix2D<T>&& m) :
  ptr_{m.ptr_}, nrows_{m.nrows_}, ncols_{m.ncols_} 
{
  m.ptr_ = nullptr;
  m.nrows_ = 0;
  m.ncols_ = 0;
  // std::cout << "move" << std::endl;
}

template<typename T>
Matrix2D<T>& Matrix2D<T>::operator=(Matrix2D<T>&& m) 
{
  std::swap(ptr_, m.ptr_);
  std::swap(nrows_, m.nrows_);
  std::swap(ncols_, m.ncols_);
  // std::cout << "move assignment" << std::endl;
  return *this;
}

template<typename T>
Matrix2D<T>& Matrix2D<T>::operator=(const Matrix2D<T>& m)
{
  if(nrows_ != m.nrows_ || ncols_ != m.ncols_) {
    throw std::runtime_error("incompatible sizes in Matrix2D =");
  }
  std::copy(m.ptr_[0], m.ptr_[0]+(m.nrows_ * m.ncols_), ptr_[0]);
  return *this;
}

template<typename T>
Matrix2D<T>::~Matrix2D() 
{
  if(ptr_) {
    if(ptr_[0]) {
      // delete pool of values
      delete[] ptr_[0];   
      ptr_[0] = nullptr;  
    }
    // delete array of row pointers
    delete[] ptr_; 
    ptr_ = nullptr;
  }
};

template<typename T> 
inline T** Matrix2D<T>::constructArray(unsigned nrows, unsigned ncols) 
{
  T** ptr = nullptr;
  T* pool = nullptr;
  try {
    ptr = new T*[nrows];
    // value initializes pool elements with default value for T
    // pool = new T[nrows * ncols]{ val }; 
    pool = new T[nrows * ncols]; 
    for(unsigned i = 0; i < nrows; i++) {
      ptr[i] = pool;
      // point to next row
      pool += ncols; 
    }
    return ptr;
  }
  catch (std::bad_alloc& ex) {
    // note: we must delete ptr[0] here, not pool
    // pool is not guaranteed to point to beginning of array.
    if(ptr) {
      if(ptr[0]) {   
        delete[] ptr[0];   
        ptr[0] = nullptr;  
      }
      delete[] ptr;        
      ptr = nullptr;
    }
    throw ex;
  }
}

template<typename T> 
inline void Matrix2D<T>::fill(const std::initializer_list<std::initializer_list<T>>& listlist)
{
  // maps each value from our initializer list to our pool 
  for(unsigned i = 0; i < nrows_; i++) {
    for(unsigned j = 0; j < ncols_; j++) {
      ptr_[i][j] = ((listlist.begin()+i)->begin())[j];
    }
  }
  return;
};

template<typename T> 
inline void Matrix2D<T>::fill(const T val) {
  for(unsigned i = 0; i < nrows_; i++) {
    for(unsigned j = 0; j < ncols_; j++) {
      ptr_[i][j] = val;
    }
  }
  return;
};

template<typename T> 
inline void Matrix2D<T>::transpose() 
{
  // if square matrix
  if(nrows_ == ncols_) {
    for(unsigned i=0; i < nrows_; i++) {
      for(unsigned j=i+1; j < nrows_; j++) {
        std::swap(ptr_[i][j], ptr_[j][i]);
      }
    }
    return;
  }
  // if non-square matrix
  // the below algorithm changes the value pool in-place
  // note: ptr_[0] points to the beginning of the value pool
  const int nm1 = nrows_ * ncols_ - 1;
  T* first = ptr_[0];
  T* last = ptr_[0] + nm1;
  std::vector<bool> visited(last-first);
  T* cycle = first;
  while(++cycle != last) {
    if(visited[cycle - first]) 
      continue;
    int a = cycle - first;
    do {
      // formula finds permutation cycles
      a = a == nm1 ? nm1  : (nrows_ * a) % nm1;
      std::swap(*(first + a), *cycle);
      visited[a] = true;
    } while((first + a) != cycle);
  }
  // swap the number of rows and columns
  unsigned temp = nrows_;
  nrows_ = ncols_;
  ncols_ = temp;
  // our value pool was changed in-place and remains in row-major ordering
  T* pool = ptr_[0];
  // delete existing array of row pointers 
  delete ptr_;
  ptr_ = nullptr;
  // create a new array of row pointers for transposed pool 
  T** p = new T*[nrows_];
  for(unsigned i = 0; i < nrows_; i++) {
    p[i] = pool;
    // point to next row
    pool += ncols_; 
  }
  // point ptr_ to new array of row pointers
  ptr_ = p;

};

template<typename T> 
inline const T* Matrix2D<T>::operator[](unsigned row) 
{
  if(row >= this->nrows_) {
    throw std::range_error("out of range row index\n");;
  }
  return ptr_[row];
}

template<typename T> 
inline const Matrix2D<T> Matrix2D<T>::operator+(const Matrix2D<T>& right) const 
{
  // note: syntax in form of ptr_[i][j] is equivalent to *(*(ptr_+i)+j)
  malg::Matrix2D<T> mC = Matrix2D<T>(this->nrows_, this->ncols_); 
  for(unsigned i=0; i < this->nrows_; i++) {
    for(unsigned j=0;  j < this->ncols_; j++) {
      mC.ptr_[i][j] = this->ptr_[i][j] + right.ptr_[i][j]; 
    }
  }
  return mC;
}

template<typename T> 
inline const Matrix2D<T> Matrix2D<T>::operator*(const Matrix2D<T>& right) const 
{
  if(this->ncols_ != right.nrows_) {
    throw std::range_error("incompatible matrix dimensions \n");;
  }
  // note: syntax in form of ptr_[i][j] is equivalent to *(*(ptr_+i)+j)
  malg::Matrix2D<T> mC = malg::Matrix2D<T>(this->nrows_, right.ncols_);
  for(unsigned i=0; i < this->nrows_; i++) {
    for(unsigned j=0;  j < right.ncols_; j++) {
      mC.ptr_[i][j] = 0;
      for(unsigned k=0; k < this->ncols_; k++) {
        mC.ptr_[i][j] = mC.ptr_[i][j] + this->ptr_[i][k] * right.ptr_[k][j];
      }
    }
  } 
  return mC;
}

}; // namespace malg 

#endif // header guard