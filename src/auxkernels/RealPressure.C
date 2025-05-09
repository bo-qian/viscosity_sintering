/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 18:15:24
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:31:10
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
  params.addRequiredCoupledVar("phase_field", "Phase field variable");
  params.addRequiredCoupledVar("chemical_potential", "Chemical potential variable");
  return params;
}

RealPressure::RealPressure(const InputParameters & parameters)
  : AuxKernel(parameters),
    _alpha(getMaterialProperty<Real>("alpha_value")),
    _kappa_c(getMaterialProperty<Real>("kappa_C_value")),
    _p(coupledValue("pressure")),
    _c(coupledValue("phase_field")),
    _grad_c(coupledGradient("phase_field")),
    _mu(coupledValue("chemical_potential"))
{
}

Real
RealPressure::computeValue()
{
  const Real f_density = _alpha[_qp] * _c[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]) + 0.5 * _kappa_c[_qp] * _grad_c[_qp].norm_sq();
  // Compute the real pressure
  return _p[_qp] - f_density + _c[_qp] * _mu[_qp];
}