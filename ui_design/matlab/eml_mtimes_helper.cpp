//
// File: eml_mtimes_helper.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 25-Mar-2024 11:13:21
//

// Include Files
#include "eml_mtimes_helper.h"
#include "coder_array.h"

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                const coder::array<double, 2U> &in2
//                const coder::array<double, 2U> &in3
//                const coder::array<double, 2U> &in4
// Return Type  : void
//
void binary_expand_op(coder::array<double, 2U> &in1,
                      const coder::array<double, 2U> &in2,
                      const coder::array<double, 2U> &in3,
                      const coder::array<double, 2U> &in4)
{
  coder::array<double, 2U> b_in3;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in4.size(1) == 1) {
    i = in3.size(1);
  } else {
    i = in4.size(1);
  }
  b_in3.set_size(1, i);
  stride_0_1 = (in3.size(1) != 1);
  stride_1_1 = (in4.size(1) != 1);
  if (in4.size(1) == 1) {
    loop_ub = in3.size(1);
  } else {
    loop_ub = in4.size(1);
  }
  for (i = 0; i < loop_ub; i++) {
    b_in3[i] = in3[i * stride_0_1] + in4[i * stride_1_1];
  }
  in1.set_size(in2.size(1), b_in3.size(1));
  loop_ub = b_in3.size(1);
  for (i = 0; i < loop_ub; i++) {
    stride_0_1 = in2.size(1);
    for (stride_1_1 = 0; stride_1_1 < stride_0_1; stride_1_1++) {
      in1[stride_1_1 + in1.size(0) * i] = in2[stride_1_1] * b_in3[i];
    }
  }
}

//
// File trailer for eml_mtimes_helper.cpp
//
// [EOF]
//
