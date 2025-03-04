//
// File: linearLeastSquares.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 04-Mar-2025 09:14:41
//

#ifndef LINEARLEASTSQUARES_H
#define LINEARLEASTSQUARES_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace levenbergMarquardt {
void linearLeastSquares(::coder::array<double, 2U> &lhs,
                        ::coder::array<double, 1U> &rhs, double dx[3], int m);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for linearLeastSquares.h
//
// [EOF]
//
