//
// File: projectBox.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "projectBox.h"
#include "matlab/rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double x[4]
//                double dx[4]
//                const double lb[4]
//                const double ub[4]
//                const boolean_T hasLB[4]
//                const boolean_T hasUB[4]
// Return Type  : double
//
namespace coder
{
  namespace optim
  {
    namespace coder
    {
      namespace levenbergMarquardt
      {
        double projectBox(const double x[4], double dx[4], const double lb[4],
                          const double ub[4], const boolean_T hasLB[4],
                          const boolean_T hasUB[4])
        {
          double dxInfNorm;
          if (hasLB[0])
          {
            dx[0] = std::fmax(lb[0] - x[0], dx[0]);
          }
          if (hasUB[0])
          {
            dx[0] = std::fmin(ub[0] - x[0], dx[0]);
          }
          dxInfNorm = std::fmax(0.0, std::abs(dx[0]));
          if (hasLB[1])
          {
            dx[1] = std::fmax(lb[1] - x[1], dx[1]);
          }
          if (hasUB[1])
          {
            dx[1] = std::fmin(ub[1] - x[1], dx[1]);
          }
          dxInfNorm = std::fmax(dxInfNorm, std::abs(dx[1]));
          if (hasLB[2])
          {
            dx[2] = std::fmax(lb[2] - x[2], dx[2]);
          }
          if (hasUB[2])
          {
            dx[2] = std::fmin(ub[2] - x[2], dx[2]);
          }
          dxInfNorm = std::fmax(dxInfNorm, std::abs(dx[2]));
          if (hasLB[3])
          {
            dx[3] = std::fmax(lb[3] - x[3], dx[3]);
          }
          if (hasUB[3])
          {
            dx[3] = std::fmin(ub[3] - x[3], dx[3]);
          }
          return std::fmax(dxInfNorm, std::abs(dx[3]));
        }

      } // namespace levenbergMarquardt
    }   // namespace coder
  }     // namespace optim
} // namespace coder

//
// File trailer for projectBox.cpp
//
// [EOF]
//
