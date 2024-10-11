//
// File: PMTReceive.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 26-Mar-2024 15:08:03
//

// Include Files
#include "PMTReceive.h"
#include "rt_nonfinite.h"
#include "coder_array.h"
#include <cmath>

// Function Definitions
//
// UNTITLED4 Summary of this function goes here
//    Detailed explanation goes here
//     %% PMT Array System Parameters
//
// Arguments    : const coder::array<double, 2U> &frequency
//                const coder::array<double, 2U> &InputSpectrum
//                double NumberChannels
//                double channel_width
//                double channel_space
//                coder::array<double, 2U> &channel_energy
//                coder::array<double, 2U> &channel_sign
// Return Type  : void
//
void PMTReceive(const coder::array<double, 2U> &frequency,
                const coder::array<double, 2U> &InputSpectrum,
                double NumberChannels, double channel_width,
                double channel_space, coder::array<double, 2U> &channel_energy,
                coder::array<double, 2U> &channel_sign)
{
  coder::array<double, 1U> c;
  double PMT_length;
  double channel_points;
  double space_points;
  int i;
  //  Number of the PMT Channerls
  // channel_width = 0.8e-3; % PMT width per channel. Unit: meters
  // channel_space = 0.2e-3; % Channel spacing. Unit: meters
  PMT_length =
      NumberChannels * channel_width + (NumberChannels - 1.0) * channel_space;
  //     %% PMT Receive Energy
  channel_points = std::floor(channel_width / PMT_length *
                              static_cast<double>(InputSpectrum.size(1)));
  //  Number of points per channel
  space_points = std::floor(channel_space / PMT_length *
                            static_cast<double>(InputSpectrum.size(1)));
  //  Number of points per space
  i = static_cast<int>(NumberChannels);
  channel_energy.set_size(1, i);
  //  计算每个通道的起始点和终止点，通过面积计算每个通道的能量
  channel_sign.set_size(1, i);
  for (int b_i{0}; b_i < i; b_i++)
  {
    double end_point;
    int i1;
    int i2;
    int iac;
    int ix;
    int loop_ub;
    PMT_length = ((static_cast<double>(b_i) + 1.0) - 1.0) *
                     (channel_points + space_points) +
                 1.0;
    end_point = PMT_length + channel_points;
    if (PMT_length > end_point)
    {
      ix = 0;
      i1 = -1;
      i2 = -1;
      iac = -1;
    }
    else
    {
      ix = static_cast<int>(PMT_length) - 1;
      i1 = static_cast<int>(end_point) - 1;
      i2 = static_cast<int>(PMT_length) - 2;
      iac = static_cast<int>(end_point) - 1;
    }
    PMT_length = 0.0;
    loop_ub = iac - i2;
    if (loop_ub <= 1)
    {
      if (((i1 - ix) + 1 == 1) &&
          (std::isinf(frequency[ix]) || std::isnan(frequency[ix])))
      {
        PMT_length = rtNaN;
      }
    }
    else
    {
      int c_tmp;
      if ((i1 - ix) + 1 == 1)
      {
        c.set_size(loop_ub);
        for (i1 = 0; i1 < loop_ub; i1++)
        {
          c[i1] = frequency[ix];
        }
        end_point = 0.5 * frequency[ix];
        c[0] = end_point;
        c[(iac - i2) - 1] = end_point;
      }
      else
      {
        c.set_size(loop_ub);
        c[0] = 0.5 * (frequency[ix + 1] - frequency[ix]);
        i1 = loop_ub - 1;
        for (int k{2}; k <= i1; k++)
        {
          c_tmp = ix + k;
          c[k - 1] = 0.5 * (frequency[c_tmp] - frequency[c_tmp - 2]);
        }
        c_tmp = (ix + iac) - i2;
        c[loop_ub - 1] = 0.5 * (frequency[c_tmp - 1] - frequency[c_tmp - 2]);
      }
      ix = 0;
      for (iac = 1; iac <= loop_ub; iac++)
      {
        for (c_tmp = iac; c_tmp <= iac; c_tmp++)
        {
          PMT_length += InputSpectrum[i2 + c_tmp] * c[ix];
        }
        ix++;
      }
    }
    channel_energy[b_i] = PMT_length;
    //  计算每个通道的中心频率
    channel_sign[b_i] =
        frequency[static_cast<int>(((static_cast<double>(b_i) + 1.0) - 1.0) *
                                       (channel_points + space_points) +
                                   std::floor(channel_points / 2.0)) -
                  1];
  }
}

// void PMTReceive(const coder::array<double, 2U> &frequency,
//                 const coder::array<double, 2U> &InputSpectrum,
//                 double NumberChannels, double channel_width,
//                 double channel_space, coder::array<double, 2U> &channel_energy,
//                 coder::array<double, 2U> &channel_sign)
// {
//   double PMT_length;
//   double channel_points;
//   double space_points;
//   int i;
//   //  Number of the PMT Channerls
//   // channel_width = 0.8e-3; % PMT width per channel. Unit: meters
//   // channel_space = 0.2e-3; % Channel spacing. Unit: meters
//   PMT_length =
//       NumberChannels * channel_width + (NumberChannels - 1.0) * channel_space;
//   //     %% PMT Receive Energy
//   channel_points = std::floor(channel_width / PMT_length *
//                               static_cast<double>(InputSpectrum.size(1)));
//   //  Number of points per channel
//   space_points = std::floor(channel_space / PMT_length *
//                             static_cast<double>(InputSpectrum.size(1)));
//   //  Number of points per space
//   i = static_cast<int>(NumberChannels);
//   channel_energy.set_size(1, i);
//   //  计算每个通道的起始点和终止点，通过面积计算每个通道的能量
//   channel_sign.set_size(1, i);
//   for (int b_i{0}; b_i < i; b_i++)
//   {
//     double end_point;
//     int i1;
//     int i2;
//     int i3;
//     int s_tmp;
//     PMT_length = ((static_cast<double>(b_i) + 1.0) - 1.0) *
//                      (channel_points + space_points) +
//                  1.0;
//     end_point = PMT_length + channel_points;
//     if (PMT_length > end_point)
//     {
//       i1 = 0;
//       i2 = -1;
//       i3 = 0;
//     }
//     else
//     {
//       i1 = static_cast<int>(PMT_length) - 1;
//       i2 = static_cast<int>(end_point) - 1;
//       i3 = static_cast<int>(PMT_length) - 1;
//     }
//     s_tmp = i2 - i1;
//     if (s_tmp + 1 == 0)
//     {
//       channel_energy[b_i] = 0.0;
//     }
//     else
//     {
//       PMT_length = (frequency[i1] - frequency[i2]) *
//                    (InputSpectrum[i3] + InputSpectrum[(i3 + i2) - i1]) / 2.0;
//       i2 = s_tmp - 1;
//       for (int k{0}; k <= i2; k++)
//       {
//         int b_s_tmp;
//         s_tmp = i1 + k;
//         b_s_tmp = i3 + k;
//         PMT_length += (frequency[s_tmp + 1] - frequency[s_tmp]) *
//                       (InputSpectrum[b_s_tmp + 1] + InputSpectrum[b_s_tmp]) /
//                       2.0;
//       }
//       channel_energy[b_i] = std::abs(PMT_length);
//     }
//     //  计算每个通道的中心频率
//     channel_sign[b_i] =
//         frequency[static_cast<int>(((static_cast<double>(b_i) + 1.0) - 1.0) *
//                                        (channel_points + space_points) +
//                                    std::floor(channel_points / 2.0)) -
//                   1];
//   }
//   //  生成柱状图的x坐标，即通道的中心频率
//   // bar_centers = channel_sign;
//   //  设置柱形的宽度，保持和通道宽度成比例
//   //  bar_widths = (channel_width / PMT_length) * length(frequency);
//   //  bar_widths是frequency的间隔，这个间隔之间有channel_points个点，所以bar_widths
//   //  = frequency(channel_points + 1) - frequency(1)
//   // bar_widths = frequency(channel_points + 1) - frequency(1);
//   //  缩放因子，将柱状图的中心点的高度与RBS曲线的中心点的高度相匹配
//   // scale_factor = max(InputSpectrum) / max(channel_energy);
//   //  缩放每个通道的能量
//   // channel_energy = channel_energy * scale_factor;
//   //  绘制每个通道的能量柱状图
//   // figure; % 打开一个新的绘图窗口
//   // bar(bar_centers, channel_energy, channel_width / (channel_width +
//   // channel_space), 'FaceColor', 'r', 'EdgeColor', 'r'); hold on; %
//   // 保持当前图形，以便在上面绘制其他内容
//   //  绘制RBS曲线
//   // plot(frequency, InputSpectrum, 'k', 'LineWidth', 1.5);
//   //  设置图形的其它属性
//   // xlabel('Relative frequency (GHz)');
//   // ylabel('Relative energy');
//   // title('PMT Array Output with Overlaid RBS Curve');
//   // xlim([min(frequency) max(frequency)]); % 设置x轴的限制以匹配频率数据的范围
//   // hold off; % 释放图形，完成绘图
// }

//
// File trailer for PMTReceive.cpp
//
// [EOF]
//
