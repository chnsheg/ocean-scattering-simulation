//
// File: checkStoppingCriteria.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

// Include Files
#include "checkStoppingCriteria.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double gradf[3]
//                double relFactor    // StepTolerance
//                double funDiff      //FunctionTolerance
//                const double x[3]   // X
//                const double dx[3]  // DeltaX
//                int funcCount       // MaxFunctionEvaluations
//                boolean_T stepSuccessful
//                int *iter           // MaxIterations
//                double projSteepestDescentInfNorm // FiniteDifferenceStepSize
//                boolean_T hasFiniteBounds
// Return Type  : int
//
namespace coder
{
  namespace optim
  {
    namespace coder
    {
      namespace levenbergMarquardt
      {
        int b_checkStoppingCriteria(const double gradf[3], double relFactor,
                                    double funDiff, const double x[3],
                                    const double dx[3], int funcCount,
                                    boolean_T stepSuccessful, int *iter,
                                    double projSteepestDescentInfNorm,
                                    boolean_T hasFiniteBounds,
                                    double tolerance)
        {
          double absx;
          double normGradF;
          int exitflag;
          normGradF = 0.0;
          absx = std::abs(gradf[0]);
          if (std::isnan(absx) || (absx > 0.0))
          {
            normGradF = absx;
          }
          absx = std::abs(gradf[1]);
          if (std::isnan(absx) || (absx > normGradF))
          {
            normGradF = absx;
          }
          absx = std::abs(gradf[2]);
          if (std::isnan(absx) || (absx > normGradF))
          {
            normGradF = absx;
          }
          // if (hasFiniteBounds &&
          //     (projSteepestDescentInfNorm * projSteepestDescentInfNorm <=
          //      1.0E-24 * normGradF * relFactor))
          // {
          //   exitflag = 1;
          // }
          // else if ((!hasFiniteBounds) && (normGradF <= 1.0E-24 * relFactor))
          // {
          //   exitflag = 1;
          // }
          if (hasFiniteBounds &&
              (projSteepestDescentInfNorm * projSteepestDescentInfNorm <=
               tolerance * normGradF * relFactor))
          {
            exitflag = 1;
          }
          else if ((!hasFiniteBounds) && (normGradF <= tolerance * relFactor))
          {
            exitflag = 1;
          }
          else if (funcCount >= 2000)
          {
            exitflag = 0;
          }
          else if (*iter >= 2000)
          {
            exitflag = 0;
          }
          else
          {
            double absxk;
            double b_y;
            double t;
            double y;
            normGradF = 3.3121686421112381E-170;
            absx = 3.3121686421112381E-170;
            absxk = std::abs(dx[0]);
            if (absxk > 3.3121686421112381E-170)
            {
              y = 1.0;
              normGradF = absxk;
            }
            else
            {
              t = absxk / 3.3121686421112381E-170;
              y = t * t;
            }
            absxk = std::abs(x[0]);
            if (absxk > 3.3121686421112381E-170)
            {
              b_y = 1.0;
              absx = absxk;
            }
            else
            {
              t = absxk / 3.3121686421112381E-170;
              b_y = t * t;
            }
            absxk = std::abs(dx[1]);
            if (absxk > normGradF)
            {
              t = normGradF / absxk;
              y = y * t * t + 1.0;
              normGradF = absxk;
            }
            else
            {
              t = absxk / normGradF;
              y += t * t;
            }
            absxk = std::abs(x[1]);
            if (absxk > absx)
            {
              t = absx / absxk;
              b_y = b_y * t * t + 1.0;
              absx = absxk;
            }
            else
            {
              t = absxk / absx;
              b_y += t * t;
            }
            absxk = std::abs(dx[2]);
            if (absxk > normGradF)
            {
              t = normGradF / absxk;
              y = y * t * t + 1.0;
              normGradF = absxk;
            }
            else
            {
              t = absxk / normGradF;
              y += t * t;
            }
            absxk = std::abs(x[2]);
            if (absxk > absx)
            {
              t = absx / absxk;
              b_y = b_y * t * t + 1.0;
              absx = absxk;
            }
            else
            {
              t = absxk / absx;
              b_y += t * t;
            }
            y = normGradF * std::sqrt(y);
            b_y = absx * std::sqrt(b_y);
            if (y < 1.0E-20 * (b_y + 1.4901161193847656E-8))
            {
              exitflag = 4;
              if (!stepSuccessful)
              {
                (*iter)++;
              }
            }
            else if (funDiff <= 1.0E-20)
            {
              exitflag = 3;
            }
            else
            {
              exitflag = -5;
            }
          }
          return exitflag;
        }

        //
        // Arguments    : const double gradf[3]
        //                double relFactor
        //                int funcCount
        //                double projSteepestDescentInfNorm
        //                boolean_T hasFiniteBounds
        // Return Type  : int
        //
        int checkStoppingCriteria(const double gradf[3], double relFactor,
                                  int funcCount, double projSteepestDescentInfNorm,
                                  boolean_T hasFiniteBounds, double tolerance)
        {
          double absx;
          double normGradF;
          int exitflag;
          normGradF = 0.0;
          absx = std::abs(gradf[0]);
          if (std::isnan(absx) || (absx > 0.0))
          {
            normGradF = absx;
          }
          absx = std::abs(gradf[1]);
          if (std::isnan(absx) || (absx > normGradF))
          {
            normGradF = absx;
          }
          absx = std::abs(gradf[2]);
          if (std::isnan(absx) || (absx > normGradF))
          {
            normGradF = absx;
          }
          // if (hasFiniteBounds &&
          //     (projSteepestDescentInfNorm * projSteepestDescentInfNorm <=
          //      1.0E-24 * normGradF * relFactor))
          if (hasFiniteBounds &&
              (projSteepestDescentInfNorm * projSteepestDescentInfNorm <=
               tolerance * normGradF * relFactor))
          {
            exitflag = 1;
          }
          else if ((!hasFiniteBounds) && (normGradF <= tolerance * relFactor))
          {
            exitflag = 1;
          }
          else if (funcCount >= 2000)
          {
            exitflag = 0;
          }
          else
          {
            exitflag = -5;
          }
          return exitflag;
        }

      } // namespace levenbergMarquardt
    } // namespace coder
  } // namespace optim
} // namespace coder

//
// File trailer for checkStoppingCriteria.cpp
//
// [EOF]
//