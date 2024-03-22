//
// File: CaculateScatteredPhotons.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 22-Mar-2024 13:35:31
//

#ifndef CACULATESCATTEREDPHOTONS_H
#define CACULATESCATTEREDPHOTONS_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void CaculateScatteredPhotons(
    double energe, double Alpha_water, double Beta_p, double Beta_m,
    double lambda, double n, double z, double H, double tau, double r, double M,
    double energy_ratio, double N_dark, double beta, double Xi, double Xi_F,
    double *N_Brillouin, double *N_Mie, double *N_Rayleigh, double *SNR);

#endif
//
// File trailer for CaculateScatteredPhotons.h
//
// [EOF]
//
