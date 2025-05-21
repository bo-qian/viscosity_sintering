/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 18:15:24
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-14 09:12:06
 * @FilePath: /viscosity_sintering/src/auxkernels/RealPressure.C
 * @Description: Compute the real pressure
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "RealPressure.h"

registerMooseObject("viscosity_sinteringApp", RealPressure);

InputParameters
RealPressure::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Compute the real pressure");
  params.addRequiredCoupledVar("pressure", "Pressure variable");
  return params;
}

RealPressure::RealPressure(const InputParameters & parameters)
  : AuxKernel(parameters),
    _p(coupledValue("pressure")),
    _f_loc(getMaterialProperty<Real>("F_loc")),
    _f_grad(getMaterialProperty<Real>("F_grad"))
{
}

Real
RealPressure::computeValue()
{
  // return _p[_qp] - f_density + _c[_qp] * _mu[_qp];
  return _p[_qp] - (_f_loc[_qp] + _f_grad[_qp]);
}