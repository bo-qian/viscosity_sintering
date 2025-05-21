/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 10:56:52
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:19:16
 * @FilePath: /viscosity_sintering/src/kernels/StokesPressure.C
 * @Description: Vector Kernel of the pressure term in the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#include "StokesPressure.h"

registerMooseObject("viscosity_sinteringApp", StokesPressure);

InputParameters
StokesPressure::validParams()
{
  InputParameters params = ADVectorKernel::validParams();
  params.addClassDescription("Vector Kernel of the pressure term in the Stokes equation");
  params.addRequiredCoupledVar("pressure", "Pressure variable");
  return params;
}

StokesPressure::StokesPressure(const InputParameters & parameters)
  : ADVectorKernel(parameters),
    // _pvar(coupled("pressure")),
    _p(coupledValue("pressure")),
    _grad_p(coupledGradient("pressure"))
{
}

ADReal
StokesPressure::computeQpResidual()
{
  return _u[_qp] * _grad_test[_i][_qp].tr();
}

// Real
// StokesPressure::computeQpJacobian()
// {
//   return 0;
// }

// Real
// StokesPressure::computeQpOffDiagJacobian(unsigned jvar)
// {
//   if (jvar == _pvar)
//     return _phi[_j][_qp] * _grad_test[_i][_qp].tr();
//   else
//     return 0;
// }

#endif