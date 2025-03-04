//
// File: driver.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 04-Mar-2025 09:14:41
//

// Include Files
#include "driver.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double gradf[3]
//                boolean_T hasFiniteBounds
//                const double *projSteepestDescentInfNorm
// Return Type  : double
//
namespace coder {
namespace optim {
namespace coder {
namespace levenbergMarquardt {
double computeFirstOrderOpt(const double gradf[3], boolean_T hasFiniteBounds,
                            const double *projSteepestDescentInfNorm)
{
  double firstOrderOpt;
  if (hasFiniteBounds) {
    double absx;
    double b;
    b = 0.0;
    absx = std::abs(gradf[0]);
    if (std::isnan(absx) || (absx > 0.0)) {
      b = absx;
    }
    absx = std::abs(gradf[1]);
    if (std::isnan(absx) || (absx > b)) {
      b = absx;
    }
    absx = std::abs(gradf[2]);
    if (std::isnan(absx) || (absx > b)) {
      b = absx;
    }
    if ((std::abs(*projSteepestDescentInfNorm - b) <
         2.2204460492503131E-16 * std::fmax(*projSteepestDescentInfNorm, b)) ||
        (b == 0.0)) {
      firstOrderOpt = *projSteepestDescentInfNorm;
    } else {
      firstOrderOpt =
          *projSteepestDescentInfNorm * *projSteepestDescentInfNorm / b;
    }
  } else {
    double absx;
    firstOrderOpt = 0.0;
    absx = std::abs(gradf[0]);
    if (std::isnan(absx) || (absx > 0.0)) {
      firstOrderOpt = absx;
    }
    absx = std::abs(gradf[1]);
    if (std::isnan(absx) || (absx > firstOrderOpt)) {
      firstOrderOpt = absx;
    }
    absx = std::abs(gradf[2]);
    if (std::isnan(absx) || (absx > firstOrderOpt)) {
      firstOrderOpt = absx;
    }
  }
  return firstOrderOpt;
}

} // namespace levenbergMarquardt
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for driver.cpp
//
// [EOF]
//
