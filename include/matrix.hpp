#ifndef MATRIX_HPP
#define MATRIX_HPP

namespace malg {

class Matrix {
  public:
    Matrix();
    ~Matrix();

    void transpose();

    // matrix * matrix 
    Matrix operator*(const Matrix& right) const;
    // matrix * scalar
    Matrix operator*(const double right) const;

  private:
};

inline Matrix Matrix::operator*(const Matrix& right) const {
  return *this;
}

inline Matrix Matrix::operator*(const double right) const {
  return *this;
}

}; // namespace malg 

#endif // header guard