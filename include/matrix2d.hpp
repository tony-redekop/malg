#ifndef MATRIX2D_HPP
#define MATRIX2D_HPP

#include <new>
#include <stdexcept>

namespace malg {

// template class allows elements of generic built-in type (i.e. int, float, double, etc).
// implements a matrix as an array of row pointers to pointers (2D array).
// abstractly, we can think of the matrix as a R x C "pool" of values.
// each pointer in the array points to appropriate position in the pool.
// this implementation contigiously allocates matrix AND allows it to be dynamically sized.
// we can also access each element using clean [i][j] syntax.
template <typename T> class Matrix2D {
  public:
    typedef T value_type;

    T** constructArray(unsigned nrows, unsigned ncols, const T& val = T()) {
      T** ptr = nullptr;
      T* pool = nullptr;
      try {
        ptr = new T*[nrows];
        // value initialize elements to default value for given template type
        pool = new T[nrows * ncols]{ val };
        for(unsigned i = 0; i < nrows; i++) {
          ptr[i] = pool;
          // use pointer arithmetic to point to next row in pool
          pool += ncols;
        }
        return ptr;
      }
      catch (std::bad_alloc& ex) {
        delete[] ptr;
        throw ex;  // throw exception in case of allocation error
      }
    }

    // gets first element (row pointer pointing to column pointer) in our array
    T** get_ptr();

    unsigned get_rows() const {
      return nrows_;
    };

    unsigned get_cols() const {
      return ncols_;
    };

    Matrix2D() : ptr_(nullptr), nrows_(0), ncols_(0) {}
    Matrix2D(unsigned nrows, unsigned ncols, const T& val = T()) {
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

    ~Matrix2D() {
      if(ptr_) {
        // delete[] deallocates memory and calls destructor for array of objects created with new[]
        // using plain old delete here results in undefined behavior
        delete[] ptr_[0]; // remove array of column pointers
        delete[] ptr_; // remove array of row pointers
      }
    };

    void transpose();

    // matrix * matrix
    Matrix2D operator*(const Matrix2D& right) const {
      return *this;
    }

    // matrix * scalar 
    Matrix2D operator*(const double right) const {
      return *this;
    }

  private:
    T** ptr_;
    unsigned nrows_;
    unsigned ncols_;
};

}; // namespace malg 

#endif // header guard