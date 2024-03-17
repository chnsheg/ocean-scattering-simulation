//
// File: FizeauInstrument.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 07-Mar-2024 14:25:44
//

// Include Files
#include "FizeauInstrument.h"
#include "colon.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>

// Function Declarations
static int div_s32_sat(int numerator, int denominator);

static double rt_hypotd_snf(double u0, double u1);

static double rt_powd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : int numerator
//                int denominator
// Return Type  : int
//
static int div_s32_sat(int numerator, int denominator)
{
  int quotient;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    unsigned int b_denominator;
    unsigned int b_numerator;
    boolean_T quotientNeedsNegation;
    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    if (numerator < 0) {
      b_numerator = ~static_cast<unsigned int>(numerator) + 1U;
    } else {
      b_numerator = static_cast<unsigned int>(numerator);
    }
    if (denominator < 0) {
      b_denominator = ~static_cast<unsigned int>(denominator) + 1U;
    } else {
      b_denominator = static_cast<unsigned int>(denominator);
    }
    b_numerator /= b_denominator;
    if ((!quotientNeedsNegation) && (b_numerator >= 2147483647U)) {
      quotient = MAX_int32_T;
    } else if (quotientNeedsNegation && (b_numerator > 2147483647U)) {
      quotient = MIN_int32_T;
    } else if (quotientNeedsNegation) {
      quotient = -static_cast<int>(b_numerator);
    } else {
      quotient = static_cast<int>(b_numerator);
    }
  }
  return quotient;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = std::abs(u0);
  y = std::abs(u1);
  if (a < y) {
    a /= y;
    y *= std::sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * std::sqrt(y * y + 1.0);
  } else if (!std::isnan(y)) {
    y = a * 1.4142135623730951;
  }
  return y;
}

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (std::isinf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = std::sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > std::floor(u1))) {
      y = rtNaN;
    } else {
      y = std::pow(u0, u1);
    }
  }
  return y;
}

//
// UNTITLED3 Summary of this function goes here
//    Detailed explanation goes here
//     %% Laser parameter
// lambda = 532e-9; % ��������
//
// Arguments    : double SetAlpha
//                const coder::array<double, 2U> &SetFrequency
//                double lambda
//                double Fizeau_plate
//                double Fizeau_angle
//                double Fizeau_index
//                double Fizeau_albedo
//                double *Fizeau_FSR
//                coder::array<double, 2U> &Fizeau_spectrum
//                coder::array<double, 2U> &Fizeau_frequency
// Return Type  : void
//
void FizeauInstrument(double SetAlpha,
                      const coder::array<double, 2U> &SetFrequency,
                      double lambda, double Fizeau_plate, double Fizeau_angle,
                      double Fizeau_index, double Fizeau_albedo,
                      double *Fizeau_FSR,
                      coder::array<double, 2U> &Fizeau_spectrum,
                      coder::array<double, 2U> &Fizeau_frequency)
{
  coder::array<creal_T, 2U> T;
  coder::array<double, 2U> Fizeau_rho;
  double Fizean_length;
  double Fizeau_start;
  double N;
  double R;
  double d;
  int b_i;
  int i;
  int nx;
  int p;
  //  ���٣���/s
  //     %% Parameters of the Fizeau Interferometer
  // Fizeau_plate = 6e-3; % Fizeau plate spacing. Unit: meters 板间距
  //  Fizeau Wedge corners. Unit: meters   楔角
  Fizeau_start = std::tan(SetAlpha);
  Fizean_length = lambda / Fizeau_start / 2.0;
  //  Fizeau plate length.  Unit: meters 板长度，根据pmt成像范围映射到板上的长度
  // Fizeau_angle = 0e-6; % Echo incidence angle，回波入射角
  Fizeau_start = Fizeau_plate / Fizeau_start;
  // 板起始位置,也就是板间距为Fizeau_plate时，对应的rho值，即为起始
  d = Fizean_length / (static_cast<double>(SetFrequency.size(1)) - 1.0);
  N = Fizeau_start + Fizean_length;
  if (std::isnan(Fizeau_start) || std::isnan(d) || std::isnan(N)) {
    Fizeau_rho.set_size(1, 1);
    Fizeau_rho[0] = rtNaN;
  } else if ((d == 0.0) || ((Fizeau_start < N) && (d < 0.0)) ||
             ((N < Fizeau_start) && (d > 0.0))) {
    Fizeau_rho.set_size(1, 0);
  } else if ((std::isinf(Fizeau_start) || std::isinf(N)) &&
             (std::isinf(d) || (Fizeau_start == N))) {
    Fizeau_rho.set_size(1, 1);
    Fizeau_rho[0] = rtNaN;
  } else if (std::isinf(d)) {
    Fizeau_rho.set_size(1, 1);
    Fizeau_rho[0] = Fizeau_start;
  } else if ((std::floor(Fizeau_start) == Fizeau_start) &&
             (std::floor(d) == d)) {
    nx = static_cast<int>((N - Fizeau_start) / d);
    Fizeau_rho.set_size(1, nx + 1);
    for (i = 0; i <= nx; i++) {
      Fizeau_rho[i] = Fizeau_start + d * static_cast<double>(i);
    }
  } else {
    coder::eml_float_colon(Fizeau_start, d, N, Fizeau_rho);
  }
  // 相对距离-----------------------希腊字母rou
  // Fizeau_index = 1; % Fizeau Refractive index of the medium between the two
  // plywoods %介质折射率-----------------------nin Fizeau_albedo = 0.9; %
  // Fizeau albedo    反射率n，R=n²
  R = Fizeau_albedo * Fizeau_albedo;
  *Fizeau_FSR = 2.99792458E+8 / (2.0 * Fizeau_index * Fizeau_plate);
  //  Free spectral range   自由光谱范围,FSR=c/2nd
  Fizean_length = -*Fizeau_FSR / 2.0;
  d = *Fizeau_FSR / (static_cast<double>(SetFrequency.size(1)) - 1.0);
  N = *Fizeau_FSR / 2.0;
  if (std::isnan(Fizean_length) || std::isnan(d) || std::isnan(N)) {
    Fizeau_frequency.set_size(1, 1);
    Fizeau_frequency[0] = rtNaN;
  } else if ((d == 0.0) || ((Fizean_length < N) && (d < 0.0)) ||
             ((N < Fizean_length) && (d > 0.0))) {
    Fizeau_frequency.set_size(1, 0);
  } else if ((std::isinf(Fizean_length) || std::isinf(N)) &&
             (std::isinf(d) || (Fizean_length == N))) {
    Fizeau_frequency.set_size(1, 1);
    Fizeau_frequency[0] = rtNaN;
  } else if (std::isinf(d)) {
    Fizeau_frequency.set_size(1, 1);
    Fizeau_frequency[0] = Fizean_length;
  } else if ((std::floor(Fizean_length) == Fizean_length) &&
             (std::floor(d) == d)) {
    nx = static_cast<int>((N - Fizean_length) / d);
    Fizeau_frequency.set_size(1, nx + 1);
    for (i = 0; i <= nx; i++) {
      Fizeau_frequency[i] = Fizean_length + d * static_cast<double>(i);
    }
  } else {
    coder::eml_float_colon(Fizean_length, d, N, Fizeau_frequency);
  }
  //  Relative frequency, Hz    相对频率
  // R=√R1R2，R1R2为反射率，N_pre为迭代次数的预估值
  N = std::ceil(std::log((1.0 - R) / 200.0 / (R + 1.0)) / std::log(R));
  //  Number of iterations  迭代次数，也就是pmax
  //     %% Calculates the instrument function
  T.set_size(1, SetFrequency.size(1));
  nx = SetFrequency.size(1);
  for (i = 0; i < nx; i++) {
    T[i].re = 0.0;
    T[i].im = 0.0;
  }
  //  将T定义为复数类型
  i = Fizeau_rho.size(1);
  for (b_i = 0; b_i < i; b_i++) {
    nx = static_cast<int>(N);
    for (p = 0; p < nx; p++) {
      double y_re;
      // 公式(5.1)
      // 公式(5.3)
      Fizean_length = SetAlpha * ((static_cast<double>(p) + 1.0) - 1.0);
      Fizean_length = 12.566370614359172 * Fizeau_index * Fizeau_rho[b_i] *
                      std::sin(Fizean_length) *
                      std::cos(Fizeau_angle + Fizean_length) / lambda;
      y_re = Fizean_length * 0.0;
      if (Fizean_length == 0.0) {
        y_re = std::exp(y_re);
        Fizean_length = 0.0;
      } else {
        Fizeau_start = std::exp(y_re / 2.0);
        y_re = Fizeau_start * (Fizeau_start * std::cos(Fizean_length));
        Fizean_length = Fizeau_start * (Fizeau_start * std::sin(Fizean_length));
      }
      d = rt_powd_snf(R, (static_cast<double>(p) + 1.0) - 1.0);
      T[b_i].re = T[b_i].re + d * y_re;
      T[b_i].im = T[b_i].im + d * Fizean_length;
    }
  }
  N = (1.0 - R) * (1.0 - R);
  nx = T.size(1);
  Fizeau_rho.set_size(1, T.size(1));
  for (p = 0; p < nx; p++) {
    Fizeau_rho[p] = rt_hypotd_snf(T[p].re, T[p].im);
  }
  Fizeau_spectrum.set_size(1, Fizeau_rho.size(1));
  nx = Fizeau_rho.size(1);
  for (i = 0; i < nx; i++) {
    Fizeau_start = Fizeau_rho[i];
    Fizeau_spectrum[i] = Fizeau_start * Fizeau_start;
  }
  Fizeau_spectrum.set_size(1, Fizeau_spectrum.size(1));
  nx = Fizeau_spectrum.size(1) - 1;
  for (i = 0; i <= nx; i++) {
    Fizeau_spectrum[i] = Fizeau_spectrum[i] * N;
  }
  // 公式(5.3),(1-R)²为透射率(T1T2)²
  //  Fizeau_spectrum = T ./ polyarea( Fizeau_frequency, T ); %Area
  //  normalization
  nx = Fizeau_spectrum.size(1);
  if (Fizeau_spectrum.size(1) <= 2) {
    if (Fizeau_spectrum.size(1) == 1) {
      b_i = 1;
    } else if ((Fizeau_spectrum[0] <
                Fizeau_spectrum[Fizeau_spectrum.size(1) - 1]) ||
               (std::isnan(Fizeau_spectrum[0]) &&
                (!std::isnan(Fizeau_spectrum[Fizeau_spectrum.size(1) - 1])))) {
      b_i = Fizeau_spectrum.size(1);
    } else {
      b_i = 1;
    }
  } else {
    if (!std::isnan(Fizeau_spectrum[0])) {
      b_i = 1;
    } else {
      boolean_T exitg1;
      b_i = 0;
      p = 2;
      exitg1 = false;
      while ((!exitg1) && (p <= nx)) {
        if (!std::isnan(Fizeau_spectrum[p - 1])) {
          b_i = p;
          exitg1 = true;
        } else {
          p++;
        }
      }
    }
    if (b_i == 0) {
      b_i = 1;
    } else {
      Fizeau_start = Fizeau_spectrum[b_i - 1];
      i = b_i + 1;
      for (p = i; p <= nx; p++) {
        d = Fizeau_spectrum[p - 1];
        if (Fizeau_start < d) {
          Fizeau_start = d;
          b_i = p;
        }
      }
    }
  }
  // 以最大值为中心，向两边平移
  Fizeau_start = static_cast<double>(b_i) -
                 (static_cast<double>(SetFrequency.size(1)) - 1.0) / 2.0;
  if (Fizeau_spectrum.size(1) != 1) {
    boolean_T shiftright;
    if (-Fizeau_start < 0.0) {
      b_i = static_cast<int>(Fizeau_start);
      shiftright = false;
    } else {
      b_i = static_cast<int>(-Fizeau_start);
      shiftright = true;
    }
    if (b_i > Fizeau_spectrum.size(1)) {
      b_i -=
          div_s32_sat(b_i, Fizeau_spectrum.size(1)) * Fizeau_spectrum.size(1);
    }
    if (b_i > (Fizeau_spectrum.size(1) >> 1)) {
      b_i = Fizeau_spectrum.size(1) - b_i;
      shiftright = !shiftright;
    }
    nx = static_cast<int>(
        std::floor(static_cast<double>(Fizeau_spectrum.size(1)) / 2.0));
    Fizeau_rho.set_size(
        1, static_cast<int>(
               std::floor(static_cast<double>(Fizeau_spectrum.size(1)) / 2.0)));
    for (i = 0; i < nx; i++) {
      Fizeau_rho[i] = 0.0;
    }
    nx = Fizeau_spectrum.size(1);
    if ((Fizeau_spectrum.size(1) > 1) && (b_i > 0)) {
      if (shiftright) {
        for (p = 0; p < b_i; p++) {
          Fizeau_rho[p] = Fizeau_spectrum[(p + nx) - b_i];
        }
        i = b_i + 1;
        for (p = nx; p >= i; p--) {
          Fizeau_spectrum[p - 1] = Fizeau_spectrum[(p - b_i) - 1];
        }
        for (p = 0; p < b_i; p++) {
          Fizeau_spectrum[p] = Fizeau_rho[p];
        }
      } else {
        for (p = 0; p < b_i; p++) {
          Fizeau_rho[p] = Fizeau_spectrum[p];
        }
        i = Fizeau_spectrum.size(1) - b_i;
        for (p = 0; p < i; p++) {
          Fizeau_spectrum[p] = Fizeau_spectrum[p + b_i];
        }
        for (p = 0; p < b_i; p++) {
          Fizeau_spectrum[(p + nx) - b_i] = Fizeau_rho[p];
        }
      }
    }
  }
  //  Shift the spectrum to the center
}

//
// File trailer for FizeauInstrument.cpp
//
// [EOF]
//
