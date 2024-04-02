//
// File: linearLeastSquares.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "linearLeastSquares.h"
#include "matlab/rt_nonfinite.h"
#include "xnrm2.h"
#include "xzlarf.h"
#include "xzlarfg.h"
#include "matlab/coder_array.h"
#include <cmath>

// Function Definitions
//
// Arguments    : ::coder::array<double, 2U> &lhs
//                ::coder::array<double, 1U> &rhs
//                double dx[4]
//                int m
// Return Type  : void
//
namespace coder
{
  namespace optim
  {
    namespace coder
    {
      namespace levenbergMarquardt
      {
        void linearLeastSquares(::coder::array<double, 2U> &lhs,
                                ::coder::array<double, 1U> &rhs, double dx[4], int m)
        {
          double jpvt[4];
          double tau_data[4];
          double vn1[4];
          double temp;
          int b_i;
          int ii;
          int ix;
          int iy;
          int j;
          int ma;
          int mmi;
          int nfxd;
          int temp_tmp;
          ma = lhs.size(0);
          jpvt[0] = 0.0;
          tau_data[0] = 0.0;
          jpvt[1] = 0.0;
          tau_data[1] = 0.0;
          jpvt[2] = 0.0;
          tau_data[2] = 0.0;
          jpvt[3] = 0.0;
          tau_data[3] = 0.0;
          nfxd = -1;
          for (j = 0; j < 4; j++)
          {
            if (jpvt[j] != 0.0)
            {
              nfxd++;
              if (j + 1 != nfxd + 1)
              {
                ix = j * ma;
                iy = nfxd * ma;
                for (int k{0}; k < m; k++)
                {
                  temp_tmp = ix + k;
                  temp = lhs[temp_tmp];
                  b_i = iy + k;
                  lhs[temp_tmp] = lhs[b_i];
                  lhs[b_i] = temp;
                }
                jpvt[j] = jpvt[nfxd];
                jpvt[nfxd] = static_cast<double>(j) + 1.0;
              }
              else
              {
                jpvt[j] = static_cast<double>(j) + 1.0;
              }
            }
            else
            {
              jpvt[j] = static_cast<double>(j) + 1.0;
            }
            dx[j] = 0.0;
          }
          iy = lhs.size(0);
          for (int i{0}; i <= nfxd; i++)
          {
            ii = i * iy + i;
            mmi = m - i;
            if (i + 1 < m)
            {
              temp = lhs[ii];
              tau_data[i] = internal::reflapack::xzlarfg(mmi, &temp, lhs, ii + 2);
              lhs[ii] = temp;
            }
            else
            {
              tau_data[3] = 0.0;
            }
            if (i + 1 < 4)
            {
              temp = lhs[ii];
              lhs[ii] = 1.0;
              internal::reflapack::xzlarf(mmi, 3 - i, ii + 1, tau_data[i], lhs,
                                          (ii + iy) + 1, iy, dx);
              lhs[ii] = temp;
            }
          }
          if (nfxd + 1 < 4)
          {
            double vn2[4];
            ma = lhs.size(0);
            dx[0] = 0.0;
            vn1[0] = 0.0;
            vn2[0] = 0.0;
            dx[1] = 0.0;
            vn1[1] = 0.0;
            vn2[1] = 0.0;
            dx[2] = 0.0;
            vn1[2] = 0.0;
            vn2[2] = 0.0;
            dx[3] = 0.0;
            vn1[3] = 0.0;
            vn2[3] = 0.0;
            b_i = nfxd + 2;
            for (j = b_i; j < 5; j++)
            {
              temp =
                  internal::blas::xnrm2((m - nfxd) - 1, lhs, (nfxd + (j - 1) * ma) + 2);
              vn1[j - 1] = temp;
              vn2[j - 1] = temp;
            }
            for (int i{b_i}; i < 5; i++)
            {
              double s;
              int ip1;
              ip1 = i + 1;
              j = (i - 1) * ma;
              ii = (j + i) - 1;
              mmi = m - i;
              iy = 5 - i;
              nfxd = -1;
              if (5 - i > 1)
              {
                temp = std::abs(vn1[i - 1]);
                for (int k{2}; k <= iy; k++)
                {
                  s = std::abs(vn1[(i + k) - 2]);
                  if (s > temp)
                  {
                    nfxd = k - 2;
                    temp = s;
                  }
                }
              }
              iy = i + nfxd;
              if (iy + 1 != i)
              {
                ix = iy * ma;
                for (int k{0}; k < m; k++)
                {
                  temp_tmp = ix + k;
                  temp = lhs[temp_tmp];
                  nfxd = j + k;
                  lhs[temp_tmp] = lhs[nfxd];
                  lhs[nfxd] = temp;
                }
                temp = jpvt[iy];
                jpvt[iy] = jpvt[i - 1];
                jpvt[i - 1] = temp;
                vn1[iy] = vn1[i - 1];
                vn2[iy] = vn2[i - 1];
              }
              if (i < m)
              {
                temp = lhs[ii];
                tau_data[i - 1] =
                    internal::reflapack::xzlarfg(mmi + 1, &temp, lhs, ii + 2);
                lhs[ii] = temp;
              }
              else
              {
                tau_data[3] = 0.0;
              }
              if (i < 4)
              {
                temp = lhs[ii];
                lhs[ii] = 1.0;
                internal::reflapack::xzlarf(mmi + 1, 4 - i, ii + 1, tau_data[i - 1],
                                            lhs, (ii + ma) + 1, ma, dx);
                lhs[ii] = temp;
              }
              for (j = ip1; j < 5; j++)
              {
                iy = i + (j - 1) * ma;
                temp = vn1[j - 1];
                if (temp != 0.0)
                {
                  double temp2;
                  s = std::abs(lhs[iy - 1]) / temp;
                  s = 1.0 - s * s;
                  if (s < 0.0)
                  {
                    s = 0.0;
                  }
                  temp2 = temp / vn2[j - 1];
                  temp2 = s * (temp2 * temp2);
                  if (temp2 <= 1.4901161193847656E-8)
                  {
                    if (i < m)
                    {
                      temp = internal::blas::xnrm2(mmi, lhs, iy + 1);
                      vn1[j - 1] = temp;
                      vn2[j - 1] = temp;
                    }
                    else
                    {
                      vn1[j - 1] = 0.0;
                      vn2[j - 1] = 0.0;
                    }
                  }
                  else
                  {
                    vn1[j - 1] = temp * std::sqrt(s);
                  }
                }
              }
            }
          }
          iy = lhs.size(0);
          for (j = 0; j < 4; j++)
          {
            if (tau_data[j] != 0.0)
            {
              temp = rhs[j];
              b_i = j + 2;
              for (int i{b_i}; i <= iy; i++)
              {
                temp += lhs[(i + lhs.size(0) * j) - 1] * rhs[i - 1];
              }
              temp *= tau_data[j];
              if (temp != 0.0)
              {
                rhs[j] = rhs[j] - temp;
                for (int i{b_i}; i <= iy; i++)
                {
                  rhs[i - 1] = rhs[i - 1] - lhs[(i + lhs.size(0) * j) - 1] * temp;
                }
              }
            }
          }
          dx[0] = rhs[0];
          dx[1] = rhs[1];
          dx[2] = rhs[2];
          dx[3] = rhs[3];
          for (j = 3; j >= 0; j--)
          {
            iy = j + j * m;
            dx[j] /= lhs[iy];
            for (int i{0}; i < j; i++)
            {
              ix = (j - i) - 1;
              dx[ix] -= dx[j] * lhs[(iy - i) - 1];
            }
          }
          vn1[1] = dx[1];
          vn1[2] = dx[2];
          vn1[3] = dx[3];
          dx[static_cast<int>(jpvt[0]) - 1] = dx[0];
          dx[static_cast<int>(jpvt[1]) - 1] = vn1[1];
          dx[static_cast<int>(jpvt[2]) - 1] = vn1[2];
          dx[static_cast<int>(jpvt[3]) - 1] = vn1[3];
        }

      } // namespace levenbergMarquardt
    }   // namespace coder
  }     // namespace optim
} // namespace coder

//
// File trailer for linearLeastSquares.cpp
//
// [EOF]
//
