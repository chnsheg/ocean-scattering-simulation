//
// File: PMTReceive.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 07-Mar-2024 20:29:20
//

// Include Files
#include "PMTReceive.h"
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
  for (int b_i{0}; b_i < i; b_i++) {
    double end_point;
    int i1;
    int i2;
    int i3;
    int s_tmp;
    PMT_length = ((static_cast<double>(b_i) + 1.0) - 1.0) *
                     (channel_points + space_points) +
                 1.0;
    end_point = PMT_length + channel_points;
    if (PMT_length > end_point) {
      i1 = 0;
      i2 = -1;
      i3 = 0;
    } else {
      i1 = static_cast<int>(PMT_length) - 1;
      i2 = static_cast<int>(end_point) - 1;
      i3 = static_cast<int>(PMT_length) - 1;
    }
    s_tmp = i2 - i1;
    if (s_tmp + 1 == 0) {
      channel_energy[b_i] = 0.0;
    } else {
      PMT_length = (frequency[i1] - frequency[i2]) *
                   (InputSpectrum[i3] + InputSpectrum[(i3 + i2) - i1]) / 2.0;
      i2 = s_tmp - 1;
      for (int k{0}; k <= i2; k++) {
        int b_s_tmp;
        s_tmp = i1 + k;
        b_s_tmp = i3 + k;
        PMT_length += (frequency[s_tmp + 1] - frequency[s_tmp]) *
                      (InputSpectrum[b_s_tmp + 1] + InputSpectrum[b_s_tmp]) /
                      2.0;
      }
      channel_energy[b_i] = std::abs(PMT_length);
    }
    //  计算每个通道的中心频率
    channel_sign[b_i] =
        frequency[static_cast<int>(((static_cast<double>(b_i) + 1.0) - 1.0) *
                                       (channel_points + space_points) +
                                   std::floor(channel_points / 2.0)) -
                  1];
  }
}

//
// File trailer for PMTReceive.cpp
//
// [EOF]
//
