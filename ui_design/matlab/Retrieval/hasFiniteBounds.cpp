//
// File: hasFiniteBounds.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "hasFiniteBounds.h"
#include "matlab/rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : boolean_T hasLB[4]
//                boolean_T hasUB[4]
//                const double lb[4]
//                const double ub[4]
// Return Type  : boolean_T
//
namespace coder
{
  namespace optim
  {
    namespace coder
    {
      namespace utils
      {
        boolean_T hasFiniteBounds(boolean_T hasLB[4], boolean_T hasUB[4],
                                  const double lb[4], const double ub[4])
        {
          int idx;
          boolean_T hasBounds;
          hasBounds = false;
          idx = 0;
          while ((!hasBounds) && (idx + 1 <= 4))
          {
            hasLB[idx] = ((!std::isinf(lb[idx])) && (!std::isnan(lb[idx])));
            hasUB[idx] = ((!std::isinf(ub[idx])) && (!std::isnan(ub[idx])));
            if (hasLB[idx] || hasUB[idx])
            {
              hasBounds = true;
            }
            idx++;
          }
          while (idx + 1 <= 4)
          {
            hasLB[idx] = ((!std::isinf(lb[idx])) && (!std::isnan(lb[idx])));
            hasUB[idx] = ((!std::isinf(ub[idx])) && (!std::isnan(ub[idx])));
            idx++;
          }
          return hasBounds;
        }

      } // namespace utils
    }   // namespace coder
  }     // namespace optim
} // namespace coder

//
// File trailer for hasFiniteBounds.cpp
//
// [EOF]
//
