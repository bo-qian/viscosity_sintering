/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-03-05 18:56:37
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:28:59
 * @FilePath: /viscosity_sintering/src/auxkernels/StressMagnitude.C
 * @Description: AuxKernel for computing the stress magnitude
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */


#include "StressMagnitude.h"

registerMooseObject("viscosity_sinteringApp", StressMagnitude);

InputParameters
StressMagnitude::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("StressMagnitude");
  return params;
}

StressMagnitude::StressMagnitude(const InputParameters & parameters)
  : AuxKernel(parameters),
  _stress_xx(getMaterialProperty<Real>("stress_xx")),
  _stress_xy(getMaterialProperty<Real>("stress_xy")),
  _stress_xz(getMaterialProperty<Real>("stress_xz")),
  _stress_yx(getMaterialProperty<Real>("stress_yx")),
  _stress_yy(getMaterialProperty<Real>("stress_yy")),
  _stress_yz(getMaterialProperty<Real>("stress_yz")),
  _stress_zx(getMaterialProperty<Real>("stress_zx")),
  _stress_zy(getMaterialProperty<Real>("stress_zy")),
  _stress_zz(getMaterialProperty<Real>("stress_zz"))
{
}

Real
StressMagnitude::computeValue()
{
  return std::sqrt(_stress_xx[_qp] * _stress_xx[_qp] + _stress_xy[_qp] * _stress_xy[_qp] + _stress_xz[_qp] * _stress_xz[_qp] +
                   _stress_yx[_qp] * _stress_yx[_qp] + _stress_yy[_qp] * _stress_yy[_qp] + _stress_yz[_qp] * _stress_yz[_qp] +
                   _stress_zx[_qp] * _stress_zx[_qp] + _stress_zy[_qp] * _stress_zy[_qp] + _stress_zz[_qp] * _stress_zz[_qp]);
}