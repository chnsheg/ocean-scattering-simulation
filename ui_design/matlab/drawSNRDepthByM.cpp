//
// File: drawSNRDepthByM.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 24-Mar-2024 15:12:57
//

// Include Files
#include "drawSNRDepthByM.h"
#include "div.h"
#include "coder_array.h"
#include <cmath>

// Function Declarations
static void binary_expand_op(coder::array<double, 2U> &in1,
                             const coder::array<double, 2U> &in2, double in3,
                             double in4, double in5, double in6);

static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             const coder::array<double, 2U> &in3);

static void plus(coder::array<double, 2U> &in1,
                 const coder::array<double, 2U> &in2,
                 const coder::array<double, 2U> &in3);

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                const coder::array<double, 2U> &in2
//                double in3
//                double in4
//                double in5
//                double in6
// Return Type  : void
//
static void binary_expand_op(coder::array<double, 2U> &in1,
                             const coder::array<double, 2U> &in2, double in3,
                             double in4, double in5, double in6)
{
  coder::array<double, 2U> b_in2;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in1.size(1) == 1) {
    i = in2.size(1);
  } else {
    i = in1.size(1);
  }
  b_in2.set_size(1, i);
  stride_0_1 = (in2.size(1) != 1);
  stride_1_1 = (in1.size(1) != 1);
  if (in1.size(1) == 1) {
    loop_ub = in2.size(1);
  } else {
    loop_ub = in1.size(1);
  }
  for (i = 0; i < loop_ub; i++) {
    b_in2[i] =
        in2[i * stride_0_1] * in3 * in1[i * stride_1_1] * in4 * in5 * in6;
  }
  in1.set_size(1, b_in2.size(1));
  loop_ub = b_in2.size(1);
  for (i = 0; i < loop_ub; i++) {
    in1[i] = b_in2[i];
  }
}

//
// Arguments    : coder::array<double, 2U> &in1
//                double in2
//                const coder::array<double, 2U> &in3
// Return Type  : void
//
static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             const coder::array<double, 2U> &in3)
{
  coder::array<double, 2U> b_in1;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in3.size(1) == 1) {
    i = in1.size(1);
  } else {
    i = in3.size(1);
  }
  b_in1.set_size(1, i);
  stride_0_1 = (in1.size(1) != 1);
  stride_1_1 = (in3.size(1) != 1);
  if (in3.size(1) == 1) {
    loop_ub = in1.size(1);
  } else {
    loop_ub = in3.size(1);
  }
  for (i = 0; i < loop_ub; i++) {
    b_in1[i] = in1[i * stride_0_1] * in2 * in3[i * stride_1_1];
  }
  in1.set_size(1, b_in1.size(1));
  loop_ub = b_in1.size(1);
  for (i = 0; i < loop_ub; i++) {
    in1[i] = b_in1[i];
  }
}

//
// Arguments    : coder::array<double, 2U> &in1
//                const coder::array<double, 2U> &in2
//                const coder::array<double, 2U> &in3
// Return Type  : void
//
static void plus(coder::array<double, 2U> &in1,
                 const coder::array<double, 2U> &in2,
                 const coder::array<double, 2U> &in3)
{
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in3.size(1) == 1) {
    i = in2.size(1);
  } else {
    i = in3.size(1);
  }
  in1.set_size(1, i);
  stride_0_1 = (in2.size(1) != 1);
  stride_1_1 = (in3.size(1) != 1);
  if (in3.size(1) == 1) {
    loop_ub = in2.size(1);
  } else {
    loop_ub = in3.size(1);
  }
  for (i = 0; i < loop_ub; i++) {
    in1[i] = in2[i * stride_0_1] + in3[i * stride_1_1];
  }
}

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
//                const coder::array<double, 2U> &z
//                double H
//                double tau
//                double r
//                const coder::array<double, 2U> &M
//                double energy_ratio
//                double N_dark
//                double beta
//                double Xi
//                double Xi_F
//                coder::array<double, 2U> &N_Brillouin
//                coder::array<double, 2U> &N_Mie
//                coder::array<double, 2U> &N_Rayleigh
//                coder::array<double, 2U> &SNR_db
// Return Type  : void
//
void drawSNRDepthByM(double energe, double Alpha_water, double Beta_p,
                     double Beta_m, double lambda, double n,
                     const coder::array<double, 2U> &z, double H, double tau,
                     double r, const coder::array<double, 2U> &M,
                     double energy_ratio, double N_dark, double beta, double Xi,
                     double Xi_F, coder::array<double, 2U> &N_Brillouin,
                     coder::array<double, 2U> &N_Mie,
                     coder::array<double, 2U> &N_Rayleigh,
                     coder::array<double, 2U> &SNR_db)
{
  coder::array<double, 2U> N_SP;
  coder::array<double, 2U> SNR;
  double Delta_z;
  double Nd;
  double b_n;
  double x;
  int i;
  int nx;
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
  // CaculateScatteredPhotons
  // Beta_m = 2.4e-4; %瑞利 布里渊散射系数
  // ---------------------------
  // velocity of light
  // lambda = 532e-9; %wavelength
  // Planck's constant
  Nd = energe * lambda / 1.9864458571489286E-25;
  // number of laser photos transmitted    入射激光的光子数
  // ---------------------------------------------
  // n = 1.3333; %index of refraction of water   水的折射率
  // z = 10; %depth of range cell(Unit:m)    水下探测深度
  // H = 150; %altitude of receiver in air   接收器在空中的高度
  // tau = 10e-9; %激光器脉宽
  Delta_z = 2.99792458E+8 * tau / (2.0 * n);
  // 水中距离分辨率
  // r = 0.05; %望远镜半径，m
  x = 3.1415926535897931 * (r * r);
  // 面积
  // Xi = 0.13; %系统检测效率
  // Xi_F = 0.4; %光学系统效率
  // solid angle of receiver 接收器的立体角
  // Number of received scattered photons
  N_Mie.set_size(1, z.size(1));
  b_n = n * H;
  nx = z.size(1);
  for (i = 0; i < nx; i++) {
    double varargin_1;
    varargin_1 = z[i] + b_n;
    N_Mie[i] = varargin_1 * varargin_1;
  }
  N_Mie.set_size(1, N_Mie.size(1));
  nx = N_Mie.size(1) - 1;
  for (i = 0; i <= nx; i++) {
    N_Mie[i] = Nd * (x / N_Mie[i]);
  }
  N_Brillouin.set_size(1, z.size(1));
  Nd = -2.0 * Alpha_water;
  nx = z.size(1);
  for (i = 0; i < nx; i++) {
    N_Brillouin[i] = Nd * z[i];
  }
  nx = N_Brillouin.size(1);
  for (int k{0}; k < nx; k++) {
    N_Brillouin[k] = std::exp(N_Brillouin[k]);
  }
  if (N_Mie.size(1) == N_Brillouin.size(1)) {
    nx = N_Mie.size(1) - 1;
    N_Brillouin.set_size(1, N_Mie.size(1));
    for (i = 0; i <= nx; i++) {
      N_Brillouin[i] = N_Mie[i] * Beta_m * N_Brillouin[i] * Delta_z * Xi * Xi_F;
    }
  } else {
    binary_expand_op(N_Brillouin, N_Mie, Beta_m, Delta_z, Xi, Xi_F);
  }
  N_SP.set_size(1, z.size(1));
  nx = z.size(1);
  for (i = 0; i < nx; i++) {
    N_SP[i] = Nd * z[i];
  }
  nx = N_SP.size(1);
  for (int k{0}; k < nx; k++) {
    N_SP[k] = std::exp(N_SP[k]);
  }
  if (N_Mie.size(1) == N_SP.size(1)) {
    nx = N_Mie.size(1) - 1;
    N_Mie.set_size(1, N_Mie.size(1));
    for (i = 0; i <= nx; i++) {
      N_Mie[i] = N_Mie[i] * Beta_p * N_SP[i];
    }
  } else {
    binary_expand_op(N_Mie, Beta_p, N_SP);
  }
  // number of output Brillouin photons
  N_Rayleigh.set_size(1, N_Brillouin.size(1));
  nx = N_Brillouin.size(1);
  for (i = 0; i < nx; i++) {
    N_Rayleigh[i] = energy_ratio * N_Brillouin[i];
  }
  // S_r =  0.04 * S_m;
  //  纯水中瑞利散射和布里渊散射的能量比小于
  //  0.04，这意味着瑞利光的强度明显低于水中的布里渊光
  // Signal Noise Ratio
  if (N_Rayleigh.size(1) == N_Brillouin.size(1)) {
    N_SP.set_size(1, N_Rayleigh.size(1));
    nx = N_Rayleigh.size(1);
    for (i = 0; i < nx; i++) {
      N_SP[i] = N_Rayleigh[i] + N_Brillouin[i];
    }
  } else {
    plus(N_SP, N_Rayleigh, N_Brillouin);
  }
  //  Day time
  //  dark current
  // M = 1; % Integration times
  // beta = 2;
  SNR_db.set_size(M.size(1), N_SP.size(1));
  nx = M.size(1) * N_SP.size(1);
  for (i = 0; i < nx; i++) {
    SNR_db[i] = 0.0;
  }
  i = M.size(1);
  for (int b_i{0}; b_i < i; b_i++) {
    x = std::sqrt(M[b_i]);
    SNR.set_size(1, N_SP.size(1));
    nx = N_SP.size(1);
    for (int k{0}; k < nx; k++) {
      SNR[k] = N_SP[k] * beta;
    }
    nx = SNR.size(1);
    for (int k{0}; k < nx; k++) {
      SNR[k] = std::sqrt(SNR[k]);
    }
    if (N_SP.size(1) == SNR.size(1)) {
      nx = N_SP.size(1) - 1;
      SNR.set_size(1, N_SP.size(1));
      for (int k{0}; k <= nx; k++) {
        SNR[k] = N_SP[k] * x / (SNR[k] + N_dark);
      }
    } else {
      binary_expand_op(SNR, N_SP, x, N_dark);
    }
    nx = SNR.size(1);
    for (int k{0}; k < nx; k++) {
      SNR[k] = std::log10(SNR[k]);
    }
    nx = SNR.size(1);
    for (int k{0}; k < nx; k++) {
      SNR_db[b_i + SNR_db.size(0) * k] = 10.0 * SNR[k];
    }
  }
}

//
// File trailer for drawSNRDepthByM.cpp
//
// [EOF]
//
