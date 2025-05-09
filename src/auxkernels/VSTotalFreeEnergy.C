/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:31:21
 * @FilePath: /viscosity_sintering/src/auxkernels/VSTotalFreeEnergy.C
 * @Description: AuxKernel for computing the total free energy
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "VSTotalFreeEnergy.h"

registerMooseObject("viscosity_sinteringApp", VSTotalFreeEnergy);

InputParameters
VSTotalFreeEnergy::validParams()
{
  InputParameters params = AuxKernel::validParams();
  params.addClassDescription("Total Free Energy Kernel");
  params.addCoupledVar("phase_field", 0.0, "The phase field variable");
  return params;
}

VSTotalFreeEnergy::VSTotalFreeEnergy(const InputParameters & parameters)
  : AuxKernel(parameters),
  _c(coupledValue("phase_field")),
  _grad_c(coupledGradient("phase_field")),
  _F_loc(getMaterialProperty<Real>("F_loc")),
  _kappa_C(getMaterialProperty<Real>("kappa_C_value"))
{
}

Real
VSTotalFreeEnergy::computeValue()
{
  Real local_energy = _F_loc[_qp];
  Real gradient_energy = 0.5 * _kappa_C[_qp] * _grad_c[_qp].norm_sq();
  return local_energy + gradient_energy;
}