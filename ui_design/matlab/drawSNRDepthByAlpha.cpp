//
// File: drawSNRDepthByAlpha.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 24-Mar-2024 20:39:37
//

// Include Files
#include "drawSNRDepthByAlpha.h"
#include "div.h"
#include "coder_array.h"
#include <cmath>

// Function Declarations
static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             coder::array<double, 2U> &in3, double in4,
                             double in5, double in6, double in7);

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                double in2
//                const coder::array<double, 2U> &in3
//                double in4
//                double in5
//                double in6
//                double in7
// Return Type  : void
//
static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             coder::array<double, 2U> &in3, double in4,
                             double in5, double in6, double in7)
{
  coder::array<double, 2U> b_in2;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in1.size(1) == 1)
  {
    i = in3.size(1);
  }
  else
  {
    i = in1.size(1);
  }
  b_in2.set_size(1, i);
  stride_0_1 = (in3.size(1) != 1);
  stride_1_1 = (in1.size(1) != 1);
  if (in1.size(1) == 1)
  {
    loop_ub = in3.size(1);
  }
  else
  {
    loop_ub = in1.size(1);
  }
  for (i = 0; i < loop_ub; i++)
  {
    b_in2[i] =
        in2 * in3[i * stride_0_1] * in4 * in1[i * stride_1_1] * in5 * in6 * in7;
  }
  in1.set_size(1, b_in2.size(1));
  loop_ub = b_in2.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    in1[i] = b_in2[i];
  }
}

//
// UNTITLED11 Summary of this function goes here
//    Detailed explanation goes here
// Beta_p = 0.00; %标准状态下（盐度３５％，２０°Ｃ，—个大气压）P55
// Alpha = Alpha_water;
// Beta_m = 2.4e-4; %瑞利 布里渊散射系数
// ---------------------------
//
// Arguments    : double energe
//                const coder::array<double, 2U> &Alpha_water
//                double Beta_p
//                double Beta_m
//                double lambda
//                double n
//                const coder::array<double, 2U> &z
//                double H
//                double tau
//                double r
//                double M
//                double energy_ratio
//                double N_dark
//                double beta
//                double Xi
//                double Xi_F
//                coder::array<double, 2U> &SNR_db
// Return Type  : void
//
void drawSNRDepthByAlpha(double energe,
                         const coder::array<double, 2U> &Alpha_water, double,
                         double Beta_m, double lambda, double n,
                         const coder::array<double, 2U> &z, double H,
                         double tau, double r, double M, double energy_ratio,
                         double N_dark, double beta, double Xi, double Xi_F,
                         coder::array<double, 2U> &SNR_db)
{
  coder::array<double, 2U> N_Brillouin;
  coder::array<double, 2U> b_x;
  coder::array<double, 2U> d_S;
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
  SNR_db.set_size(Alpha_water.size(1), z.size(1));
  nx = Alpha_water.size(1) * z.size(1);
  for (i = 0; i < nx; i++)
  {
    SNR_db[i] = 0.0;
  }
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
  d_S.set_size(1, z.size(1));
  b_n = n * H;
  nx = z.size(1);
  for (i = 0; i < nx; i++)
  {
    double varargin_1;
    varargin_1 = z[i] + b_n;
    d_S[i] = varargin_1 * varargin_1;
  }
  d_S.set_size(1, d_S.size(1));
  nx = d_S.size(1) - 1;
  for (i = 0; i <= nx; i++)
  {
    d_S[i] = x / d_S[i];
  }
  // solid angle of receiver 接收器的立体角
  // Number of received scattered photons
  i = Alpha_water.size(1);
  for (int b_i{0}; b_i < i; b_i++)
  {
    N_Brillouin.set_size(1, z.size(1));
    x = -2.0 * Alpha_water[b_i];
    nx = z.size(1);
    for (int k{0}; k < nx; k++)
    {
      N_Brillouin[k] = x * z[k];
    }
    nx = N_Brillouin.size(1);
    for (int k{0}; k < nx; k++)
    {
      N_Brillouin[k] = std::exp(N_Brillouin[k]);
    }
    if (d_S.size(1) == N_Brillouin.size(1))
    {
      nx = d_S.size(1) - 1;
      N_Brillouin.set_size(1, d_S.size(1));
      for (int k{0}; k <= nx; k++)
      {
        N_Brillouin[k] =
            Nd * d_S[k] * Beta_m * N_Brillouin[k] * Delta_z * Xi * Xi_F;
      }
    }
    else
    {
      binary_expand_op(N_Brillouin, Nd, d_S, Beta_m, Delta_z, Xi, Xi_F);
    }
    // number of output Brillouin photons
    // S_r =  0.04 * S_m;
    //  纯水中瑞利散射和布里渊散射的能量比小于
    //  0.04，这意味着瑞利光的强度明显低于水中的布里渊光
    // Signal Noise Ratio
    N_Brillouin.set_size(1, N_Brillouin.size(1));
    nx = N_Brillouin.size(1) - 1;
    for (int k{0}; k <= nx; k++)
    {
      x = N_Brillouin[k];
      x += energy_ratio * x;
      N_Brillouin[k] = x;
    }
    //  Day time
    //  dark current
    // M = 1; % Integration times
    // beta = 2;
    // x = std::sqrt(M);
    x = M;
    b_x.set_size(1, N_Brillouin.size(1));
    nx = N_Brillouin.size(1);
    for (int k{0}; k < nx; k++)
    {
      b_x[k] = N_Brillouin[k] * beta * x; // add
    }
    nx = b_x.size(1);
    for (int k{0}; k < nx; k++)
    {
      b_x[k] = std::sqrt(b_x[k]);
    }
    if (N_Brillouin.size(1) == b_x.size(1))
    {
      nx = N_Brillouin.size(1) - 1;
      N_Brillouin.set_size(1, N_Brillouin.size(1));
      for (int k{0}; k <= nx; k++)
      {
        N_Brillouin[k] = N_Brillouin[k] * x / (b_x[k] + N_dark); // add
      }
    }
    else
    {
      binary_expand_op(N_Brillouin, b_x, x, N_dark);
    }
    nx = N_Brillouin.size(1);
    for (int k{0}; k < nx; k++)
    {
      N_Brillouin[k] = std::log10(N_Brillouin[k]);
    }
    nx = N_Brillouin.size(1);
    for (int k{0}; k < nx; k++)
    {
      SNR_db[b_i + SNR_db.size(0) * k] = 10.0 * N_Brillouin[k];
    }
  }
}

//
// File trailer for drawSNRDepthByAlpha.cpp
//
// [EOF]
//
