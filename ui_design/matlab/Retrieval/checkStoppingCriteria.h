//
// File: checkStoppingCriteria.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

#ifndef CHECKSTOPPINGCRITERIA_H
#define CHECKSTOPPINGCRITERIA_H

// Include Files
#include "rtwtypes.h"
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
                int b_checkStoppingCriteria(const double gradf[3], double relFactor,
                                            double funDiff, const double x[3],
                                            const double dx[3], int funcCount,
                                            boolean_T stepSuccessful, int *iter,
                                            double projSteepestDescentInfNorm,
                                            boolean_T hasFiniteBounds, double tolerance);

                int checkStoppingCriteria(const double gradf[3], double relFactor,
                                          int funcCount, double projSteepestDescentInfNorm,
                                          boolean_T hasFiniteBounds, double tolerance);

            } // namespace levenbergMarquardt
        } // namespace coder
    } // namespace optim
} // namespace coder

#endif
//
// File trailer for checkStoppingCriteria.h
//
// [EOF]
//
