//
// File: convolution.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 06-Mar-2024 14:46:49
//

#ifndef CONVOLUTION_H
#define CONVOLUTION_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void convolution(const coder::array<double, 2U> &input,
                        const coder::array<double, 2U> &kernel,
                        coder::array<double, 2U> &L_out);

#endif
//
// File trailer for convolution.h
//
// [EOF]
//
