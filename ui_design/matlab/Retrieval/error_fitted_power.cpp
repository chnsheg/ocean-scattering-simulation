//
// File: error_fitted_power.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "error_fitted_power.h"
#include "matlab/rt_nonfinite.h"
#include "matlab/coder_array.h"

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                double in2
//                const coder::array<double, 2U> &in3
//                double in4
// Return Type  : void
//
void binary_expand_op(coder::array<double, 2U> &in1, double in2,
                      const coder::array<double, 2U> &in3, double in4)
{
  coder::array<double, 2U> b_in1;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  if (in3.size(1) == 1)
  {
    i = in1.size(1);
  }
  else
  {
    i = in3.size(1);
  }
  b_in1.set_size(1, i);
  stride_0_1 = (in1.size(1) != 1);
  stride_1_1 = (in3.size(1) != 1);
  if (in3.size(1) == 1)
  {
    loop_ub = in1.size(1);
  }
  else
  {
    loop_ub = in3.size(1);
  }
  for (i = 0; i < loop_ub; i++)
  {
    b_in1[i] = in1[i * stride_0_1] * in2 + in3[i * stride_1_1] * in4;
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
void minus(coder::array<double, 2U> &in1, const coder::array<double, 2U> &in2)
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
// File trailer for error_fitted_power.cpp
//
// [EOF]
//
