//
// File: FizeauInstrument.h
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 07-Mar-2024 14:25:44
//

#ifndef FIZEAUINSTRUMENT_H
#define FIZEAUINSTRUMENT_H

// Include Files
#include "rtwtypes.h"
#include "coder_array.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void FizeauInstrument(double SetAlpha,
                             const coder::array<double, 2U> &SetFrequency,
                             double lambda, double Fizeau_plate,
                             double Fizeau_angle, double Fizeau_index,
                             double Fizeau_albedo, double *Fizeau_FSR,
                             coder::array<double, 2U> &Fizeau_spectrum,
                             coder::array<double, 2U> &Fizeau_frequency);

#endif
//
// File trailer for FizeauInstrument.h
//
// [EOF]
//
