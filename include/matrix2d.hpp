#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include <new>
#include <stdexcept>
#include <initializer_list>

namespace malg {

/**
 * template class allows elements of generic built-in type (int, float, etc).
 *
 * this class implements a matrix as an array of row pointers to pointers (2D array).
 * abstractly, we can think of the matrix as a R x C "pool" of values.
 * each pointer in the array points to appropriate position in the pool.
 *
 * unlike vector of vectors or traditional C++ 2D array, this implementation 
 * contiguously allocates memory AND allows dynamically sizing of matrix.
 * we can also access each element using clean [i][j] syntax.
 */
template <typename T> 
class Matrix2D 
{
  public:
    typedef T value_type;

    Matrix2D() : ptr_(nullptr), nrows_(0), ncols_(0) {}
    // initializes data members and calls constructArray() to allocated memory for R X C values
    Matrix2D(unsigned nrows, unsigned ncols, const T& val = T());
    // allows us to use list initialization to create our matrix 
    Matrix2D(std::initializer_list<std::initializer_list<T>> listlist) : 
      Matrix2D((int)listlist.size(), (int)(listlist.begin())->size()) 
    {
      populateArray(listlist);
    }
    ~Matrix2D();

    T** get_ptr() { return ptr_; };
    unsigned get_rows() const { return nrows_; }
    unsigned get_cols() const { return ncols_; }

    // operations    
    void transpose();
    const T* operator[](unsigned row);
    // matrix * matrix
    Matrix2D<T> operator*(const Matrix2D& right) const;
    // matrix * scalar 
    Matrix2D<T> operator*(const T right) const;

  private:
    // member functions
    T** constructArray(unsigned nrows, unsigned ncols, const T& val = T());
    void populateArray(const std::initializer_list<std::initializer_list<T>>&);

    // data members
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
Matrix2D<T>::~Matrix2D() 
{
  if(ptr_) {
    // delete[] deallocates memory and calls destructor for array of objects on the heap 
    // using plain old delete here would result in undefined behavior
    delete[] ptr_[0]; // delete pool 
    delete[] ptr_; // delete array of row pointers
  }
};

template<typename T> 
inline T** Matrix2D<T>::constructArray(unsigned nrows, unsigned ncols, const T& val) 
{
  T** ptr = nullptr;
  T* pool = nullptr;
  try {
    ptr = new T*[nrows];
    // value initialize elements to default value for given template type
    pool = new T[nrows * ncols]{ val };
    for(unsigned i = 0; i < nrows; i++) {
      ptr[i] = pool;
      // point to next row in pool
      pool += ncols;
    }
    return ptr;
  }
  catch (std::bad_alloc& ex) {
    delete[] ptr;
    throw ex;  // throw exception in case of allocation error
  }
}

template<typename T> 
inline void Matrix2D<T>::populateArray(const std::initializer_list<std::initializer_list<T>>& listlist)
{
  for(unsigned i = 0; i < nrows_; i++) {
    for(unsigned j = 0; j < ncols_; j++) {
      // note: pointer arithmetic is awesome
      // this maps each value from our initializer list to our R X C pool of values
      *(*(ptr_+i)+j) = ((listlist.begin()+i)->begin())[j];
    }
  }
  return;
};

template<typename T> 
inline const T* Matrix2D<T>::operator[](unsigned row) 
{
  return ptr_[row];
}

template<typename T> 
inline Matrix2D<T> Matrix2D<T>::operator*(const Matrix2D<T>& right) const 
{
  return *this;
}

template<typename T> 
inline Matrix2D<T> Matrix2D<T>::operator*(const T right) const 
{
  return *this;
}

}; // namespace malg 

#endif // header guard