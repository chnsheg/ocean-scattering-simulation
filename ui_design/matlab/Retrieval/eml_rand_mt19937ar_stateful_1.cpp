//
// File: eml_rand_mt19937ar_stateful.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "eml_rand_mt19937ar_stateful_1.h"
#include "RetrievalAlgorithm_data.h"
#include "matlab/rt_nonfinite.h"
#include <cstring>

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void eml_rand_mt19937ar_stateful_1_init()
{
  unsigned int r;
  std::memset(&state_1[0], 0, 625U * sizeof(unsigned int));
  r = 5489U;
  state_1[0] = 5489U;
  for (int mti{0}; mti < 623; mti++)
  {
    r = ((r ^ r >> 30U) * 1812433253U + mti) + 1U;
    state_1[mti + 1] = r;
  }
  state_1[624] = 624U;
}

//
// File trailer for eml_rand_mt19937ar_stateful.cpp
//
// [EOF]
//
