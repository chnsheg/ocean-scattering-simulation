//
// File: CaculateScatteredPhotons.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 07-Mar-2024 09:58:44
//

// Include Files
#include "CaculateScatteredPhotons.h"
#include <cmath>

// Function Definitions
//
// UNTITLED11 Summary of this function goes here
//    Detailed explanation goes here
//
// Arguments    : double energe
//                double Alpha_water
//                double Beta_particulate
//                double Beta_m
//                double lambda
//                double n
//                double z
//                double H
//                double energy_ratio
//                double *N_Brillouin
//                double *N_Mie
//                double *N_Rayleigh
// Return Type  : void
//
void CaculateScatteredPhotons(double energe, double Alpha_water,
                              double Beta_particulate, double Beta_m,
                              double lambda, double n, double z, double H,
                              double energy_ratio, double *N_Brillouin,
                              double *N_Mie, double *N_Rayleigh)
{
  double N_Brillouin_tmp;
  double a;
  //  FILEPATH: /d:/graduation
  //  design/preparation/code/PMTFMethods/PMTFSystem/ScatteredPhotons.m
  //
  //  ScatteredPhotons calculates the number of scattered photons in a given
  //  environment.
  //
  //  Inputs:
  //    energe - Energy of the laser photons
  //    Alpha_water - Absorption coefficient of water
  //    Beta_particulate - Scattering coefficient of particulate matter
  //
  //  Outputs:
  //    N_Brillouin - Number of Brillouin scattered photons
  //    N_Mie - Number of Mie scattered photons
  //    N_Rayleigh - Number of Rayleigh scattered photons
  //
  //  Example usage:
  //    [N_Brillouin, N_Mie, N_Rayleigh] = ScatteredPhotons(energe, Alpha_water,
  //    Beta_particulate)
  // 标准状态下（盐度３５％，２０°Ｃ，—个大气压）P55
  // Beta_m = 2.4e-4; %瑞利 布里渊散射系数
  // ---------------------------
  // velocity of light
  // lambda = 532e-9; %wavelength
  // Planck's constant
  // number of laser photos transmitted    入射激光的光子数
  // ---------------------------------------------
  // n = 1.3333; %index of refraction of water   水的折射率
  // z = 10; %depth of range cell(Unit:m)    水下探测深度
  // H = 150; %altitude of receiver in air   接收器在空中的高度
  a = z + n * H;
  // solid angle of receiver 接收器的立体角
  // Number of received scattered photons
  N_Brillouin_tmp = std::exp(-2.0 * Alpha_water * z);
  a = energe * lambda / 1.9846260719600002E-25 * (1.0 / (a * a));
  *N_Brillouin = a * Beta_m * N_Brillouin_tmp;
  *N_Mie = a * Beta_particulate * N_Brillouin_tmp;
  // number of output Brillouin photons
  *N_Rayleigh = energy_ratio * *N_Brillouin;
  // S_r =  0.04 * S_m;
  //  纯水中瑞利散射和布里渊散射的能量比小于
  //  0.04，这意味着瑞利光的强度明显低于水中的布里渊光
}

//
// File trailer for CaculateScatteredPhotons.cpp
//
// [EOF]
//
