//
// File: PMTReceive.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 26-Mar-2024 15:08:03
//

#ifndef PMTRECEIVE_H
#define PMTRECEIVE_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void PMTReceive(const coder::array<double, 2U> &frequency,
                       const coder::array<double, 2U> &InputSpectrum,
                       double NumberChannels, double channel_width,
                       double channel_space,
                       coder::array<double, 2U> &channel_energy,
                       coder::array<double, 2U> &channel_sign);

#endif
//
// File trailer for PMTReceive.h
//
// [EOF]
//
