//
// File: computeFiniteDifferences.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 04-Mar-2025 09:14:41
//

// Include Files
#include "computeFiniteDifferences.h"
#include "RetrievalAlgorithm.h"
#include "RetrievalAlgorithm_internal_types.h"
#include "RetrievalAlgorithm_internal_types1.h"
#include "RetrievalAlgorithm_internal_types11.h"
#include "anonymous_function.h"
#include "anonymous_function1.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : c_struct_T *obj
//                const ::coder::array<double, 2U> &cEqCurrent
//                double xk[3]
//                ::coder::array<double, 2U> &JacCeqTrans
//                const double lb[3]
//                const double ub[3]
// Return Type  : boolean_T
//
namespace coder {
namespace optim {
namespace coder {
namespace utils {
namespace FiniteDifferences {
boolean_T computeFiniteDifferences(c_struct_T *obj,
                                   const ::coder::array<double, 2U> &cEqCurrent,
                                   double xk[3],
                                   ::coder::array<double, 2U> &JacCeqTrans,
                                   const double lb[3], const double ub[3])
{
  array<double, 2U> r;
  boolean_T evalOK;
  if (obj->isEmptyNonlcon) {
    evalOK = true;
  } else {
    int idx;
    boolean_T exitg1;
    evalOK = true;
    obj->numEvals = 0;
    idx = 0;
    exitg1 = false;
    while ((!exitg1) && (idx < 3)) {
      double deltaX;
      double lbDiff;
      double ubDiff;
      int b_idx;
      int i;
      boolean_T guard1{false};
      boolean_T modifiedStep;
      modifiedStep = false;
      deltaX = 1.4901161193847656E-8 *
               (1.0 - 2.0 * static_cast<double>(xk[idx] < 0.0)) *
               std::fmax(std::abs(xk[idx]), 1.0);
      if (obj->hasLB[idx] || obj->hasUB[idx]) {
        if (obj->hasLB[idx] && obj->hasUB[idx]) {
          lbDiff = deltaX;
          if ((lb[idx] != ub[idx]) && (xk[idx] >= lb[idx]) &&
              (xk[idx] <= ub[idx]) &&
              ((xk[idx] + deltaX > ub[idx]) || (xk[idx] + deltaX < lb[idx]))) {
            lbDiff = -deltaX;
            modifiedStep = true;
            ubDiff = xk[idx] + -deltaX;
            if ((ubDiff > ub[idx]) || (ubDiff < lb[idx])) {
              lbDiff = xk[idx] - lb[idx];
              ubDiff = ub[idx] - xk[idx];
              if (lbDiff <= ubDiff) {
                lbDiff = -lbDiff;
              } else {
                lbDiff = ubDiff;
              }
            }
          }
          deltaX = lbDiff;
        } else if (obj->hasUB[idx]) {
          if ((xk[idx] <= ub[idx]) && (xk[idx] + deltaX > ub[idx])) {
            deltaX = -deltaX;
            modifiedStep = true;
          }
        } else if ((xk[idx] >= lb[idx]) && (xk[idx] + deltaX < lb[idx])) {
          deltaX = -deltaX;
          modifiedStep = true;
        }
      }
      evalOK = true;
      lbDiff = xk[idx];
      xk[idx] += deltaX;
      RetrievalAlgorithm_anonFcn1(
          obj->nonlin.workspace.fun.workspace.MeasurementEnergy,
          obj->nonlin.workspace.fun.workspace.ChannelNumber,
          obj->nonlin.workspace.fun.workspace.Fizeau_spectrum,
          obj->nonlin.workspace.fun.workspace.params, xk, r);
      b_idx = 0;
      while (evalOK && (b_idx + 1 <= obj->mEq)) {
        evalOK = ((!std::isinf(r[b_idx])) && (!std::isnan(r[b_idx])));
        b_idx++;
      }
      xk[idx] = lbDiff;
      obj->f_1 = 0.0;
      obj->cEq_1.set_size(r.size(1));
      b_idx = r.size(1);
      for (i = 0; i < b_idx; i++) {
        obj->cEq_1[i] = r[i];
      }
      obj->numEvals++;
      guard1 = false;
      if (!evalOK) {
        if (!modifiedStep) {
          deltaX = -deltaX;
          if (obj->hasLB[idx]) {
            ubDiff = xk[idx] + deltaX;
            if ((ubDiff >= lb[idx]) && obj->hasUB[idx] && (ubDiff <= ub[idx])) {
              modifiedStep = true;
            } else {
              modifiedStep = false;
            }
          } else {
            modifiedStep = false;
          }
          if ((!obj->hasBounds) || modifiedStep) {
            evalOK = true;
            lbDiff = xk[idx];
            xk[idx] += deltaX;
            RetrievalAlgorithm_anonFcn1(
                obj->nonlin.workspace.fun.workspace.MeasurementEnergy,
                obj->nonlin.workspace.fun.workspace.ChannelNumber,
                obj->nonlin.workspace.fun.workspace.Fizeau_spectrum,
                obj->nonlin.workspace.fun.workspace.params, xk, r);
            b_idx = 0;
            while (evalOK && (b_idx + 1 <= obj->mEq)) {
              evalOK = ((!std::isinf(r[b_idx])) && (!std::isnan(r[b_idx])));
              b_idx++;
            }
            xk[idx] = lbDiff;
            obj->f_1 = 0.0;
            obj->cEq_1.set_size(r.size(1));
            b_idx = r.size(1);
            for (i = 0; i < b_idx; i++) {
              obj->cEq_1[i] = r[i];
            }
            obj->numEvals++;
          }
        }
        if (!evalOK) {
          exitg1 = true;
        } else {
          guard1 = true;
        }
      } else {
        guard1 = true;
      }
      if (guard1) {
        i = obj->mEq;
        for (b_idx = 0; b_idx < i; b_idx++) {
          JacCeqTrans[idx + 3 * b_idx] =
              (obj->cEq_1[b_idx] - cEqCurrent[b_idx]) / deltaX;
        }
        idx++;
      }
    }
  }
  return evalOK;
}

} // namespace FiniteDifferences
} // namespace utils
} // namespace coder
} // namespace optim
} // namespace coder

//
// File trailer for computeFiniteDifferences.cpp
//
// [EOF]
//
