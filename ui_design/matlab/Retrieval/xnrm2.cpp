//
// File: xnrm2.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "xnrm2.h"
#include "matlab/rt_nonfinite.h"
#include "matlab/coder_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : int n
//                const ::coder::array<double, 2U> &x
//                int ix0
// Return Type  : double
//
namespace coder
{
  namespace internal
  {
    namespace blas
    {
      double xnrm2(int n, const ::coder::array<double, 2U> &x, int ix0)
      {
        double y;
        y = 0.0;
        if (n >= 1)
        {
          if (n == 1)
          {
            y = std::abs(x[ix0 - 1]);
          }
          else
          {
            double scale;
            int kend;
            scale = 3.3121686421112381E-170;
            kend = (ix0 + n) - 1;
            for (int k{ix0}; k <= kend; k++)
            {
              double absxk;
              absxk = std::abs(x[k - 1]);
              if (absxk > scale)
              {
                double t;
                t = scale / absxk;
                y = y * t * t + 1.0;
                scale = absxk;
              }
              else
              {
                double t;
                t = absxk / scale;
                y += t * t;
              }
            }
            y = scale * std::sqrt(y);
          }
        }
        return y;
      }

    } // namespace blas
  }   // namespace internal
} // namespace coder

//
// File trailer for xnrm2.cpp
//
// [EOF]
//
