/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 09:22:17
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:19:56
 * @FilePath: /viscosity_sintering/src/kernels/StokesViscosity.C
 * @Description: Vector Kernel of the viscosity term in the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#include "StokesViscosity.h"
#include "MathUtils.h"

registerMooseObject("viscosity_sinteringApp", StoeksViscosity);

InputParameters
StoeksViscosity::validParams()
{
  InputParameters params = ADVectorKernel::validParams();
  params.addClassDescription("Vector Kernel of the viscosity term in the Stokes equation");
  return params;
}

StoeksViscosity::StoeksViscosity(const InputParameters & parameters)
  : ADVectorKernel(parameters),
    _mu_eff(getMaterialProperty<Real>("mu_eff"))
{
}

ADReal
StoeksViscosity::computeQpResidual()
{
  return MathUtils::dotProduct(_mu_eff[_qp] * (_grad_u[_qp] + _grad_u[_qp].transpose()), _grad_test[_i][_qp]);
}

// Real
// StoeksViscosity::computeQpJacobian()
// {
//   return MathUtils::dotProduct(_mu_eff[_qp] * (_grad_phi[_j][_qp] + _grad_phi[_j][_qp].transpose()), _grad_test[_i][_qp]);
// }

// Real
// StoeksViscosity::computeQpOffDiagJacobian(unsigned jvar)
// {
//   return 0;
// }
#endif
