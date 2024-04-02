//
// File: driver.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef DRIVER_H
#define DRIVER_H

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
            namespace levenbergMarquardt
            {
                double computeFirstOrderOpt(const double gradf[4], boolean_T hasFiniteBounds,
                                            const double *projSteepestDescentInfNorm);

            }
        } // namespace coder
    }     // namespace optim
} // namespace coder

#endif
//
// File trailer for driver.h
//
// [EOF]
//
