//
// File: SpectrumGeneration.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 06-Mar-2024 10:20:46
//

// Include Files
#include "SpectrumGeneration.h"
#include "coder_array.h"
#include <cmath>

// Function Declarations
static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             double in3, const coder::array<double, 2U> &in4,
                             double in5);

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                double in2
//                double in3
//                const coder::array<double, 2U> &in4
//                double in5
// Return Type  : void
//
static void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                             double in3, const coder::array<double, 2U> &in4,
                             double in5)
{
  coder::array<double, 2U> b_in2;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in4.size(1) == 1)
  {
    i = in1.size(1);
  }
  else
  {
    i = in4.size(1);
  }
  b_in2.set_size(1, i);
  stride_0_1 = (in1.size(1) != 1);
  stride_1_1 = (in4.size(1) != 1);
  if (in4.size(1) == 1)
  {
    loop_ub = in1.size(1);
  }
  else
  {
    loop_ub = in4.size(1);
  }
  for (i = 0; i < loop_ub; i++)
  {
    b_in2[i] = in2 / (4.0 * in1[i * stride_0_1] + in3) +
               in2 / (4.0 * in4[i * stride_1_1] + in5);
  }
  in1.set_size(1, b_in2.size(1));
  loop_ub = b_in2.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    in1[i] = b_in2[i];
  }
}

//
// UNTITLED Summary of this function goes here
//    Detailed explanation goes here
//  生成面积归一化光谱
//  miu_0 = c/lambda_0;              %激光器中心频率
//  miu = miu_0-12e9:1e6:miu_0+12e9; %频率探测接收范围
//
// Arguments    : double miu_0
//                const coder::array<double, 2U> &miu
//                double Brillouin_shift
//                double Brillouin_width
//                coder::array<double, 2U> &S_m
//                coder::array<double, 2U> &S_r
//                coder::array<double, 2U> &S_b
// Return Type  : void
//
void SpectrumGeneration(double miu_0, const coder::array<double, 2U> &miu,
                        double Brillouin_shift, double Brillouin_width,
                        coder::array<double, 2U> &S_m,
                        coder::array<double, 2U> &S_r,
                        coder::array<double, 2U> &S_b)
{
  coder::array<double, 2U> a_tmp;
  double c_tmp;
  double delta_b;
  double v_b;
  double vb1;
  double vb2;
  int i;
  int loop_ub;
  //  瑞利峰位置，单位Hz
  v_b = Brillouin_shift * 1.0E+9;
  //  频移量，单位Hz
  vb1 = miu_0 - v_b;
  // 布里渊峰位置，单位Hz
  vb2 = miu_0 + v_b;
  delta_b = Brillouin_width * 1.0E+9;
  //  布里渊线宽，单位Hz
  //  瑞利线宽(150MHz)，单位GHz %瑞利线宽比米散射宽
  //  米散射线宽(20~30MHz)，单位GHz,和激光线宽一样
  // 计算米散射光谱
  a_tmp.set_size(1, miu.size(1));
  loop_ub = miu.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    a_tmp[i] = miu[i] - miu_0;
  }
  S_m.set_size(1, a_tmp.size(1));
  loop_ub = a_tmp.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    v_b = a_tmp[i];
    S_m[i] = v_b * v_b;
  }
  S_m.set_size(1, S_m.size(1));
  loop_ub = S_m.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    S_m[i] = 4.0E+7 / (4.0 * S_m[i] + 1.6E+15);
  }
  //  定义瑞利峰，线型为洛伦兹函数
  if (miu.size(1) == 0)
  {
    v_b = 0.0;
  }
  else
  {
    v_b =
        (miu[0] - miu[miu.size(1) - 1]) * (S_m[0] + S_m[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (loop_ub = 0; loop_ub <= i - 2; loop_ub++)
    {
      v_b += (miu[loop_ub + 1] - miu[loop_ub]) *
             (S_m[loop_ub + 1] + S_m[loop_ub]) / 2.0;
    }
    v_b = std::abs(v_b);
  }
  S_m.set_size(1, S_m.size(1));
  loop_ub = S_m.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    S_m[i] = S_m[i] / v_b;
  }
  //  米散射峰归一化
  //  计算瑞利-布里渊谱线(采用面积归一化 A = polyarea(x,y)\)
  S_r.set_size(1, a_tmp.size(1));
  loop_ub = a_tmp.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    v_b = a_tmp[i];
    S_r[i] = v_b * v_b;
  }
  S_r.set_size(1, S_r.size(1));
  loop_ub = S_r.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    S_r[i] = 1.5E+8 / (4.0 * S_r[i] + 2.25E+16);
  }
  if (miu.size(1) == 0)
  {
    v_b = 0.0;
  }
  else
  {
    v_b =
        (miu[0] - miu[miu.size(1) - 1]) * (S_r[0] + S_r[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (loop_ub = 0; loop_ub <= i - 2; loop_ub++)
    {
      v_b += (miu[loop_ub + 1] - miu[loop_ub]) *
             (S_r[loop_ub + 1] + S_r[loop_ub]) / 2.0;
    }
    v_b = std::abs(v_b);
  }
  S_r.set_size(1, S_r.size(1));
  loop_ub = S_r.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    S_r[i] = S_r[i] / v_b;
  }
  //  瑞利峰归一化
  //  定义布里渊峰，线型为洛伦兹函数
  c_tmp = delta_b * delta_b;
  S_b.set_size(1, miu.size(1));
  loop_ub = miu.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    v_b = miu[i] - vb1;
    S_b[i] = v_b * v_b;
  }
  a_tmp.set_size(1, miu.size(1));
  loop_ub = miu.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    v_b = miu[i] - vb2;
    a_tmp[i] = v_b * v_b;
  }
  if (S_b.size(1) == a_tmp.size(1))
  {
    loop_ub = S_b.size(1) - 1;
    S_b.set_size(1, S_b.size(1));
    for (i = 0; i <= loop_ub; i++)
    {
      S_b[i] =
          delta_b / (4.0 * S_b[i] + c_tmp) + delta_b / (4.0 * a_tmp[i] + c_tmp);
    }
  }
  else
  {
    binary_expand_op(S_b, delta_b, c_tmp, a_tmp, c_tmp);
  }
  if (miu.size(1) == 0)
  {
    v_b = 0.0;
  }
  else
  {
    v_b =
        (miu[0] - miu[miu.size(1) - 1]) * (S_b[0] + S_b[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (loop_ub = 0; loop_ub <= i - 2; loop_ub++)
    {
      v_b += (miu[loop_ub + 1] - miu[loop_ub]) *
             (S_b[loop_ub + 1] + S_b[loop_ub]) / 2.0;
    }
    v_b = std::abs(v_b);
  }
  S_b.set_size(1, S_b.size(1));
  loop_ub = S_b.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    S_b[i] = S_b[i] / v_b;
  }
  //  米散射峰归一化
}

//
// File trailer for SpectrumGeneration.cpp
//
// [EOF]
//
