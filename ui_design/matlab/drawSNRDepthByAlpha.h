//
// File: drawSNRDepthByAlpha.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 24-Mar-2024 20:39:37
//

#ifndef DRAWSNRDEPTHBYALPHA_H
#define DRAWSNRDEPTHBYALPHA_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void drawSNRDepthByAlpha(double energe,
                                const coder::array<double, 2U> &Alpha_water,
                                double Beta_p, double Beta_m, double lambda,
                                double n, const coder::array<double, 2U> &z,
                                double H, double tau, double r, double M,
                                double energy_ratio, double N_dark, double beta,
                                double Xi, double Xi_F,
                                coder::array<double, 2U> &SNR_db);

#endif
//
// File trailer for drawSNRDepthByAlpha.h
//
// [EOF]
//
