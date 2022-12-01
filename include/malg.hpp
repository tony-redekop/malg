#ifndef MALG_HPP
#define MALG_HPP

#include "malg/matrix.hpp"

namespace malg {

class Malg {
  public:
    Malg();
    ~Malg();
    Matrix multiply(Matrix& m1, Matrix& m2, int s);
    Matrix transpose(Matrix& m);

  private:
};

}; // namespace malg 

#endif // header guard