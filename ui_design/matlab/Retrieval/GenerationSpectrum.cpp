//
// File: GenerationSpectrum.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "GenerationSpectrum.h"
#include "matlab/rt_nonfinite.h"
#include "matlab/coder_array.h"

// Function Definitions
//
// Arguments    : coder::array<double, 2U> &in1
//                const double in2[4]
//                double in3
//                const coder::array<double, 2U> &in4
//                double in5
// Return Type  : void
//
void binary_expand_op(coder::array<double, 2U> &in1, const double in2[4],
                      double in3, const coder::array<double, 2U> &in4,
                      double in5)
{
  coder::array<double, 2U> d_in2;
  double b_in2;
  double c_in2;
  int i;
  int loop_ub;
  int stride_0_1;
  int stride_1_1;
  b_in2 = in2[1];
  c_in2 = in2[1];
  if (in4.size(1) == 1)
  {
    i = in1.size(1);
  }
  else
  {
    i = in4.size(1);
  }
  d_in2.set_size(1, i);
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
    d_in2[i] = b_in2 / (4.0 * in1[i * stride_0_1] + in3) +
               c_in2 / (4.0 * in4[i * stride_1_1] + in5);
  }
  in1.set_size(1, d_in2.size(1));
  loop_ub = d_in2.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    in1[i] = d_in2[i];
  }
}

//
// File trailer for GenerationSpectrum.cpp
//
// [EOF]
//
