/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 10:16:55
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:20:05
 * @FilePath: /viscosity_sintering/src/kernels/StokesSurfaceTension.C
 * @Description: Vector Kernel of the surface tension term in the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#include "StokesSurfaceTension.h"
#include "MathUtils.h"
#include "RankTwoTensor.h"

registerMooseObject("viscosity_sinteringApp", StokesSurfaceTension);

InputParameters
StokesSurfaceTension::validParams()
{
  InputParameters params = ADVectorKernel::validParams();
  params.addClassDescription("Vector Kernel of the surface tension term in the Stokes equation");
  params.addRequiredCoupledVar("phase_field", "variant of phase field");
  params.addRequiredCoupledVar("pressure", "Pressure variable");
  return params;
}

StokesSurfaceTension::StokesSurfaceTension(const InputParameters & parameters)
  : ADVectorKernel(parameters),
    _kappa_c(getMaterialProperty<Real>("kappa_C_value")),
    // _cvar(coupled("phase_field")),
    _c(coupledValue("phase_field")),
    _grad_c(coupledGradient("phase_field")),
    // _pvar(coupled("pressure")),
    _p(coupledValue("pressure"))
{
}

ADReal
StokesSurfaceTension::computeQpResidual()
{
  return - MathUtils::dotProduct(_kappa_c[_qp] * RankTwoTensor::outerProduct(_grad_c[_qp], _grad_c[_qp]), _grad_test[_i][_qp]);
}

// Real
// StokesSurfaceTension::computeQpJacobian()
// {
//   return 0;
// }

// Real
// StokesSurfaceTension::computeQpOffDiagJacobian(unsigned jvar)
// {
//   return 0;
// }
#endif