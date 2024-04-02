//
// File: hasFiniteBounds.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef HASFINITEBOUNDS_H
#define HASFINITEBOUNDS_H

// Include Files
#include "matlab/rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
    namespace optim
    {
        namespace coder
        {
            namespace utils
            {
                boolean_T hasFiniteBounds(boolean_T hasLB[4], boolean_T hasUB[4],
                                          const double lb[4], const double ub[4]);

            }
        } // namespace coder
    }     // namespace optim
} // namespace coder

#endif
//
// File trailer for hasFiniteBounds.h
//
// [EOF]
//
