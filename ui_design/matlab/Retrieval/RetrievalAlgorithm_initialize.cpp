//
// File: RetrievalAlgorithm_initialize.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "RetrievalAlgorithm_initialize.h"
#include "RetrievalAlgorithm_data.h"
#include "matlab/Retrieval/eml_rand_mt19937ar_stateful_1.h"
#include "matlab/rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void RetrievalAlgorithm_initialize()
{
  eml_rand_mt19937ar_stateful_1_init();
  isInitialized_RetrievalAlgorithm = true;
}

//
// File trailer for RetrievalAlgorithm_initialize.cpp
//
// [EOF]
//
