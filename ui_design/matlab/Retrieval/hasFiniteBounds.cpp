//
// File: hasFiniteBounds.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 04-Mar-2025 09:14:41
//

// Include Files
#include "hasFiniteBounds.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : boolean_T hasLB[3]
//                boolean_T hasUB[3]
//                const double lb[3]
//                const double ub[3]
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
boolean_T hasFiniteBounds(boolean_T hasLB[3], boolean_T hasUB[3],
                          const double lb[3], const double ub[3])
{
  int idx;
  boolean_T hasBounds;
  hasBounds = false;
  idx = 0;
  while ((!hasBounds) && (idx + 1 <= 3)) {
    hasLB[idx] = ((!std::isinf(lb[idx])) && (!std::isnan(lb[idx])));
    hasUB[idx] = ((!std::isinf(ub[idx])) && (!std::isnan(ub[idx])));
    if (hasLB[idx] || hasUB[idx]) {
      hasBounds = true;
    }
    idx++;
  }
  while (idx + 1 <= 3) {
    hasLB[idx] = ((!std::isinf(lb[idx])) && (!std::isnan(lb[idx])));
    hasUB[idx] = ((!std::isinf(ub[idx])) && (!std::isnan(ub[idx])));
    idx++;
  }
  return hasBounds;
}

} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for hasFiniteBounds.cpp
//
// [EOF]
//
