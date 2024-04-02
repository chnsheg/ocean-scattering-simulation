//
// File: xzlarf.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef XZLARF_H
#define XZLARF_H

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
        namespace reflapack
        {
            void xzlarf(int m, int n, int iv0, double tau, ::coder::array<double, 2U> &C,
                        int ic0, int ldc, double work[4]);

        }
    } // namespace internal
} // namespace coder

#endif
//
// File trailer for xzlarf.h
//
// [EOF]
//
