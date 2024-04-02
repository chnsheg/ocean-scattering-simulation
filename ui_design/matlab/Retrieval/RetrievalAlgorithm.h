//
// File: RetrievalAlgorithm.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

#ifndef RETRIEVALALGORITHM_H
#define RETRIEVALALGORITHM_H

// Include Files
#include "RetrievalAlgorithm_types.h"
#include "matlab/rtwtypes.h"
#include "matlab/coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void
RetrievalAlgorithm(double number, const coder::array<double, 2U> &PMT_energy,
                   const coder::array<double, 2U> &Fizeau_spectrum,
                   const double Initial_lower[4], const double Initial_upper[4],
                   const double Initial_value[4], const double params[19],
                   double fitted_value[4], double *resnorm,
                   coder::array<double, 2U> &residual, double *exitflag,
                   struct0_T *output, struct1_T *lambda,
                   coder::array<double, 2U> &jacobia);

void RetrievalAlgorithm_anonFcn1(
    const coder::array<double, 2U> &MeasurementEnergy, double ChannelNumber,
    const coder::array<double, 2U> &Fizeau_spectrum, const double params[19],
    const double Initial_value[4], coder::array<double, 2U> &varargout_1);

#endif
//
// File trailer for RetrievalAlgorithm.h
//
// [EOF]
//
