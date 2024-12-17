//
// File: CaculateScatteredPhotons.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 22-Mar-2024 13:35:31
//

// Include Files
#include "CaculateScatteredPhotons.h"
#include <cmath>
#include <QDebug>

// Function Definitions
//
// UNTITLED11 Summary of this function goes here
//    Detailed explanation goes here
// Beta_p = 0.00; %标准状态下（盐度３５％，２０°Ｃ，—个大气压）P55
//
// Arguments    : double energe
//                double Alpha_water
//                double Beta_p
//                double Beta_m
//                double lambda
//                double n
//                double z
//                double H
//                double tau
//                double r
//                double M
//                double energy_ratio
//                double N_dark
//                double beta
//                double Xi
//                double Xi_F
//                double *N_Brillouin
//                double *N_Mie
//                double *N_Rayleigh
//                double *SNR
// Return Type  : void
//
void CaculateScatteredPhotons(double energe, double Alpha_water, double Beta_p,
                              double Beta_m, double lambda, double n, double z,
                              double H, double tau, double r, double M,
                              double energy_ratio, double N_dark, double beta,
                              double Xi, double Xi_F, double *N_Brillouin,
                              double *N_Mie, double *N_Rayleigh, double *SNR)
{
  double N_Brillouin_tmp;
  double N_SP;
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
  // tau = 10e-9; %激光器脉宽
  // 水中距离分辨率
  // r = 0.05; %望远镜半径，m
  // 面积
  // Xi = 0.13; %系统检测效率
  // Xi_F = 0.4; %光学系统效率
  N_SP = z + n * H;
  // solid angle of receiver 接收器的立体角
  // Number of received scattered photons
  N_Brillouin_tmp = std::exp(-2.0 * Alpha_water * z);
  N_SP = energe * lambda / 1.9864458571489286E-25 *
         (3.1415926535897931 * (r * r) / (N_SP * N_SP));
  *N_Brillouin = N_SP * Beta_m * N_Brillouin_tmp *
                 (2.99792458E+8 * tau / (2.0 * n)) * Xi * Xi_F;
  *N_Mie = N_SP * Beta_p * N_Brillouin_tmp;
  // number of output Brillouin photons
  *N_Rayleigh = energy_ratio * *N_Brillouin;
  // S_r =  0.04 * S_m;
  //  纯水中瑞利散射和布里渊散射的能量比小于
  //  0.04，这意味着瑞利光的强度明显低于水中的布里渊光
  // Signal Noise Ratio
  // N_SP = M * (*N_Rayleigh + *N_Brillouin);
  *N_Rayleigh = M * *N_Rayleigh;
  *N_Brillouin = M * *N_Brillouin;
  N_SP = 1 * (*N_Rayleigh + *N_Brillouin);
  //  Day time
  //  dark current
  // M = 1; % Integration times
  // beta = 2;
  // *SNR = N_SP * std::sqrt(M) / (std::sqrt(N_SP * beta) + N_dark);  // 乘根号M
  *SNR = N_SP / (std::sqrt(N_SP * beta) + N_dark);

  qDebug() << "calculate SNR: " << *SNR;
}

//
// File trailer for CaculateScatteredPhotons.cpp
//
// [EOF]
//
