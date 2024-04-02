//
// File: projectBox.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef PROJECTBOX_H
#define PROJECTBOX_H

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
                double projectBox(const double x[4], double dx[4], const double lb[4],
                                  const double ub[4], const boolean_T hasLB[4],
                                  const boolean_T hasUB[4]);

            }
        } // namespace coder
    }     // namespace optim
} // namespace coder

#endif
//
// File trailer for projectBox.h
//
// [EOF]
//
