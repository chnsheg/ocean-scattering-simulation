//
// File: xgemv.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef XGEMV_H
#define XGEMV_H

// Include Files
#include "matlab/rtwtypes.h"
#include "matlab/coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
    namespace internal
    {
        namespace blas
        {
            void xgemv(int m, const ::coder::array<double, 2U> &A, int lda,
                       const ::coder::array<double, 2U> &x, double y[4]);

        }
    } // namespace internal
} // namespace coder

#endif
//
// File trailer for xgemv.h
//
// [EOF]
//
