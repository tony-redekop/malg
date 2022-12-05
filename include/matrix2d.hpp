#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include <new>
#include <stdexcept>
#include <initializer_list>

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
  for(unsigned n=0; n <= (this->nrows_ - 2); n++) {
    for(unsigned m=n+1; m <= (this->nrows_ - 1); m++) {
      auto temp = this->ptr_[n][m];
      this->ptr_[n][m] = this->ptr_[m][n];
      this->ptr_[m][n] = temp;
    }
  }
};

template<typename T> 
inline const T* Matrix2D<T>::operator[](unsigned row) 
{
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