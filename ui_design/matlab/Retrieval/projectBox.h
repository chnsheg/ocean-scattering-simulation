//
// File: projectBox.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 04-Mar-2025 09:14:41
//

#ifndef PROJECTBOX_H
#define PROJECTBOX_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder {
namespace optim {
namespace coder {
namespace levenbergMarquardt {
double projectBox(const double x[3], double dx[3], const double lb[3],
                  const double ub[3], const boolean_T hasLB[3],
                  const boolean_T hasUB[3]);

}
} // namespace coder
} // namespace optim
} // namespace coder

#endif
//
// File trailer for projectBox.h
//
// [EOF]
//
