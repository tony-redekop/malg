#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include <new>
#include <stdexcept>
#include <initializer_list>

namespace malg {

/**
 * template class allows elements of generic built-in type (int, float, etc).
 *
 * implements a matrix as an array of row pointers pointing to a position in an array.
 * abstractly, we can think of the matrix as a 2D array or an R x C 'pool' of values.
 *
 * unlike a 'vector of vectors' or traditional C++ 2D array, this implementation 
 * contiguously allocates memory AND allows dynamic sizing of matrix at runtime.
 * we can also list-initialize matrices and access elements using [i][j] syntax.
 *
 */
template <typename T> 
class Matrix2D 
{
  public:
    Matrix2D() : ptr_(nullptr), nrows_(0), ncols_(0) {}
    // calls constructArray() to contiguously allocated memory for matrix
    Matrix2D(unsigned nrows, unsigned ncols, const T& val = T());
    // uses list initialization to instantiate and populate our matrix
    Matrix2D(std::initializer_list<std::initializer_list<T>> listlist); 
    // destroy all humans 
    ~Matrix2D();

    // transpose a square matrix in-place    
    void transpose();
    // index matrix using clean [i][j] syntax
    const T* operator[](unsigned row);
    // matrix + matrix
    const Matrix2D<T>& operator+(const Matrix2D<T>& right) const;
    // matrix * matrix
    const Matrix2D<T>& operator*(const Matrix2D<T>& right) const;
    // matrix * scalar 
    Matrix2D<T> operator*(const T right) const;

  private:
    // allocates memory contiguously & returns a pointer to first element of row array
    T** constructArray(unsigned nrows, unsigned ncols, const T& val = T());
    // populates array with values from initializer list
    void populateArray(const std::initializer_list<std::initializer_list<T>>&);
    // pointer to first element of row array
    T** ptr_;
    unsigned nrows_;
    unsigned ncols_;
};

template<typename T>
Matrix2D<T>::Matrix2D(unsigned nrows, unsigned ncols, const T& val) 
{
  if(!nrows) {
    throw std::invalid_argument("number of rows is 0");
  }
  if(!ncols) {
    throw std::invalid_argument("number of cols is 0");
  }
  ptr_ = constructArray(nrows, ncols, val);
  nrows_ = nrows;
  ncols_ = ncols;
}

template<typename T>
Matrix2D<T>::Matrix2D(std::initializer_list<std::initializer_list<T>> listlist) : 
  Matrix2D((int)listlist.size(), (int)(listlist.begin())->size()) 
{
  populateArray(listlist);
}

template<typename T>
Matrix2D<T>::~Matrix2D() 
{
  if(ptr_) {
    // must use delete[], using plain old delete here would result in undefined behavior
    delete[] ptr_[0]; // delete pool 
    delete[] ptr_;    // delete array of row pointers
  }
};

template<typename T> 
inline T** Matrix2D<T>::constructArray(unsigned nrows, unsigned ncols, const T& val) 
{
  T** ptr = nullptr;
  T* pool = nullptr;
  try {
    ptr = new T*[nrows];
    pool = new T[nrows * ncols]{ val };  // value initialize elements to default val for T
    for(unsigned i = 0; i < nrows; i++) {
      ptr[i] = pool;
      pool += ncols;  // point to next row in pool
    }
    return ptr;
  }
  // delete ptr and throw exception in case of allocation error
  catch (std::bad_alloc& ex) {
    delete[] ptr;
    throw ex;
  }
}

template<typename T> 
inline void Matrix2D<T>::populateArray(const std::initializer_list<std::initializer_list<T>>& listlist)
{
  for(unsigned i = 0; i < nrows_; i++) {
    for(unsigned j = 0; j < ncols_; j++) {
      // maps each value from our initializer list to our pool 
      // note: syntax in form of ptr_[i][j] is equivalent to *(*(ptr_+i)+j)
      ptr_[i][j] = ((listlist.begin()+i)->begin())[j];
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
inline const Matrix2D<T>& Matrix2D<T>::operator+(const Matrix2D<T>& right) const 
{
  // 'this' pointer is bound to left-hand operand 
  // note: syntax in form of ptr_[i][j] is equivalent to *(*(ptr_+i)+j)
  malg::Matrix2D<T>* mC = new malg::Matrix2D<T>(this->nrows_, this->ncols_);
  for(unsigned i=0; i < this->nrows_; i++) {
    for(unsigned j=0;  j < this->ncols_; j++) {
      mC->ptr_[i][j] = this->ptr_[i][j] + right.ptr_[i][j]; 
    }
  }
  return *mC;
}

template<typename T> 
inline const Matrix2D<T>& Matrix2D<T>::operator*(const Matrix2D<T>& right) const 
{
  // 'this' pointer is bound to left-hand operand 
  // note: syntax in form of ptr_[i][j] is equivalent to *(*(ptr_+i)+j)
  malg::Matrix2D<T>* mC = new malg::Matrix2D<T>(this->nrows_, right.ncols_);
  for(unsigned i=0; i < this->nrows_; i++) {
    for(unsigned j=0;  j < right.ncols_; j++) {
      mC->ptr_[i][j] = 0;
      for(unsigned k=0; k < this->ncols_; k++) {
        mC->ptr_[i][j] = mC->ptr_[i][j] + this->ptr_[i][k] * right.ptr_[k][j];
      }
    }
  } 
  return *mC;
}

template<typename T> 
inline Matrix2D<T> Matrix2D<T>::operator*(const T right) const 
{
  return *this;
}

}; // namespace malg 

#endif // header guard