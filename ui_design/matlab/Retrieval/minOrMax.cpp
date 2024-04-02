//
// File: minOrMax.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "minOrMax.h"
#include "matlab/rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double x[4]
// Return Type  : double
//
namespace coder
{
  namespace internal
  {
    double minimum(const double x[4])
    {
      double ex;
      int idx;
      int k;
      if (!std::isnan(x[0]))
      {
        idx = 1;
      }
      else
      {
        boolean_T exitg1;
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k < 5))
        {
          if (!std::isnan(x[k - 1]))
          {
            idx = k;
            exitg1 = true;
          }
          else
          {
            k++;
          }
        }
      }
      if (idx == 0)
      {
        ex = x[0];
      }
      else
      {
        ex = x[idx - 1];
        idx++;
        for (k = idx; k < 5; k++)
        {
          double d;
          d = x[k - 1];
          if (ex > d)
          {
            ex = d;
          }
        }
      }
      return ex;
    }

  } // namespace internal
} // namespace coder

//
// File trailer for minOrMax.cpp
//
// [EOF]
//
