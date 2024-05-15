//
// File: RetrievalAlgorithm.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

#ifndef RETRIEVALALGORITHM_H
#define RETRIEVALALGORITHM_H

// Include Files
#include "RetrievalAlgorithm_types.h"
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void
RetrievalAlgorithm(double number, const coder::array<double, 2U> &PMT_energy,
                   const coder::array<double, 2U> &Fizeau_spectrum,
                   const double Initial_lower[3], const double Initial_upper[3],
                   const double Initial_value[3], const double params[19],
                   double fitted_value[3], double *resnorm,
                   coder::array<double, 2U> &residual, double *exitflag,
                   struct0_T *output, struct1_T *lambda,
                   coder::array<double, 2U> &jacobia,
                   double tolerance = 1e-24);

void RetrievalAlgorithm_anonFcn1(
    const coder::array<double, 2U> &MeasurementEnergy, double ChannelNumber,
    const coder::array<double, 2U> &Fizeau_spectrum, const double params[19],
    const double Initial_value[3], coder::array<double, 2U> &varargout_1);

#endif
//
// File trailer for RetrievalAlgorithm.h
//
// [EOF]
//
