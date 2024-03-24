//
// File: drawSNRDepthByM.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 24-Mar-2024 15:12:57
//

#ifndef DRAWSNRDEPTHBYM_H
#define DRAWSNRDEPTHBYM_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void drawSNRDepthByM(
    double energe, double Alpha_water, double Beta_p, double Beta_m,
    double lambda, double n, const coder::array<double, 2U> &z, double H,
    double tau, double r, const coder::array<double, 2U> &M,
    double energy_ratio, double N_dark, double beta, double Xi, double Xi_F,
    coder::array<double, 2U> &N_Brillouin, coder::array<double, 2U> &N_Mie,
    coder::array<double, 2U> &N_Rayleigh, coder::array<double, 2U> &SNR_db);

#endif
//
// File trailer for drawSNRDepthByM.h
//
// [EOF]
//
