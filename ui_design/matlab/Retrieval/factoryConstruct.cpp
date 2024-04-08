//
// File: factoryConstruct.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

// Include Files
#include "factoryConstruct.h"
#include "RetrievalAlgorithm_internal_types.h"
#include "anonymous_function.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const b_anonymous_function *nonlin
//                int mCeq
//                const double lb[3]
//                const double ub[3]
//                c_struct_T *obj
// Return Type  : void
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace FiniteDifferences {
void factoryConstruct(const b_anonymous_function *nonlin, int mCeq,
                      const double lb[3], const double ub[3], c_struct_T *obj)
{
  int idx;
  boolean_T b;
  obj->nonlin = *nonlin;
  obj->f_1 = 0.0;
  obj->cEq_1.set_size(mCeq);
  obj->f_2 = 0.0;
  obj->cEq_2.set_size(mCeq);
  obj->nVar = 3;
  obj->mIneq = 0;
  obj->mEq = mCeq;
  obj->numEvals = 0;
  obj->SpecifyObjectiveGradient = false;
  obj->SpecifyConstraintGradient = false;
  obj->isEmptyNonlcon = (mCeq == 0);
  obj->FiniteDifferenceType = 0;
  b = false;
  idx = 0;
  while ((!b) && (idx + 1 <= 3)) {
    obj->hasLB[idx] = ((!std::isinf(lb[idx])) && (!std::isnan(lb[idx])));
    obj->hasUB[idx] = ((!std::isinf(ub[idx])) && (!std::isnan(ub[idx])));
    if (obj->hasLB[idx] || obj->hasUB[idx]) {
      b = true;
    }
    idx++;
  }
  while (idx + 1 <= 3) {
    obj->hasLB[idx] = ((!std::isinf(lb[idx])) && (!std::isnan(lb[idx])));
    obj->hasUB[idx] = ((!std::isinf(ub[idx])) && (!std::isnan(ub[idx])));
    idx++;
  }
  obj->hasBounds = b;
}

} // namespace FiniteDifferences
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for factoryConstruct.cpp
//
// [EOF]
//
