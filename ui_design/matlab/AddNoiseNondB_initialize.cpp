//
// File: AddNoiseNondB_initialize.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 22-Mar-2024 19:43:24
//

// Include Files
#include "AddNoiseNondB_initialize.h"
#include "AddNoiseNondB_data.h"
#include "eml_rand_mt19937ar_stateful.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void AddNoiseNondB_initialize()
{
  eml_rand_mt19937ar_stateful_init();
  isInitialized_AddNoiseNondB = true;
}

//
// File trailer for AddNoiseNondB_initialize.cpp
//
// [EOF]
//
