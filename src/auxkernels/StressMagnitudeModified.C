/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-03-05 18:56:37
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-12 16:45:56
 * @FilePath: /viscosity_sintering/src/auxkernels/StressMagnitudeModified.C
 * @Description: AuxKernel for computing the stress magnitude
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */


#include "StressMagnitudeModified.h"

registerMooseObject("viscosity_sinteringApp", StressMagnitudeModified);

InputParameters
StressMagnitudeModified::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("StressMagnitudeModified");
  return params;
}

StressMagnitudeModified::StressMagnitudeModified(const InputParameters & parameters)
  : AuxKernel(parameters),
  _stress_xx_modified(getMaterialProperty<Real>("stress_xx_modified")),
  _stress_xy(getMaterialProperty<Real>("stress_xy")),
  _stress_xz(getMaterialProperty<Real>("stress_xz")),
  _stress_yx(getMaterialProperty<Real>("stress_yx")),
  _stress_yy_modified(getMaterialProperty<Real>("stress_yy_modified")),
  _stress_yz(getMaterialProperty<Real>("stress_yz")),
  _stress_zx(getMaterialProperty<Real>("stress_zx")),
  _stress_zy(getMaterialProperty<Real>("stress_zy")),
  _stress_zz_modified(getMaterialProperty<Real>("stress_zz_modified"))
{
}

Real
StressMagnitudeModified::computeValue()
{
  return std::sqrt(_stress_xx_modified[_qp] * _stress_xx_modified[_qp] + _stress_xy[_qp] * _stress_xy[_qp] + _stress_xz[_qp] * _stress_xz[_qp] +
                   _stress_yx[_qp] * _stress_yx[_qp] + _stress_yy_modified[_qp] * _stress_yy_modified[_qp] + _stress_yz[_qp] * _stress_yz[_qp] +
                   _stress_zx[_qp] * _stress_zx[_qp] + _stress_zy[_qp] * _stress_zy[_qp] + _stress_zz_modified[_qp] * _stress_zz_modified[_qp]);
}