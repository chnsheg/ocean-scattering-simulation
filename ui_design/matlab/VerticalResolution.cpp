//
// File: VerticalResolution.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 03-Mar-2025 14:04:07
//

// Include Files
#include "VerticalResolution.h"
#include "sum.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
//
// CaculateScatteredPhotonsIntegrated 计算积分切片内的散射光子数
//
//  输入参数:
//    energy          - 激光光子能量
//    Alpha_water     - 水的吸收系数
//    Beta_p          - Mie散射系数（颗粒散射）
//    Beta_m          - 布里渊散射系数（分子散射）
//    lambda          - 激光波长
//    n               - 水的折射率
//    z               - 积分起始深度（单位：m）
//    H               - 接收器在空中的高度（单位：m）
//    tau             - 激光脉冲宽度（单位：s） [保留参数，可用于其它用途]
//    r               - 接收器望远镜半径（单位：m）
//    M               - 积分次数（可以是向量）
//    energy_ratio    - Rayleigh散射能量与布里渊散射能量的比例（例如1）
//    N_dark          - 暗电流计数
//    beta            - 噪声相关参数
//    Xi              - 系统检测效率
//    Xi_F            - 光学系统效率
//    sampling_interval -
//    采样时间间隔（单位：s），例如2e-9代表2ns一个点,可能就是激光的脉冲宽度
//    slice_thickness - 切片厚度（单位：m），例如5表示5m厚度
//
//  输出参数:
//    N_Brillouin_int   - 积分切片内布里渊散射光子的总数
//    N_Mie_int         - 积分切片内Mie散射光子的总数
//    N_Rayleigh_int    - 积分切片内Rayleigh散射光子的总数
//    SNR_out           - 针对各积分次数 M 计算得到的信噪比
//
// Arguments    : double energy
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
//                double sampling_interval
//                double slice_thickness
//                double *N_Brillouin_int
//                double *N_Mie_int
//                double *N_Rayleigh_int
//                double *SNR_out
// Return Type  : void
//
void VerticalResolution(double energy, double Alpha_water, double Beta_p,
                        double Beta_m, double lambda, double n, double z,
                        double H, double tau, double r, double M,
                        double energy_ratio, double N_dark, double beta,
                        double Xi, double Xi_F, double sampling_interval,
                        double slice_thickness, double *N_Brillouin_int,
                        double *N_Mie_int, double *N_Rayleigh_int,
                        double *SNR_out)
{
  coder::array<double, 2U> N_Brillouin_array;
  coder::array<double, 2U> N_Mie_array;
  double Ad;
  double Delta_z_sample;
  double Nd;
  double z_i;
  int i;
  //  常量定义
  //  光速 (m/s)
  //  普朗克常数 (J*s)
  //  计算激光入射的光子总数
  Nd = energy * lambda / 1.9864458571489286E-25;
  //  利用采样间隔计算单点垂直分辨率（与激光脉冲宽度 tau 无关）
  // Delta_z_sample = 2.99792458E+8 * sampling_interval / (2.0 * n);
  Delta_z_sample = 2.99792458E+8 * tau / (2.0 * n);
  //  根据切片厚度确定积分的采样点数
  //  预分配数组用于存储各采样点的散射光子数
  i = static_cast<int>(std::floor(slice_thickness / Delta_z_sample));
  i = 1;
  N_Brillouin_array.set_size(1, i);
  N_Mie_array.set_size(1, i);
  //  接收器口径面积
  Ad = 3.1415926535897931 * (r * r);

  //  对积分区间内每个采样点计算散射光子数
  for (int b_i{0}; b_i < i; b_i++)
  {
    double a;
    double d;
    //  当前采样点深度（从积分起始深度开始）
    // z_i = z + ((static_cast<double>(b_i) + 1.0) - 1.0) * Delta_z_sample;
    z_i = z;
    //  计算接收器的立体角（假设球面扩散）
    a = z_i + n * H;
    //  布里渊散射光子数（乘上了单采样点的厚度Delta_z_sample）
    d = std::exp(-2.0 * Alpha_water * z_i);
    z_i = Nd * (Ad / (a * a));
    N_Brillouin_array[b_i] = z_i * Beta_m * d * Delta_z_sample * Xi * Xi_F;
    //  Mie散射光子数
    N_Mie_array[b_i] = z_i * Beta_p * d * Delta_z_sample;
  }
  //  积分切片内各散射类型光子数（求和积分）
  *N_Brillouin_int = coder::sum(N_Brillouin_array) * M;
  *N_Mie_int = coder::sum(N_Mie_array) * M;
  *N_Rayleigh_int = energy_ratio * *N_Brillouin_int;
  //  计算总信号光子数
  z_i = *N_Rayleigh_int + *N_Brillouin_int;
  //  信噪比计算（这里保留原来的计算方法，可根据需要修改）
  //  背景光子数（此处假设白天情况为0，可进一步参数化）
  // *SNR_out = z_i * std::sqrt(M) / (std::sqrt(z_i * beta) + N_dark);
  *SNR_out = z_i / (std::sqrt(z_i * beta) + N_dark);
}

//
// File trailer for VerticalResolution.cpp
//
// [EOF]
//
