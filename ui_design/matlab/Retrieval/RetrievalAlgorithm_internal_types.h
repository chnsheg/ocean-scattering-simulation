//
// File: RetrievalAlgorithm_internal_types.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

#ifndef RETRIEVALALGORITHM_INTERNAL_TYPES_H
#define RETRIEVALALGORITHM_INTERNAL_TYPES_H

// Include Files
#include "RetrievalAlgorithm_types.h"
#include "anonymous_function.h"
#include "rtwtypes.h"
#include "coder_array.h"

// Type Definitions
struct c_struct_T {
  coder::b_anonymous_function nonlin;
  double f_1;
  coder::array<double, 1U> cEq_1;
  double f_2;
  coder::array<double, 1U> cEq_2;
  int nVar;
  int mIneq;
  int mEq;
  int numEvals;
  boolean_T SpecifyObjectiveGradient;
  boolean_T SpecifyConstraintGradient;
  boolean_T isEmptyNonlcon;
  boolean_T hasLB[3];
  boolean_T hasUB[3];
  boolean_T hasBounds;
  int FiniteDifferenceType;
};

#endif
//
// File trailer for RetrievalAlgorithm_internal_types.h
//
// [EOF]
//
