//
// File: checkStoppingCriteria.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef CHECKSTOPPINGCRITERIA_H
#define CHECKSTOPPINGCRITERIA_H

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
                int b_checkStoppingCriteria(const double gradf[4], double relFactor,
                                            double funDiff, const double x[4],
                                            const double dx[4], int funcCount,
                                            boolean_T stepSuccessful, int *iter,
                                            double projSteepestDescentInfNorm,
                                            boolean_T hasFiniteBounds);

                int checkStoppingCriteria(const double gradf[4], double relFactor,
                                          int funcCount, double projSteepestDescentInfNorm,
                                          boolean_T hasFiniteBounds);

            } // namespace levenbergMarquardt
        }     // namespace coder
    }         // namespace optim
} // namespace coder

#endif
//
// File trailer for checkStoppingCriteria.h
//
// [EOF]
//
