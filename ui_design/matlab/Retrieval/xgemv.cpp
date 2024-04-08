//
// File: xgemv.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

// Include Files
#include "xgemv.h"
#include "rt_nonfinite.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : int m
//                const ::coder::array<double, 2U> &A
//                int lda
//                const ::coder::array<double, 2U> &x
//                double y[3]
// Return Type  : void
//
namespace coder {
namespace internal {
namespace blas {
void xgemv(int m, const ::coder::array<double, 2U> &A, int lda,
           const ::coder::array<double, 2U> &x, double y[3])
{
  if (m != 0) {
    int i;
    int iy;
    y[0] = 0.0;
    y[1] = 0.0;
    y[2] = 0.0;
    iy = 0;
    i = (lda << 1) + 1;
    for (int iac{1}; lda < 0 ? iac >= i : iac <= i; iac += lda) {
      double c;
      int i1;
      c = 0.0;
      i1 = (iac + m) - 1;
      for (int ia{iac}; ia <= i1; ia++) {
        c += A[ia - 1] * x[ia - iac];
      }
      y[iy] += c;
      iy++;
    }
  }
}

} // namespace blas
} // namespace internal
} // namespace coder

//
// File trailer for xgemv.cpp
//
// [EOF]
//
