/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 11:06:26
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:19:28
 * @FilePath: /viscosity_sintering/src/kernels/StokesMass.C
 * @Description: Incompressibility ADKernel
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#include "StokesMass.h"

registerMooseObject("viscosity_sinteringApp", StokesMass);

InputParameters
StokesMass::validParams()
{
  InputParameters params = ADKernel::validParams();
  params.addClassDescription("Incompressibility ADKernel");
  params.addRequiredCoupledVar("velocity", "velocity variable");
  return params;
}

StokesMass::StokesMass(const InputParameters & parameters)
  : ADKernel(parameters),
    // _velvar(coupledVectorDot("velocity")),
    _vel(coupledVectorValue("velocity")),
    _grad_vel(coupledVectorGradient("velocity"))
{
}

ADReal
StokesMass::computeQpResidual()
{
  return _grad_vel[_qp].tr() * _test[_i][_qp];
}

// Real
// StokesMass::computeQpJacobian()
// {
//   return 0;
// }

// Real
// StokesMass::computeQpOffDiagJacobian(unsigned jvar)
// {
//   if (jvar == _velvar(0))
//     return _grad_phi[_j][_qp].tr() * _test[_i][_qp];
//   else
//     return 0;
// }
#endif