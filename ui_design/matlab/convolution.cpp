//
// File: convolution.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 06-Mar-2024 14:46:49
//

// Include Files
#include "convolution.h"
#include "coder_array.h"

// Function Definitions
//
// 卷积函数
//
// Arguments    : const coder::array<double, 2U> &input
//                const coder::array<double, 2U> &kernel
//                coder::array<double, 2U> &L_out
// Return Type  : void
//
void convolution(const coder::array<double, 2U> &input,
                 const coder::array<double, 2U> &kernel,
                 coder::array<double, 2U> &L_out)
{
  int i;
  int iA0;
  int nA;
  int nB;
  nA = input.size(1) - 1;
  nB = kernel.size(1);
  L_out.set_size(1, input.size(1));
  iA0 = input.size(1);
  for (i = 0; i < iA0; i++) {
    L_out[i] = 0.0;
  }
  if ((input.size(1) > 0) && (kernel.size(1) > 0)) {
    int nBd2;
    nBd2 = kernel.size(1) >> 1;
    for (int k{0}; k < nBd2; k++) {
      iA0 = nBd2 - k;
      i = nA - iA0;
      for (int b_k{0}; b_k <= i; b_k++) {
        L_out[b_k] = L_out[b_k] + kernel[k] * input[iA0 + b_k];
      }
    }
    i = nBd2 + 1;
    for (int k{i}; k <= nB; k++) {
      int i1;
      iA0 = (k - nBd2) - 1;
      i1 = nA - iA0;
      for (int b_k{0}; b_k <= i1; b_k++) {
        int L_out_tmp;
        L_out_tmp = iA0 + b_k;
        L_out[L_out_tmp] = L_out[L_out_tmp] + kernel[k - 1] * input[b_k];
      }
    }
  }
}

//
// File trailer for convolution.cpp
//
// [EOF]
//
