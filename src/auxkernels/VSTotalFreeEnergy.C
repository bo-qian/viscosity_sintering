/*
 * @Author: Bo Qian
 * @Date: 2024-12-26 14:02:56
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-26 14:40:10
 * @LastEditors: Bo Qian
 * @Description: 
 * @FilePath: /viscosity_sintering/src/auxkernels/VSTotalFreeEnergy.C
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
  return _F_loc[_qp] + 0.5 * _kappa_C[_qp] * _grad_c[_qp].norm_sq();
}