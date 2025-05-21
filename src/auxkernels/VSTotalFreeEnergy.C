/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-13 19:12:41
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
  return params;
}

VSTotalFreeEnergy::VSTotalFreeEnergy(const InputParameters & parameters)
  : AuxKernel(parameters),
  _F_loc(getMaterialProperty<Real>("F_loc")),
  _F_grad(getMaterialProperty<Real>("F_grad"))
{
}

Real
VSTotalFreeEnergy::computeValue()
{
  return _F_loc[_qp] + _F_grad[_qp];
}