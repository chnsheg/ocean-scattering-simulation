//
// File: factoryConstruct.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef FACTORYCONSTRUCT_H
#define FACTORYCONSTRUCT_H

// Include Files
#include "matlab/rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace coder
{
    class b_anonymous_function;

}
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
                    void factoryConstruct(const b_anonymous_function *nonlin, int mCeq,
                                          const double lb[4], const double ub[4], c_struct_T *obj);

                }
            } // namespace utils
        }     // namespace coder
    }         // namespace optim
} // namespace coder

#endif
//
// File trailer for factoryConstruct.h
//
// [EOF]
//