//
// File: drawNsMByDepth.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 25-Mar-2024 11:13:21
//

#ifndef DRAWNSMBYDEPTH_H
#define DRAWNSMBYDEPTH_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void drawNsMByDepth(
    double energe, double Alpha_water, double Beta_p, double Beta_m,
    double lambda, double n, const coder::array<double, 2U> &z, double H,
    double tau, double r, const coder::array<double, 2U> &M,
    double energy_ratio, double N_dark, double beta, double Xi, double Xi_F,
    coder::array<double, 2U> &N_Brillouin, coder::array<double, 2U> &N_Mie,
    coder::array<double, 2U> &N_Rayleigh, coder::array<double, 2U> &Ns);

#endif
//
// File trailer for drawNsMByDepth.h
//
// [EOF]
//
