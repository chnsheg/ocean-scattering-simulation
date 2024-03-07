//
// File: SpectrumGeneration.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 06-Mar-2024 10:20:46
//

#ifndef SPECTRUMGENERATION_H
#define SPECTRUMGENERATION_H

// Include Files
#include "coder_array.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void SpectrumGeneration(double miu_0,
                               const coder::array<double, 2U> &miu,
                               double Brillouin_shift, double Brillouin_width,
                               coder::array<double, 2U> &S_m,
                               coder::array<double, 2U> &S_r,
                               coder::array<double, 2U> &S_b);

#endif
//
// File trailer for SpectrumGeneration.h
//
// [EOF]
//
