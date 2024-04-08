//
// File: xzlarf.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

#ifndef XZLARF_H
#define XZLARF_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace internal {
namespace reflapack {
void xzlarf(int m, int n, int iv0, double tau, ::coder::array<double, 2U> &C,
            int ic0, int ldc, double work[3]);

}
} // namespace internal
} // namespace coder

#endif
//
// File trailer for xzlarf.h
//
// [EOF]
//
