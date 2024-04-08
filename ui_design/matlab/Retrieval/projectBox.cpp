//
// File: projectBox.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

// Include Files
#include "projectBox.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double x[3]
//                double dx[3]
//                const double lb[3]
//                const double ub[3]
//                const boolean_T hasLB[3]
//                const boolean_T hasUB[3]
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace levenbergMarquardt {
double projectBox(const double x[3], double dx[3], const double lb[3],
                  const double ub[3], const boolean_T hasLB[3],
                  const boolean_T hasUB[3])
{
  double dxInfNorm;
  if (hasLB[0]) {
    dx[0] = std::fmax(lb[0] - x[0], dx[0]);
  }
  if (hasUB[0]) {
    dx[0] = std::fmin(ub[0] - x[0], dx[0]);
  }
  dxInfNorm = std::fmax(0.0, std::abs(dx[0]));
  if (hasLB[1]) {
    dx[1] = std::fmax(lb[1] - x[1], dx[1]);
  }
  if (hasUB[1]) {
    dx[1] = std::fmin(ub[1] - x[1], dx[1]);
  }
  dxInfNorm = std::fmax(dxInfNorm, std::abs(dx[1]));
  if (hasLB[2]) {
    dx[2] = std::fmax(lb[2] - x[2], dx[2]);
  }
  if (hasUB[2]) {
    dx[2] = std::fmin(ub[2] - x[2], dx[2]);
  }
  return std::fmax(dxInfNorm, std::abs(dx[2]));
}

} // namespace levenbergMarquardt
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for projectBox.cpp
//
// [EOF]
//
