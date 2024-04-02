//
// File: poissrnd.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "poissrnd.h"
#include "matlab/rand.h"
#include "matlab/randn.h"
#include "matlab/rt_nonfinite.h"
#include "matlab/coder_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const ::coder::array<double, 2U> &lambda
//                ::coder::array<double, 2U> &r
// Return Type  : void
//
namespace coder
{
  void poissrnd(const ::coder::array<double, 2U> &lambda,
                ::coder::array<double, 2U> &r)
  {
    int i;
    r.set_size(1, lambda.size(1));
    i = lambda.size(1);
    for (int k{0}; k < i; k++)
    {
      double lk;
      lk = lambda[k];
      if (lk >= 0.0)
      {
        if (std::isinf(lk))
        {
          r[k] = rtInf;
        }
        else if (lk >= 15.0)
        {
          r[k] = 0.0;
          double m;
          int exitg1;
          do
          {
            double d;
            double p;
            double x;
            int iter;
            exitg1 = 0;
            m = std::floor(0.875 * lk);
            if ((!std::isinf(m)) && (!std::isnan(m)))
            {
              double c;
              double u;
              double v;
              if (m >= 1.0)
              {
                d = m - 0.33333333333333331;
                u = b_rand();
                p = 1.0;
              }
              else
              {
                double ur[2];
                d = rtNaN;
                b_rand(ur);
                u = ur[0];
                p = rtNaN;
              }
              c = 1.0 / std::sqrt(9.0 * d);
              iter = 0;
              x = 0.0;
              int exitg2;
              do
              {
                exitg2 = 0;
                for (v = -1.0; v <= 0.0; v = c * x + 1.0)
                {
                  x = randn();
                }
                v *= v * v;
                x *= x;
                if ((u < 1.0 - 0.0331 * x * x) ||
                    (std::log(u) < 0.5 * x + d * ((1.0 - v) + std::log(v))))
                {
                  exitg2 = 1;
                }
                else
                {
                  iter++;
                  if (iter > 1000000)
                  {
                    exitg2 = 1;
                  }
                  else
                  {
                    u = b_rand();
                  }
                }
              } while (exitg2 == 0);
              x = d * v * p;
            }
            else
            {
              x = m;
            }
            if (lk < x)
            {
              if (m > 1.0)
              {
                p = lk / x;
                if (p <= 1.0)
                {
                  d = 0.0;
                  iter = static_cast<int>(m - 1.0);
                  for (int j{0}; j < iter; j++)
                  {
                    if (b_rand() < p)
                    {
                      d++;
                    }
                  }
                }
                else
                {
                  d = rtNaN;
                }
                r[k] = r[k] + d;
              }
              exitg1 = 1;
            }
            else
            {
              lk -= x;
              r[k] = r[k] + m;
              if (lk < 15.0)
              {
                d = 0.0;
                x = b_rand();
                x = std::log(x);
                for (p = -x; p < lk; p -= x)
                {
                  d++;
                  x = b_rand();
                  x = std::log(x);
                }
                r[k] = r[k] + d;
                exitg1 = 1;
              }
            }
          } while (exitg1 == 0);
        }
        else
        {
          double d;
          double x;
          d = 0.0;
          x = b_rand();
          x = std::log(x);
          for (double p{-x}; p < lambda[k]; p -= x)
          {
            d++;
            x = b_rand();
            x = std::log(x);
          }
          r[k] = d;
        }
      }
      else
      {
        r[k] = rtNaN;
      }
    }
  }

} // namespace coder

//
// File trailer for poissrnd.cpp
//
// [EOF]
//
