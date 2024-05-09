//
// File: RetrievalAlgorithm.cpp
//
// MATLAB Coder version            : 5.4
// C/C++ source code generated on  : 08-Apr-2024 10:38:18
//

// Include Files
#include "RetrievalAlgorithm.h"
#include "GenerationSpectrum.h"
#include "PMTFMethods.h"
#include "RetrievalAlgorithm_internal_types.h"
#include "RetrievalAlgorithm_internal_types1.h"
#include "RetrievalAlgorithm_internal_types11.h"
#include "RetrievalAlgorithm_types.h"
#include "anonymous_function.h"
#include "anonymous_function1.h"
#include "checkStoppingCriteria.h"
#include "computeFiniteDifferences.h"
#include "conv.h"
#include "driver.h"
#include "error_fitted_power.h"
#include "factoryConstruct.h"
#include "hasFiniteBounds.h"
#include "linearLeastSquares.h"
#include "minOrMax.h"
#include "projectBox.h"
#include "rt_nonfinite.h"
#include "xgemv.h"
#include "coder_array.h"
#include <algorithm>
#include <cmath>
#include <QDebug>

// Function Definitions
//
// UNTITLED5 Summary of this function goes here
//    Detailed explanation goes here
//     %% Global Veriable
//
// Arguments    : double number
//                const coder::array<double, 2U> &PMT_energy
//                const coder::array<double, 2U> &Fizeau_spectrum
//                const double Initial_lower[3]
//                const double Initial_upper[3]
//                const double Initial_value[3]
//                const double params[19]
//                double fitted_value[3]
//                double *resnorm
//                coder::array<double, 2U> &residual
//                double *exitflag
//                struct0_T *output
//                struct1_T *lambda
//                coder::array<double, 2U> &jacobia
// Return Type  : void
//
void RetrievalAlgorithm(double number,
                        const coder::array<double, 2U> &PMT_energy,
                        const coder::array<double, 2U> &Fizeau_spectrum,
                        const double Initial_lower[3],
                        const double Initial_upper[3],
                        const double Initial_value[3], const double params[19],
                        double fitted_value[3], double *resnorm,
                        coder::array<double, 2U> &residual, double *exitflag,
                        struct0_T *output, struct1_T *lambda,
                        coder::array<double, 2U> &jacobia)
{
  static const char cv[19]{'l', 'e', 'v', 'e', 'n', 'b', 'e', 'r', 'g', '-',
                           'm', 'a', 'r', 'q', 'u', 'a', 'r', 'd', 't'};
  coder::b_anonymous_function b_this;
  coder::array<double, 2U> JacCeqTrans;
  coder::array<double, 2U> augJacobian;
  coder::array<double, 2U> fNew;
  coder::array<double, 2U> f_temp;
  coder::array<double, 1U> rhs;
  c_struct_T FiniteDifferences;
  c_struct_T b_FiniteDifferences;
  double b_Initial_upper[3];
  double dx[3];
  double gradf[3];
  double xp[3];
  double b_gamma;
  double funDiff;
  double minWidth;
  double relFactor;
  double resnormNew;
  double t;
  double tolActive;
  int aIdx;
  int bIdx;
  int funcCount;
  int i;
  int iter;
  int m;
  int m_temp;
  boolean_T hasLB[3];
  boolean_T hasUB[3];
  boolean_T indActive[3];
  boolean_T exitg1;
  boolean_T hasFiniteBounds;
  boolean_T stepSuccessful;
  //     %% Initial value
  // [Bri_shift Bri_width Ray_width Mie_width]
  //     %% Set the algorithm parameters
  // options = optimset('lsqnonlin'); % Set Fit to Nonlinear Fit
  // 使用最小二乘法进行拟合 设置了 Algorithm 选项为
  // 'trust-region-reflective'。这是一种用于非线性最小二乘法的优化算法，它适用于大部分情况，特别是当问题中存在边界约束时。
  // options = optimset(options, 'Algorithm', 'trust-region-reflective');
  //  domain algorithm and set the initial damping coefficient in the algorithm
  // options = optimoptions('lsqnonlin','Algorithm','levenberg-marquardt');
  // options = optimset(options, 'TolFun', 1e-20); % �涨��С���仯��   TolFun
  // 是函数值变化的容忍度 TolX
  // 是参数值变化的容忍度。当函数值或参数值的变化小于这些容忍度时，算法将停止迭代。
  // options = optimset(options, 'TolX', 1e-20); % �涨��С���������仯�
  //  options = optimoptions('lsqnonlin', 'Algorithm',
  //  'trust-region-reflective', 'MaxFunctionEvaluations', 2000,
  //  'MaxIterations', 2000, 'TolFun', 1e-20, 'TolX', 1e-20, 'Display', 'iter');
  // options.ScaleProblem = 'Jacobian'; % 设置 ScaleProblem 选项为
  // 'Jacobian'，这是一种用于非线性最小二乘法的优化算法 options.Algorithm =
  // 'levenberg-marquardt'; % 设置 Algorithm 选项为
  // 'trust-region-reflective'，这是一种用于非线性最小二乘法的优化算法
  //  options = optimoptions('lsqnonlin', 'Algorithm',
  //  'trust-region-reflective', 'MaxFunctionEvaluations', 2000,
  //  'MaxIterations', 2000, 'FunctionTolerance', 1e-20, 'StepTolerance', 1e-19,
  //  'OptimalityTolerance', 1e-19, 'Display', 'iter'); options =
  //  optimoptions('lsqnonlin'); % 创建 lsqnonlin 优化器选项对象 % 设置
  //  Algorithm 选项为
  //  'trust-region-reflective'，这是一种用于非线性最小二乘法的优化算法
  //  options.Algorithm = 'trust-region-reflective';
  //  % 设置 TolFun 选项为 1e-20，表示函数值变化的容忍度
  //  options.TolFun = 1e-20;
  //  % 设置 TolX 选项为 1e-20，表示参数值变化的容忍度
  //  options.TolX = 1e-20;
  //     %% Curve fitting
  //  [fitted_value, resnorm, residual, exitflag, output, lambda, jacobia] = ...
  //      lsqnonlin(@error_fitted_power, Initial_value, Initial_lower,
  //      Initial_upper, options);
  //  修改lsqnonlin调用以传递新增参数,包括MeasurementEnergy、ChannelNumber，同时接收产生的误差
  b_this.workspace.fun.workspace.ChannelNumber = number;
  b_this.workspace.fun.workspace.MeasurementEnergy.set_size(1,
                                                            PMT_energy.size(1));
  aIdx = PMT_energy.size(1);
  for (i = 0; i < aIdx; i++)
  {
    b_this.workspace.fun.workspace.MeasurementEnergy[i] = PMT_energy[i];
  }
  b_this.workspace.fun.workspace.Fizeau_spectrum.set_size(
      1, Fizeau_spectrum.size(1));
  aIdx = Fizeau_spectrum.size(1);
  for (i = 0; i < aIdx; i++)
  {
    b_this.workspace.fun.workspace.Fizeau_spectrum[i] = Fizeau_spectrum[i];
  }
  std::copy(&params[0], &params[19], &b_this.workspace.fun.workspace.params[0]);
  funDiff = rtInf;
  iter = 0;
  indActive[0] = false;
  dx[0] = rtInf;
  b_Initial_upper[0] = Initial_upper[0] - Initial_lower[0];
  indActive[1] = false;
  dx[1] = rtInf;
  b_Initial_upper[1] = Initial_upper[1] - Initial_lower[1];
  indActive[2] = false;
  dx[2] = rtInf;
  b_Initial_upper[2] = Initial_upper[2] - Initial_lower[2];
  minWidth = coder::internal::minimum(b_Initial_upper);
  hasFiniteBounds = coder::optim::coder::utils::hasFiniteBounds(
      hasLB, hasUB, Initial_lower, Initial_upper);
  if ((!hasFiniteBounds) || (minWidth < 0.0))
  {
    fitted_value[0] = Initial_value[0];
    fitted_value[1] = Initial_value[1];
    fitted_value[2] = Initial_value[2];
  }
  else
  {
    tolActive = Initial_value[0];
    fitted_value[0] = Initial_value[0];
    if (hasLB[0])
    {
      tolActive = std::fmax(Initial_lower[0], Initial_value[0]);
      fitted_value[0] = tolActive;
    }
    if (hasUB[0])
    {
      tolActive = std::fmin(Initial_upper[0], tolActive);
      fitted_value[0] = tolActive;
    }
    tolActive = Initial_value[1];
    fitted_value[1] = Initial_value[1];
    if (hasLB[1])
    {
      tolActive = std::fmax(Initial_lower[1], Initial_value[1]);
      fitted_value[1] = tolActive;
    }
    if (hasUB[1])
    {
      tolActive = std::fmin(Initial_upper[1], tolActive);
      fitted_value[1] = tolActive;
    }
    tolActive = Initial_value[2];
    fitted_value[2] = Initial_value[2];
    if (hasLB[2])
    {
      tolActive = std::fmax(Initial_lower[2], Initial_value[2]);
      fitted_value[2] = tolActive;
    }
    if (hasUB[2])
    {
      tolActive = std::fmin(Initial_upper[2], tolActive);
      fitted_value[2] = tolActive;
    }
  }
  RetrievalAlgorithm_anonFcn1(PMT_energy, number, Fizeau_spectrum, params,
                              fitted_value, f_temp);
  m_temp = f_temp.size(1);
  jacobia.set_size(f_temp.size(1), 3);
  m = f_temp.size(1) - 1;
  residual.set_size(1, f_temp.size(1));
  fNew.set_size(1, f_temp.size(1));
  for (int b_i{0}; b_i <= m; b_i++)
  {
    residual[b_i] = f_temp[b_i];
  }
  augJacobian.set_size(f_temp.size(1) + 3, 3);
  rhs.set_size(f_temp.size(1) + 3);
  i = f_temp.size(1) - 1;
  for (int j{0}; j < 3; j++)
  {
    aIdx = j * m_temp;
    bIdx = j * (m + 4);
    for (int b_i{0}; b_i <= i; b_i++)
    {
      augJacobian[bIdx + b_i] = jacobia[aIdx + b_i];
    }
  }
  *resnorm = 0.0;
  if (f_temp.size(1) >= 1)
  {
    aIdx = f_temp.size(1);
    for (int j{0}; j < aIdx; j++)
    {
      tolActive = residual[j];
      *resnorm += tolActive * tolActive;
    }
  }
  JacCeqTrans.set_size(3, residual.size(1));
  coder::optim::coder::utils::FiniteDifferences::factoryConstruct(
      &b_this, f_temp.size(1), Initial_lower, Initial_upper,
      &FiniteDifferences);
  b_Initial_upper[0] = fitted_value[0];
  b_Initial_upper[1] = fitted_value[1];
  b_Initial_upper[2] = fitted_value[2];
  coder::optim::coder::utils::FiniteDifferences::computeFiniteDifferences(
      &FiniteDifferences, residual, b_Initial_upper, JacCeqTrans, Initial_lower,
      Initial_upper);
  aIdx = JacCeqTrans.size(1);
  for (i = 0; i < 3; i++)
  {
    for (bIdx = 0; bIdx < aIdx; bIdx++)
    {
      augJacobian[bIdx + augJacobian.size(0) * i] = JacCeqTrans[i + 3 * bIdx];
    }
  }
  funcCount = FiniteDifferences.numEvals + 1;
  // b_gamma = 1.0E-100; // InitDamping
  b_gamma = 1.0E-30;
  for (int b_i{0}; b_i < 3; b_i++)
  {
    aIdx = (m_temp + 3) * (b_i + 1);
    augJacobian[aIdx - 3] = 0.0;
    augJacobian[aIdx - 2] = 0.0;
    augJacobian[aIdx - 1] = 0.0;
    augJacobian[(m_temp + b_i) + augJacobian.size(0) * b_i] = 1.0E-50;
  }
  i = f_temp.size(1) - 1;
  for (int j{0}; j < 3; j++)
  {
    aIdx = j * (m + 4);
    bIdx = j * m_temp;
    for (int b_i{0}; b_i <= i; b_i++)
    {
      jacobia[bIdx + b_i] = augJacobian[aIdx + b_i];
    }
  }
  coder::internal::blas::xgemv(f_temp.size(1), jacobia, f_temp.size(1),
                               residual, gradf);
  b_Initial_upper[0] = -gradf[0];
  b_Initial_upper[1] = -gradf[1];
  b_Initial_upper[2] = -gradf[2];
  t = coder::optim::coder::levenbergMarquardt::projectBox(
      fitted_value, b_Initial_upper, Initial_lower, Initial_upper, hasLB,
      hasUB);
  tolActive = coder::optim::coder::levenbergMarquardt::computeFirstOrderOpt(
      gradf, hasFiniteBounds, &t);
  relFactor = std::fmax(tolActive, 1.0);
  stepSuccessful = true;
  if (minWidth < 0.0)
  {
    bIdx = -2;
  }
  else
  {
    bIdx = coder::optim::coder::levenbergMarquardt::checkStoppingCriteria(
        gradf, relFactor, FiniteDifferences.numEvals + 1, t, hasFiniteBounds);
  }
  exitg1 = false;
  while ((!exitg1) && (bIdx == -5))
  {
    boolean_T evalOK;
    boolean_T guard1{false};
    f_temp.set_size(1, residual.size(1));
    aIdx = residual.size(1);
    for (i = 0; i < aIdx; i++)
    {
      f_temp[i] = -residual[i];
    }
    for (int j{0}; j <= m; j++)
    {
      rhs[j] = f_temp[j];
    }
    rhs[m + 1] = 0.0;
    rhs[m + 2] = 0.0;
    rhs[m + 3] = 0.0;
    if (hasFiniteBounds)
    {
      b_Initial_upper[0] = -gradf[0] / (b_gamma + 1.0);
      b_Initial_upper[1] = -gradf[1] / (b_gamma + 1.0);
      b_Initial_upper[2] = -gradf[2] / (b_gamma + 1.0);
      t = coder::optim::coder::levenbergMarquardt::projectBox(
          fitted_value, b_Initial_upper, Initial_lower, Initial_upper, hasLB,
          hasUB);
      tolActive = std::fmin(t, minWidth / 2.0);
      for (int b_i{0}; b_i < 3; b_i++)
      {
        if (hasLB[b_i])
        {
          if ((fitted_value[b_i] - Initial_lower[b_i] <= tolActive) &&
              (gradf[b_i] > 0.0))
          {
            indActive[b_i] = true;
          }
          else
          {
            indActive[b_i] = false;
          }
        }
        if (hasUB[b_i])
        {
          if (indActive[b_i] ||
              ((Initial_upper[b_i] - fitted_value[b_i] <= tolActive) &&
               (gradf[b_i] < 0.0)))
          {
            indActive[b_i] = true;
          }
          else
          {
            indActive[b_i] = false;
          }
        }
        if (indActive[b_i])
        {
          aIdx = (m_temp + 3) * b_i;
          for (int j{0}; j <= m; j++)
          {
            augJacobian[aIdx + j] = 0.0;
          }
        }
      }
    }
    coder::optim::coder::levenbergMarquardt::linearLeastSquares(
        augJacobian, rhs, dx, m_temp + 3);
    if (hasFiniteBounds)
    {
      if (indActive[0])
      {
        dx[0] = -gradf[0] / (b_gamma + 1.0);
      }
      tolActive = fitted_value[0] + dx[0];
      xp[0] = tolActive;
      if (hasLB[0])
      {
        tolActive = std::fmax(Initial_lower[0], tolActive);
        xp[0] = tolActive;
      }
      if (hasUB[0])
      {
        tolActive = std::fmin(Initial_upper[0], tolActive);
        xp[0] = tolActive;
      }
      if (indActive[1])
      {
        dx[1] = -gradf[1] / (b_gamma + 1.0);
      }
      tolActive = fitted_value[1] + dx[1];
      xp[1] = tolActive;
      if (hasLB[1])
      {
        tolActive = std::fmax(Initial_lower[1], tolActive);
        xp[1] = tolActive;
      }
      if (hasUB[1])
      {
        tolActive = std::fmin(Initial_upper[1], tolActive);
        xp[1] = tolActive;
      }
      if (indActive[2])
      {
        dx[2] = -gradf[2] / (b_gamma + 1.0);
      }
      tolActive = fitted_value[2] + dx[2];
      xp[2] = tolActive;
      if (hasLB[2])
      {
        tolActive = std::fmax(Initial_lower[2], tolActive);
        xp[2] = tolActive;
      }
      if (hasUB[2])
      {
        tolActive = std::fmin(Initial_upper[2], tolActive);
        xp[2] = tolActive;
      }
    }
    else
    {
      xp[0] = fitted_value[0] + dx[0];
      xp[1] = fitted_value[1] + dx[1];
      xp[2] = fitted_value[2] + dx[2];
    }
    RetrievalAlgorithm_anonFcn1(PMT_energy, number, Fizeau_spectrum, params, xp,
                                f_temp);
    for (int b_i{0}; b_i <= m; b_i++)
    {
      fNew[b_i] = f_temp[b_i];
    }
    resnormNew = 0.0;
    if (m_temp >= 1)
    {
      for (int j{0}; j <= m; j++)
      {
        tolActive = fNew[j];
        resnormNew += tolActive * tolActive;
      }
    }
    evalOK = true;
    for (int b_i{0}; b_i < m_temp; b_i++)
    {
      if (evalOK)
      {
        tolActive = fNew[b_i];
        if (std::isinf(tolActive) || std::isnan(tolActive))
        {
          evalOK = false;
        }
      }
      else
      {
        evalOK = false;
      }
    }
    funcCount++;
    guard1 = false;
    if ((resnormNew < *resnorm) && evalOK)
    {
      iter++;
      funDiff = std::abs(resnormNew - *resnorm) / *resnorm; // 计算相对残差
      *resnorm = resnormNew;
      residual.set_size(1, fNew.size(1));
      aIdx = fNew.size(1);
      for (i = 0; i < aIdx; i++)
      {
        residual[i] = fNew[i];
      }
      JacCeqTrans.set_size(3, fNew.size(1));
      b_Initial_upper[0] = xp[0];
      b_Initial_upper[1] = xp[1];
      b_Initial_upper[2] = xp[2];
      b_FiniteDifferences = FiniteDifferences;
      evalOK = coder::optim::coder::utils::FiniteDifferences::
          computeFiniteDifferences(&b_FiniteDifferences, fNew, b_Initial_upper,
                                   JacCeqTrans, Initial_lower, Initial_upper);
      funcCount += b_FiniteDifferences.numEvals;
      aIdx = JacCeqTrans.size(1);
      for (i = 0; i < 3; i++)
      {
        for (bIdx = 0; bIdx < aIdx; bIdx++)
        {
          augJacobian[bIdx + augJacobian.size(0) * i] =
              JacCeqTrans[i + 3 * bIdx];
        }
      }
      i = m_temp - 1;
      for (int j{0}; j < 3; j++)
      {
        aIdx = j * (m + 4);
        bIdx = j * m_temp;
        for (int b_i{0}; b_i <= i; b_i++)
        {
          jacobia[bIdx + b_i] = augJacobian[aIdx + b_i];
        }
      }
      if (evalOK)
      {
        fitted_value[0] = xp[0];
        fitted_value[1] = xp[1];
        fitted_value[2] = xp[2];
        if (stepSuccessful)
        {
          b_gamma *= 0.1;
        }
        stepSuccessful = true;
        guard1 = true;
      }
      else
      {
        bIdx = 2;
        aIdx = m_temp * 3;
        for (int j{0}; j < aIdx; j++)
        {
          jacobia[j] = rtNaN;
        }
        exitg1 = true;
      }
    }
    else
    {
      b_gamma *= 10.0;
      stepSuccessful = false;
      i = m_temp - 1;
      for (int j{0}; j < 3; j++)
      {
        aIdx = j * m_temp;
        bIdx = j * (m + 4);
        for (int b_i{0}; b_i <= i; b_i++)
        {
          augJacobian[bIdx + b_i] = jacobia[aIdx + b_i];
        }
      }
      guard1 = true;
    }
    if (guard1)
    {
      tolActive = std::sqrt(b_gamma);
      for (int b_i{0}; b_i < 3; b_i++)
      {
        aIdx = (m_temp + 3) * (b_i + 1);
        augJacobian[aIdx - 3] = 0.0;
        augJacobian[aIdx - 2] = 0.0;
        augJacobian[aIdx - 1] = 0.0;
        augJacobian[(m_temp + b_i) + augJacobian.size(0) * b_i] = tolActive;
      }
      coder::internal::blas::xgemv(m_temp, jacobia, m_temp, residual, gradf);
      b_Initial_upper[0] = -gradf[0];
      b_Initial_upper[1] = -gradf[1];
      b_Initial_upper[2] = -gradf[2];
      t = coder::optim::coder::levenbergMarquardt::projectBox(
          fitted_value, b_Initial_upper, Initial_lower, Initial_upper, hasLB,
          hasUB);
      bIdx = coder::optim::coder::levenbergMarquardt::b_checkStoppingCriteria(
          gradf, relFactor, funDiff, fitted_value, dx, funcCount,
          stepSuccessful, &iter, t, hasFiniteBounds);

      // 打印迭代次数和funDiff
      qDebug() << "iter: " << iter << "resnorm: " << resnormNew << "funDiff: " << funDiff;

      if (bIdx != -5)
      {
        exitg1 = true;
      }
    }
  }
  output->firstorderopt =
      coder::optim::coder::levenbergMarquardt::computeFirstOrderOpt(
          gradf, hasFiniteBounds, &t);
  output->iterations = iter;
  output->funcCount = funcCount;
  tolActive = 3.3121686421112381E-170;
  resnormNew = std::abs(dx[0]);
  if (resnormNew > 3.3121686421112381E-170)
  {
    funDiff = 1.0;
    tolActive = resnormNew;
  }
  else
  {
    t = resnormNew / 3.3121686421112381E-170;
    funDiff = t * t;
  }
  resnormNew = std::abs(dx[1]);
  if (resnormNew > tolActive)
  {
    t = tolActive / resnormNew;
    funDiff = funDiff * t * t + 1.0;
    tolActive = resnormNew;
  }
  else
  {
    t = resnormNew / tolActive;
    funDiff += t * t;
  }
  resnormNew = std::abs(dx[2]);
  if (resnormNew > tolActive)
  {
    t = tolActive / resnormNew;
    funDiff = funDiff * t * t + 1.0;
    tolActive = resnormNew;
  }
  else
  {
    t = resnormNew / tolActive;
    funDiff += t * t;
  }
  output->stepsize = tolActive * std::sqrt(funDiff);
  for (i = 0; i < 19; i++)
  {
    output->algorithm[i] = cv[i];
  }
  lambda->lower[0] = 0.0;
  lambda->upper[0] = 0.0;
  lambda->lower[1] = 0.0;
  lambda->upper[1] = 0.0;
  lambda->lower[2] = 0.0;
  lambda->upper[2] = 0.0;
  if (hasFiniteBounds)
  {
    b_Initial_upper[0] = -gradf[0] / (b_gamma + 1.0);
    b_Initial_upper[1] = -gradf[1] / (b_gamma + 1.0);
    b_Initial_upper[2] = -gradf[2] / (b_gamma + 1.0);
    tolActive = coder::optim::coder::levenbergMarquardt::projectBox(
        fitted_value, b_Initial_upper, Initial_lower, Initial_upper, hasLB,
        hasUB);
    tolActive = std::fmin(tolActive, minWidth / 2.0);
    if (hasLB[0] && (fitted_value[0] - Initial_lower[0] <= tolActive) &&
        (gradf[0] > 0.0))
    {
      lambda->lower[0] = 2.0 * gradf[0];
    }
    if (hasUB[0] && (Initial_upper[0] - fitted_value[0] <= tolActive) &&
        (gradf[0] < 0.0))
    {
      lambda->upper[0] = -2.0 * gradf[0];
    }
    if (hasLB[1] && (fitted_value[1] - Initial_lower[1] <= tolActive) &&
        (gradf[1] > 0.0))
    {
      lambda->lower[1] = 2.0 * gradf[1];
    }
    if (hasUB[1] && (Initial_upper[1] - fitted_value[1] <= tolActive) &&
        (gradf[1] < 0.0))
    {
      lambda->upper[1] = -2.0 * gradf[1];
    }
    if (hasLB[2] && (fitted_value[2] - Initial_lower[2] <= tolActive) &&
        (gradf[2] > 0.0))
    {
      lambda->lower[2] = 2.0 * gradf[2];
    }
    if (hasUB[2] && (Initial_upper[2] - fitted_value[2] <= tolActive) &&
        (gradf[2] < 0.0))
    {
      lambda->upper[2] = -2.0 * gradf[2];
    }
  }
  *exitflag = bIdx;
}

//
// Arguments    : const coder::array<double, 2U> &MeasurementEnergy
//                double ChannelNumber
//                const coder::array<double, 2U> &Fizeau_spectrum
//                const double params[19]
//                const double Initial_value[3]
//                coder::array<double, 2U> &varargout_1
// Return Type  : void
//
void RetrievalAlgorithm_anonFcn1(
    const coder::array<double, 2U> &MeasurementEnergy, double ChannelNumber,
    const coder::array<double, 2U> &Fizeau_spectrum, const double params[19],
    const double Initial_value[3], coder::array<double, 2U> &varargout_1)
{
  coder::array<double, 2U> L_b;
  coder::array<double, 2U> L_r;
  coder::array<double, 2U> RF;
  coder::array<double, 2U> b_L_r;
  coder::array<double, 2U> col_LS;
  coder::array<double, 2U> fx;
  coder::array<double, 2U> miu;
  coder::array<double, 1U> c;
  double B;
  double a_tmp;
  double apnd;
  double b_tmp;
  double cdiff;
  double miu_0;
  double ndbl;
  double vb2;
  int i;
  int idx;
  int k;
  int loop_ub;
  int nm1d2;
  // UNTITLED3 Summary of this function goes here
  //    Detailed explanation goes here
  //     %% Global Variable
  // global ChannelNumber MeasurementEnergy
  //     %% Systerm Parameters
  // the speed of light??
  // lambda_0 = 532e-9; %Laser wavelength?
  miu_0 = 2.99792458E+8 / params[0];
  // Laser center frequency
  a_tmp = miu_0 - params[1];
  b_tmp = miu_0 + params[1];
  if (std::isnan(a_tmp) || std::isnan(b_tmp))
  {
    miu.set_size(1, 1);
    miu[0] = rtNaN;
  }
  else if (b_tmp < a_tmp)
  {
    miu.set_size(1, 0);
  }
  else if ((std::isinf(a_tmp) || std::isinf(b_tmp)) && (a_tmp == b_tmp))
  {
    miu.set_size(1, 1);
    miu[0] = rtNaN;
  }
  else if (std::floor(a_tmp) == a_tmp)
  {
    loop_ub = static_cast<int>((b_tmp - a_tmp) / 1.0E+6);
    miu.set_size(1, loop_ub + 1);
    for (i = 0; i <= loop_ub; i++)
    {
      miu[i] = a_tmp + 1.0E+6 * static_cast<double>(i);
    }
  }
  else
  {
    ndbl = std::floor((b_tmp - a_tmp) / 1.0E+6 + 0.5);
    apnd = a_tmp + ndbl * 1.0E+6;
    cdiff = apnd - b_tmp;
    if (std::abs(cdiff) <
        4.4408920985006262E-16 * std::fmax(std::abs(a_tmp), std::abs(b_tmp)))
    {
      ndbl++;
      apnd = b_tmp;
    }
    else if (cdiff > 0.0)
    {
      apnd = a_tmp + (ndbl - 1.0) * 1.0E+6;
    }
    else
    {
      ndbl++;
    }
    if (ndbl >= 0.0)
    {
      idx = static_cast<int>(ndbl);
    }
    else
    {
      idx = 0;
    }
    miu.set_size(1, idx);
    if (idx > 0)
    {
      miu[0] = a_tmp;
      if (idx > 1)
      {
        miu[idx - 1] = apnd;
        nm1d2 = (idx - 1) / 2;
        for (k = 0; k <= nm1d2 - 2; k++)
        {
          cdiff = (static_cast<double>(k) + 1.0) * 1.0E+6;
          miu[k + 1] = a_tmp + cdiff;
          miu[(idx - k) - 2] = apnd - cdiff;
        }
        if (nm1d2 << 1 == idx - 1)
        {
          miu[nm1d2] = (a_tmp + apnd) / 2.0;
        }
        else
        {
          cdiff = static_cast<double>(nm1d2) * 1.0E+6;
          miu[nm1d2] = a_tmp + cdiff;
          miu[nm1d2 + 1] = apnd - cdiff;
        }
      }
    }
  }
  // Frequency ranges
  RF.set_size(1, miu.size(1));
  loop_ub = miu.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    RF[i] = (miu[i] - miu_0) / 1.0E+9;
  }
  // Relative frequency
  //     %% Scattering Spectrum
  // Width_Mie = Initial_value(:, 4);
  //  fprintf('%8.6e %8.6e %8.6e %8.6e \n', Shift_Bri, Width_Bri, Width_Ray,
  //  Width_Mie);
  // UNTITLED5 Summary of this function goes here
  //    Detailed explanation goes here
  //
  apnd = miu_0 - Initial_value[0];
  //
  vb2 = miu_0 + Initial_value[0];
  //
  ndbl = Initial_value[2] * Initial_value[2];
  L_r.set_size(1, miu.size(1));
  loop_ub = miu.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    cdiff = miu[i] - miu_0;
    L_r[i] = cdiff * cdiff;
  }
  L_r.set_size(1, L_r.size(1));
  loop_ub = L_r.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    L_r[i] = Initial_value[2] / (4.0 * L_r[i] + ndbl);
  }
  //
  if (miu.size(1) == 0)
  {
    B = 0.0;
  }
  else
  {
    ndbl =
        (miu[0] - miu[miu.size(1) - 1]) * (L_r[0] + L_r[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (k = 0; k <= i - 2; k++)
    {
      ndbl += (miu[k + 1] - miu[k]) * (L_r[k + 1] + L_r[k]) / 2.0;
    }
    B = std::abs(ndbl);
  }
  //
  ndbl = Initial_value[1] * Initial_value[1];
  L_b.set_size(1, miu.size(1));
  loop_ub = miu.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    cdiff = miu[i] - apnd;
    L_b[i] = cdiff * cdiff;
  }
  col_LS.set_size(1, miu.size(1));
  loop_ub = miu.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    cdiff = miu[i] - vb2;
    col_LS[i] = cdiff * cdiff;
  }
  if (L_b.size(1) == col_LS.size(1))
  {
    loop_ub = L_b.size(1) - 1;
    L_b.set_size(1, L_b.size(1));
    for (i = 0; i <= loop_ub; i++)
    {
      L_b[i] = Initial_value[1] / (4.0 * L_b[i] + ndbl) +
               Initial_value[1] / (4.0 * col_LS[i] + ndbl);
    }
  }
  else
  {
    binary_expand_op(L_b, Initial_value, ndbl, col_LS, ndbl);
  }
  if (miu.size(1) == 0)
  {
    apnd = 0.0;
  }
  else
  {
    ndbl =
        (miu[0] - miu[miu.size(1) - 1]) * (L_b[0] + L_b[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (k = 0; k <= i - 2; k++)
    {
      ndbl += (miu[k + 1] - miu[k]) * (L_b[k + 1] + L_b[k]) / 2.0;
    }
    apnd = std::abs(ndbl);
  }
  //
  //     %%  Laser Linewidth ��
  // Set the Laser width ��
  // UNTITLED3 Summary of this function goes here
  //    Detailed explanation goes here
  //  Intensity = 1;
  cdiff = params[3] / 2.0;
  // 此处对应半高半宽
  idx = miu.size(1);
  fx.set_size(1, miu.size(1));
  for (i = 0; i < idx; i++)
  {
    fx[i] = 0.0;
  }
  i = static_cast<int>((b_tmp + (1.0E+6 - a_tmp)) / 1.0E+6);
  for (nm1d2 = 0; nm1d2 < i; nm1d2++)
  {
    ndbl = (a_tmp + static_cast<double>(nm1d2) * 1.0E+6) - miu_0;
    fx[nm1d2] = params[2] * std::exp(-(ndbl * ndbl) / (2.0 * (cdiff * cdiff)));
  }
  if (miu.size(1) == 0)
  {
    ndbl = 0.0;
  }
  else
  {
    ndbl =
        (miu[0] - miu[miu.size(1) - 1]) * (fx[0] + fx[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (k = 0; k <= i - 2; k++)
    {
      ndbl += (miu[k + 1] - miu[k]) * (fx[k + 1] + fx[k]) / 2.0;
    }
    ndbl = std::abs(ndbl);
  }
  //  求面积，面积归一化
  fx.set_size(1, fx.size(1));
  loop_ub = fx.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    fx[i] = fx[i] / ndbl;
  }
  //  激光脉冲峰归一化
  nm1d2 = fx.size(1);
  if (fx.size(1) <= 2)
  {
    if (fx.size(1) == 1)
    {
      cdiff = fx[0];
    }
    else if ((fx[0] < fx[fx.size(1) - 1]) ||
             (std::isnan(fx[0]) && (!std::isnan(fx[fx.size(1) - 1]))))
    {
      cdiff = fx[fx.size(1) - 1];
    }
    else
    {
      cdiff = fx[0];
    }
  }
  else
  {
    if (!std::isnan(fx[0]))
    {
      idx = 1;
    }
    else
    {
      boolean_T exitg1;
      idx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= nm1d2))
      {
        if (!std::isnan(fx[k - 1]))
        {
          idx = k;
          exitg1 = true;
        }
        else
        {
          k++;
        }
      }
    }
    if (idx == 0)
    {
      cdiff = fx[0];
    }
    else
    {
      cdiff = fx[idx - 1];
      i = idx + 1;
      for (k = i; k <= nm1d2; k++)
      {
        ndbl = fx[k - 1];
        if (cdiff < ndbl)
        {
          cdiff = ndbl;
        }
      }
    }
  }
  fx.set_size(1, fx.size(1));
  loop_ub = fx.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    fx[i] = fx[i] / cdiff;
  }
  //
  //  SLaser = polyarea( miu, LS );
  //  SpecLaser = LS ./ SLaser;% 米散射峰归一化
  //  S_conv = conv( Iv0, LS, 'same' );
  //     %% Influence of Laser Linewidth
  //  卷积
  // Laser Plus width broadening
  b_L_r.set_size(1, L_r.size(1));
  loop_ub = L_r.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    b_L_r[i] = L_r[i] / B;
  }
  coder::conv(b_L_r, fx, L_r);
  b_L_r.set_size(1, L_b.size(1));
  loop_ub = L_b.size(1);
  for (i = 0; i < loop_ub; i++)
  {
    b_L_r[i] = L_b[i] / apnd;
  }
  coder::conv(b_L_r, fx, col_LS);
  //  卷积
  // Area normalized
  if (miu.size(1) == 0)
  {
    cdiff = 0.0;
  }
  else
  {
    ndbl =
        (miu[0] - miu[miu.size(1) - 1]) * (L_r[0] + L_r[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (k = 0; k <= i - 2; k++)
    {
      ndbl += (miu[k + 1] - miu[k]) * (L_r[k + 1] + L_r[k]) / 2.0;
    }
    cdiff = std::abs(ndbl);
  }
  L_r.set_size(1, L_r.size(1));
  loop_ub = L_r.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    L_r[i] = L_r[i] / cdiff;
  }
  if (miu.size(1) == 0)
  {
    cdiff = 0.0;
  }
  else
  {
    ndbl = (miu[0] - miu[miu.size(1) - 1]) *
           (col_LS[0] + col_LS[miu.size(1) - 1]) / 2.0;
    i = miu.size(1);
    for (k = 0; k <= i - 2; k++)
    {
      ndbl += (miu[k + 1] - miu[k]) * (col_LS[k + 1] + col_LS[k]) / 2.0;
    }
    cdiff = std::abs(ndbl);
  }
  col_LS.set_size(1, col_LS.size(1));
  loop_ub = col_LS.size(1) - 1;
  for (i = 0; i <= loop_ub; i++)
  {
    col_LS[i] = col_LS[i] / cdiff;
  }
  //  S_0 = L_mc + L_rc + L_bc; %Scattering Spectra with Laser Width
  //     %% Water Scatterring Photon
  // Laser energy，论文中是20mJ
  // Attenuation Coefficient in water，论文中是0.08
  // Volume scattering coefficient，论文中是2.4e-4
  // Brillouin Mie Rayleigh ?
  //  [Num_Brillouin, Num_Mie, Num_Rayleigh] = ScatteredPhotons(E_laser,
  //  SetAlpha_water, setBetaP_water);
  // Volume scattering coefficient
  // Laser wavelength
  // Refractive index of water
  // Depth of water
  // Height of water
  // Energy ratio of Mie scattering
  // Brillouin Mie Rayleigh ?
  // UNTITLED11 Summary of this function goes here
  //    Detailed explanation goes here
  // Beta_p = 0.00; %标准状态下（盐度３５％，２０°Ｃ，—个大气压）P55
  // Beta_m = 2.4e-4; %瑞利 布里渊散射系数
  // ---------------------------
  // velocity of light
  // lambda = 532e-9; %wavelength
  // Planck's constant
  // number of laser photos transmitted    入射激光的光子数
  // ---------------------------------------------
  // n = 1.3333; %index of refraction of water   水的折射率
  // z = 10; %depth of range cell(Unit:m)    水下探测深度
  // H = 150; %altitude of receiver in air   接收器在空中的高度
  // tau = 10e-9; %激光器脉宽
  // 水中距离分辨率
  // r = 0.05; %望远镜半径，m
  // 面积
  // Xi = 0.13; %系统检测效率
  // Xi_F = 0.4; %光学系统效率
  ndbl = params[10] + params[9] * params[11];
  // solid angle of receiver 接收器的立体角
  // Number of received scattered photons
  cdiff = params[0] * params[4] / 1.9864458571489286E-25 *
          (3.1415926535897931 * (params[13] * params[13]) / (ndbl * ndbl)) *
          params[7] * std::exp(-2.0 * params[5] * params[10]) *
          (2.99792458E+8 * (1.0 / params[3]) / (2.0 * params[9])) * params[17] *
          params[18];
  // number of output Brillouin photons
  ndbl = params[12] * cdiff;
  // S_r =  0.04 * S_m;
  //  纯水中瑞利散射和布里渊散射的能量比小于
  //  0.04，这意味着瑞利光的强度明显低于水中的布里渊光
  // Signal Noise Ratio
  //  Day time
  //  dark current
  // M = 1; % Integration times
  // beta = 2;
  //     %% Laser induced scattering spectra
  // Rayleigh Spectra
  // SpectraM = L_mc * Num_Mie; %Mie Spectrum?
  // Brillouin Spectrum
  // water Scatter Spectrum
  // S_1 = SpectraR + SpectraM + SpectraB;
  //  [Spectrum1, N_SNRA] = AddNoiseNondB(S_1, SNR);
  //     %% PMT + Fizeau Methods
  // UNTITLED2 Summary of this function goes here
  //    Detailed explanation goes here
  //  %% Laser parameter
  //  lambda = 532e-9; % ��������
  //  c = 299792458; % ���٣���/s
  //  %% PMT parameter
  //  channel_numbe = NumberChannels; % Number of the PMT Channerls
  //  channel_width = 0.8e-3;   % PMT width per channel. Unit: meters
  //  channel_space = 0.2e-3;     % Channel spacing. Unit: meters
  //  PMT_length = channel_numbe * channel_width + (channel_numbe - 1 ) *
  //  channel_space; %The PMT width corresponds to the alpha value of the stripe
  //  width alpha_cal = atan( lambda/2/PMT_length );  %
  //  %% Fizeau Instrument Function
  //  [ FizeauFunction, ~ ] = FizeauIF( alpha_cal, Frequency );
  //  Fizeau_spectrum = FizeauFunction ./ polyarea( Frequency, FizeauFunction );
  //  %Area normalization save('FizeauData.mat', 'Fizeau_spectrum' );
  //     %%
  //  load('FizeauData.mat', 'Fizeau_spectrum');
  //     %% Spectrum after passing through the Fizeau interferometer
  if (L_r.size(1) == col_LS.size(1))
  {
    b_L_r.set_size(1, L_r.size(1));
    loop_ub = L_r.size(1);
    for (i = 0; i < loop_ub; i++)
    {
      b_L_r[i] = L_r[i] * ndbl + col_LS[i] * cdiff;
    }
    coder::conv(b_L_r, Fizeau_spectrum, col_LS);
  }
  else
  {
    binary_expand_op(col_LS, L_r, ndbl, cdiff, Fizeau_spectrum);
  }
  // Energy is enhanced after convolution
  //     %% PMT Arrays Receive the spectrum
  // UNTITLED4 Summary of this function goes here
  //    Detailed explanation goes here
  //  PMT Array System Parameters
  //  Number of the PMT Channerls
  //  PMT width per channel. Unit: meters
  //  Channel spacing. Unit: meters
  cdiff = ChannelNumber * 0.0008 + (ChannelNumber - 1.0) * 0.0002;
  //  PMT Receive Energy
  vb2 = std::floor(0.0008 / cdiff * static_cast<double>(col_LS.size(1)));
  apnd = std::floor(0.0002 / cdiff * static_cast<double>(col_LS.size(1)));
  i = static_cast<int>(ChannelNumber);
  varargout_1.set_size(1, i);
  for (int b_i{0}; b_i < i; b_i++)
  {
    int i1;
    int i2;
    cdiff = ((static_cast<double>(b_i) + 1.0) - 1.0) * (vb2 + apnd) + 1.0;
    ndbl = cdiff + vb2;
    if (cdiff > ndbl)
    {
      nm1d2 = 0;
      i1 = 0;
      i2 = -1;
      idx = -1;
    }
    else
    {
      nm1d2 = static_cast<int>(cdiff) - 1;
      i1 = static_cast<int>(ndbl);
      i2 = static_cast<int>(cdiff) - 2;
      idx = static_cast<int>(ndbl) - 1;
    }
    cdiff = 0.0;
    loop_ub = idx - i2;
    if (loop_ub <= 1)
    {
      if ((i1 - nm1d2 == 1) &&
          (std::isinf(RF[nm1d2]) || std::isnan(RF[nm1d2])))
      {
        cdiff = rtNaN;
      }
    }
    else
    {
      int c_tmp;
      if (i1 - nm1d2 == 1)
      {
        c.set_size(loop_ub);
        for (i1 = 0; i1 < loop_ub; i1++)
        {
          c[i1] = RF[nm1d2];
        }
        ndbl = 0.5 * RF[nm1d2];
        c[0] = ndbl;
        c[(idx - i2) - 1] = ndbl;
      }
      else
      {
        c.set_size(loop_ub);
        c[0] = 0.5 * (RF[nm1d2 + 1] - RF[nm1d2]);
        i1 = loop_ub - 1;
        for (k = 2; k <= i1; k++)
        {
          c_tmp = nm1d2 + k;
          c[k - 1] = 0.5 * (RF[c_tmp] - RF[c_tmp - 2]);
        }
        c_tmp = (nm1d2 + idx) - i2;
        c[loop_ub - 1] = 0.5 * (RF[c_tmp - 1] - RF[c_tmp - 2]);
      }
      nm1d2 = 0;
      for (idx = 1; idx <= loop_ub; idx++)
      {
        for (c_tmp = idx; c_tmp <= idx; c_tmp++)
        {
          cdiff += col_LS[i2 + c_tmp] * c[nm1d2];
        }
        nm1d2++;
      }
    }
    varargout_1[b_i] = cdiff;
  }
  //  %% Retrieval Algorithm
  //  [ fitted_value, residual ] = RetrievalAlgorithm( channel_sign,
  //  channel_energy, FizeauFunction );
  //     %% Function Output
  //     %% Caculate the Error
  if (varargout_1.size(1) == MeasurementEnergy.size(1))
  {
    loop_ub = varargout_1.size(1) - 1;
    varargout_1.set_size(1, varargout_1.size(1));
    for (i = 0; i <= loop_ub; i++)
    {
      varargout_1[i] = varargout_1[i] - MeasurementEnergy[i];
    }
  }
  else
  {
    minus(varargout_1, MeasurementEnergy);
  }
  //  在 x 处计算的雅可比值矩阵
  //     %% Calculate the Error
  // plot(PMT_Sign, PMT_Eneeergy, 'r', PMT_Sign, MeasurementEnergy, 'b');
  // xlabel('Relative Frequency (GHz)');
  // ylabel('Energy (a.u.)');
  // legend('Fitted', 'Measured');
  // title('Fitted and Measured Energy');
  // hold on;
}

//
// File trailer for RetrievalAlgorithm.cpp
//
// [EOF]
//
