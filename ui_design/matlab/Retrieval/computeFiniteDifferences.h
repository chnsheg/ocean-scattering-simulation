//
// File: computeFiniteDifferences.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 04-Mar-2025 09:14:41
//

#ifndef COMPUTEFINITEDIFFERENCES_H
#define COMPUTEFINITEDIFFERENCES_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct c_struct_T;

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace FiniteDifferences {
boolean_T computeFiniteDifferences(c_struct_T *obj,
                                   const ::coder::array<double, 2U> &cEqCurrent,
                                   double xk[3],
                                   ::coder::array<double, 2U> &JacCeqTrans,
                                   const double lb[3], const double ub[3]);

}
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for computeFiniteDifferences.h
//
// [EOF]
//
