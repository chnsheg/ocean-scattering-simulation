//
// File: conv.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 01-Apr-2024 20:40:38
//

// Include Files
#include "conv.h"
#include "matlab/rt_nonfinite.h"
#include "matlab/coder_array.h"
#include "utils/myMath.h"

// Function Definitions
//
// Arguments    : const ::coder::array<double, 2U> &A
//                const ::coder::array<double, 2U> &B
//                ::coder::array<double, 2U> &C
// Return Type  : void
//
namespace coder
{
  // QVector<double> *MyMath::convolution(QVector<double> *input, QVector<double> *kernel)
  void conv(const ::coder::array<double, 2U> &A,
            const ::coder::array<double, 2U> &B, ::coder::array<double, 2U> &C)
  {
    // int i;
    // int iA0;
    // int nA;
    // int nB;
    // nA = A.size(1) - 1;
    // nB = B.size(1);
    // C.set_size(1, A.size(1));
    // iA0 = A.size(1);
    // for (i = 0; i < iA0; i++)
    // {
    //   C[i] = 0.0;
    // }
    // if ((A.size(1) > 0) && (B.size(1) > 0))
    // {
    //   int nBd2;
    //   nBd2 = B.size(1) >> 1;
    //   for (int k{0}; k < nBd2; k++)
    //   {
    //     iA0 = nBd2 - k;
    //     i = nA - iA0;
    //     for (int b_k{0}; b_k <= i; b_k++)
    //     {
    //       C[b_k] = C[b_k] + B[k] * A[iA0 + b_k];
    //     }
    //   }
    //   i = nBd2 + 1;
    //   for (int k{i}; k <= nB; k++)
    //   {
    //     int i1;
    //     iA0 = (k - nBd2) - 1;
    //     i1 = nA - iA0;
    //     for (int b_k{0}; b_k <= i1; b_k++)
    //     {
    //       int C_tmp;
    //       C_tmp = iA0 + b_k;
    //       C[C_tmp] = C[C_tmp] + B[k - 1] * A[b_k];
    //     }
    //   }
    // }

    QVector<double> *A_input = MyMath::convertArrayToQVector(A);
    QVector<double> *B_kernel = MyMath::convertArrayToQVector(B);
    QVector<double> *C_output = MyMath::convolution(A_input, B_kernel);
    MyMath::convertQVectorToArray(C_output, C);
    delete A_input;
    delete B_kernel;
    delete C_output;
  }

} // namespace coder

//
// File trailer for conv.cpp
//
// [EOF]
//
