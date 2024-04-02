//
// File: Retrieval_TS.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 02-Apr-2024 13:54:59
//

// Include Files
#include "Retrieval_TS.h"
#include "matlab/rt_nonfinite.h"
#include <cmath>

// Function Declarations
static double rt_powd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (std::isnan(u0) || std::isnan(u1))
  {
    y = rtNaN;
  }
  else
  {
    double d;
    double d1;
    d = std::abs(u0);
    d1 = std::abs(u1);
    if (std::isinf(u1))
    {
      if (d == 1.0)
      {
        y = 1.0;
      }
      else if (d > 1.0)
      {
        if (u1 > 0.0)
        {
          y = rtInf;
        }
        else
        {
          y = 0.0;
        }
      }
      else if (u1 > 0.0)
      {
        y = 0.0;
      }
      else
      {
        y = rtInf;
      }
    }
    else if (d1 == 0.0)
    {
      y = 1.0;
    }
    else if (d1 == 1.0)
    {
      if (u1 > 0.0)
      {
        y = u0;
      }
      else
      {
        y = 1.0 / u0;
      }
    }
    else if (u1 == 2.0)
    {
      y = u0 * u0;
    }
    else if ((u1 == 0.5) && (u0 >= 0.0))
    {
      y = std::sqrt(u0);
    }
    else if ((u0 < 0.0) && (u1 > std::floor(u1)))
    {
      y = rtNaN;
    }
    else
    {
      y = std::pow(u0, u1);
    }
  }
  return y;
}

//
// UNTITLED10 Summary of this function goes here
//    Detailed explanation goes here
//
// Arguments    : double B_Shift
//                double B_Width
//                double *Temperature
//                double *Sality
// Return Type  : void
//
void Retrieval_TS(double B_Shift, double B_Width, double *Temperature,
                  double *Sality)
{
  double Gb;
  double Temperature_tmp;
  double Vb;
  double b_Temperature_tmp;
  double dfg;
  Vb = B_Shift * 1.0E-9;
  Gb = B_Width * 1.0E-9;
  // -----------------------------------
  // ----------------------------
  // ---------------------------------------------------------------------------
  dfg = Gb * Gb;
  Temperature_tmp = Vb * Vb;
  b_Temperature_tmp = rt_powd_snf(Vb, 3.0);
  *Temperature =
      ((((((((-43.253 * Vb + 120.2) + 21.125 / Gb) + 3.4079 * Temperature_tmp) +
           -60.661 / dfg) +
          13.645 * Vb / Gb) +
         -0.0053609 * b_Temperature_tmp) +
        3.6607 / rt_powd_snf(Gb, 3.0)) +
       6.1565 * Vb / dfg) +
      -1.5792 * Temperature_tmp / Gb;
  // --------------------------------------------------------------------------
  dfg = std::log(Gb);
  *Sality = (((((((((-2.71176747E+7 / Vb + 718064.353) +
                    4.10067103E+8 / Temperature_tmp) +
                   -3.10104773E+9 / b_Temperature_tmp) +
                  1.1721699E+10 / rt_powd_snf(Vb, 4.0)) +
                 -1.7713023E+10 / rt_powd_snf(Vb, 5.0)) +
                42.8770135 * dfg) +
               -4.58265316 * (dfg * dfg)) +
              9.5134333 * rt_powd_snf(dfg, 3.0)) +
             -11.3828414 * rt_powd_snf(dfg, 4.0)) +
            1.44472609 * rt_powd_snf(dfg, 5.0);
}

//
// File trailer for Retrieval_TS.cpp
//
// [EOF]
//
