//
// File: computeFiniteDifferences.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef COMPUTEFINITEDIFFERENCES_H
#define COMPUTEFINITEDIFFERENCES_H

// Include Files
#include "matlab/rtwtypes.h"
#include "matlab/coder_array.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct c_struct_T;

// Function Declarations
namespace coder
{
    namespace optim
    {
        namespace coder
        {
            namespace utils
            {
                namespace FiniteDifferences
                {
                    boolean_T computeFiniteDifferences(c_struct_T *obj,
                                                       const ::coder::array<double, 2U> &cEqCurrent,
                                                       double xk[4],
                                                       ::coder::array<double, 2U> &JacCeqTrans,
                                                       const double lb[4], const double ub[4]);

                }
            } // namespace utils
        }     // namespace coder
    }         // namespace optim
} // namespace coder

#endif
//
// File trailer for computeFiniteDifferences.h
//
// [EOF]
//
