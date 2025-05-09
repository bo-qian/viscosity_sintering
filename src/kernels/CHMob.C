/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:36:35
 * @FilePath: /viscosity_sintering/src/kernels/CHMob.C
 * @Description: Kernel of the mobility term of Cahn-Hilliard equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "CHMob.h"

registerMooseObject("viscosity_sinteringApp", CHMob);

InputParameters
CHMob::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of the mobility term of Cahn-Hilliard equation");
  params.addRequiredCoupledVar("coupledvar", "variant of phase field");
  return params;
}

CHMob::CHMob(const InputParameters & parameters)
  : Kernel(parameters),
  _mobility(getMaterialProperty<Real>("M_value")),
  _muvar(coupled("coupledvar")),
  _mu(coupledValue("coupledvar")),
  _grad_mu(coupledGradient("coupledvar"))
{
  
}

Real 
CHMob::computeQpResidual()
{
  return _mobility[_qp] * _grad_mu[_qp] * _grad_test[_i][_qp];
}

Real
CHMob::computeQpJacobian()
{
  return 0.0;
}

Real
CHMob::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _muvar)
    return _mobility[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
  return 0.0;
}
