//
// File: AddNoiseNondB.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 22-Mar-2024 19:43:24
//

// Include Files
#include "AddNoiseNondB.h"
#include "AddNoiseNondB_data.h"
#include "AddNoiseNondB_initialize.h"
#include "rand.h"
#include "randn.h"
#include "rt_nonfinite.h"
#include "sum.h"
#include "coder_array.h"
#include <cmath>

// Function Declarations
static void minus(coder::array<double, 2U> &in1,
                  const coder::array<double, 2U> &in2);

static void plus(coder::array<double, 2U> &in1,
                 const coder::array<double, 2U> &in2);

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                const coder::array<double, 2U> &in2
// Return Type  : void
//
static void minus(coder::array<double, 2U> &in1,
                  const coder::array<double, 2U> &in2)
{
  coder::array<double, 2U> b_in1;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in2.size(1) == 1)
  {
    i = in1.size(1);
  }
  else
  {
    i = in2.size(1);
  }
  b_in1.set_size(1, i);
  stride_0_1 = (in1.size(1) != 1);
  stride_1_1 = (in2.size(1) != 1);
  if (in2.size(1) == 1)
  {
    loop_ub = in1.size(1);
  }
  else
  {
    loop_ub = in2.size(1);
  }
  for (i = 0; i < loop_ub; i++)
  {
    b_in1[i] = in1[i * stride_0_1] - in2[i * stride_1_1];
  }
  in1.set_size(1, b_in1.size(1));
  loop_ub = b_in1.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    in1[i] = b_in1[i];
  }
}

//
// Arguments    : coder::array<double, 2U> &in1
//                const coder::array<double, 2U> &in2
// Return Type  : void
//
static void plus(coder::array<double, 2U> &in1,
                 const coder::array<double, 2U> &in2)
{
  coder::array<double, 2U> b_in2;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in1.size(1) == 1)
  {
    i = in2.size(1);
  }
  else
  {
    i = in1.size(1);
  }
  b_in2.set_size(1, i);
  stride_0_1 = (in2.size(1) != 1);
  stride_1_1 = (in1.size(1) != 1);
  if (in1.size(1) == 1)
  {
    loop_ub = in2.size(1);
  }
  else
  {
    loop_ub = in1.size(1);
  }
  for (i = 0; i < loop_ub; i++)
  {
    b_in2[i] = in2[i * stride_0_1] + in1[i * stride_1_1];
  }
  in1.set_size(1, b_in2.size(1));
  loop_ub = b_in2.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    in1[i] = b_in2[i];
  }
}

//
// Arguments    : const coder::array<double, 2U> &Iv
//                double SNR
//                coder::array<double, 2U> &Iv_Noised
//                double *SNR_cal
// Return Type  : void
//
void AddNoiseNondB(const coder::array<double, 2U> &Iv, double SNR,
                   coder::array<double, 2U> &Iv_Noised, double *SNR_cal)
{
  coder::array<double, 2U> y_tmp;
  double d;
  double u;
  int i;
  int iter;
  if (!isInitialized_AddNoiseNondB)
  {
    AddNoiseNondB_initialize();
  }
  y_tmp.set_size(1, Iv.size(1));
  iter = Iv.size(1);
  for (i = 0; i < iter; i++)
  {
    y_tmp[i] = Iv[i] * 2.0E+10;
  }
  Iv_Noised.set_size(1, y_tmp.size(1));
  i = y_tmp.size(1);
  for (int k{0}; k < i; k++)
  {
    double lk;
    lk = y_tmp[k];
    if (lk >= 0.0)
    {
      if (std::isinf(lk))
      {
        Iv_Noised[k] = rtInf;
      }
      else if (lk >= 15.0)
      {
        Iv_Noised[k] = 0.0;
        double m;
        int exitg1;
        do
        {
          double p;
          double x;
          exitg1 = 0;
          m = std::floor(0.875 * lk);
          if ((!std::isinf(m)) && (!std::isnan(m)))
          {
            double c;
            double v;
            if (m >= 1.0)
            {
              d = m - 0.33333333333333331;
              u = coder::b_rand();
              p = 1.0;
            }
            else
            {
              double ur[2];
              d = rtNaN;
              coder::b_rand(ur);
              u = ur[0];
              p = rtNaN;
            }
            c = 1.0 / std::sqrt(9.0 * d);
            iter = 0;
            x = 0.0;
            int exitg2;
            do
            {
              exitg2 = 0;
              for (v = -1.0; v <= 0.0; v = c * x + 1.0)
              {
                x = coder::randn();
              }
              v *= v * v;
              x *= x;
              if ((u < 1.0 - 0.0331 * x * x) ||
                  (std::log(u) < 0.5 * x + d * ((1.0 - v) + std::log(v))))
              {
                exitg2 = 1;
              }
              else
              {
                iter++;
                if (iter > 1000000)
                {
                  exitg2 = 1;
                }
                else
                {
                  u = coder::b_rand();
                }
              }
            } while (exitg2 == 0);
            x = d * v * p;
          }
          else
          {
            x = m;
          }
          if (lk < x)
          {
            if (m > 1.0)
            {
              p = lk / x;
              if (p <= 1.0)
              {
                d = 0.0;
                iter = static_cast<int>(m - 1.0);
                for (int j{0}; j < iter; j++)
                {
                  if (coder::b_rand() < p)
                  {
                    d++;
                  }
                }
              }
              else
              {
                d = rtNaN;
              }
              Iv_Noised[k] = Iv_Noised[k] + d;
            }
            exitg1 = 1;
          }
          else
          {
            lk -= x;
            Iv_Noised[k] = Iv_Noised[k] + m;
            if (lk < 15.0)
            {
              d = 0.0;
              x = coder::b_rand();
              x = std::log(x);
              for (p = -x; p < lk; p -= x)
              {
                d++;
                x = coder::b_rand();
                x = std::log(x);
              }
              Iv_Noised[k] = Iv_Noised[k] + d;
              exitg1 = 1;
            }
          }
        } while (exitg1 == 0);
      }
      else
      {
        double x;
        d = 0.0;
        x = coder::b_rand();
        x = std::log(x);
        for (double p{-x}; p < y_tmp[k]; p -= x)
        {
          d++;
          x = coder::b_rand();
          x = std::log(x);
        }
        Iv_Noised[k] = d;
      }
    }
    else
    {
      Iv_Noised[k] = rtNaN;
    }
  }
  // 加噪后的干涉谱
  if (Iv_Noised.size(1) == y_tmp.size(1))
  {
    iter = Iv_Noised.size(1) - 1;
    Iv_Noised.set_size(1, Iv_Noised.size(1));
    for (i = 0; i <= iter; i++)
    {
      Iv_Noised[i] = Iv_Noised[i] - y_tmp[i];
    }
  }
  else
  {
    minus(Iv_Noised, y_tmp);
  }
  // 预噪声信号
  // 预噪声信号功率
  y_tmp.set_size(1, Iv.size(1));
  iter = Iv.size(1);
  for (i = 0; i < iter; i++)
  {
    d = Iv[i];
    y_tmp[i] = d * d;
  }
  u = coder::sum(y_tmp) / static_cast<double>(Iv.size(1));
  // 干涉谱功率
  // 功率比
  y_tmp.set_size(1, Iv_Noised.size(1));
  iter = Iv_Noised.size(1);
  for (i = 0; i < iter; i++)
  {
    d = Iv_Noised[i];
    y_tmp[i] = d * d;
  }
  d = std::sqrt(
      u / (coder::sum(y_tmp) / static_cast<double>(Iv_Noised.size(1))) / SNR);
  // 缩放因子
  //  Noise = abs(Noise_pre.*scale); %噪声信号
  Iv_Noised.set_size(1, Iv_Noised.size(1));
  iter = Iv_Noised.size(1) - 1;
  for (i = 0; i <= iter; i++)
  {
    Iv_Noised[i] = Iv_Noised[i] * d;
  }
  // 噪声信号
  y_tmp.set_size(1, Iv_Noised.size(1));
  iter = Iv_Noised.size(1);
  for (i = 0; i < iter; i++)
  {
    d = Iv_Noised[i];
    y_tmp[i] = d * d;
  }
  *SNR_cal = u / (coder::sum(y_tmp) / static_cast<double>(Iv_Noised.size(1)));
  if (Iv.size(1) == Iv_Noised.size(1))
  {
    iter = Iv.size(1) - 1;
    Iv_Noised.set_size(1, Iv.size(1));
    for (i = 0; i <= iter; i++)
    {
      Iv_Noised[i] = Iv[i] + Iv_Noised[i];
    }
  }
  else
  {
    plus(Iv_Noised, Iv);
  }
  // 加噪
}

//
// File trailer for AddNoiseNondB.cpp
//
// [EOF]
//
