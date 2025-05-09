/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:35:40
 * @FilePath: /viscosity_sintering/src/kernels/Incompressibility.C
 * @Description: Incompressibility Kernel
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */


# include "Incompressibility.h"

registerMooseObject("viscosity_sinteringApp", Incompressibility);

InputParameters
Incompressibility::validParams()
{
  InputParameters params = Kernel::validParams();
  MooseEnum dims("2=2 3");
	params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
  params.addClassDescription("Incompressibility Kernel");
  params.addRequiredCoupledVar("x_velocity", "The x-velocity variable coupled into CH equation");
  params.addRequiredCoupledVar("y_velocity", "The y-velocity variable coupled into CH equation");
  params.addCoupledVar("z_velocity", 0, "The z-velocity variable coupled into CH equation");
  return params;
}

Incompressibility::Incompressibility(const InputParameters & parameters)
  : Kernel(parameters),
  _dim(getParam<MooseEnum>("dim")),

  _u_velocity_var(coupled("x_velocity")),
  _v_velocity_var(coupled("y_velocity")),
  _w_velocity_var(coupled("z_velocity")),

  _u_velocity(coupledValue("x_velocity")),
  _v_velocity(coupledValue("y_velocity")),
  _w_velocity(coupledValue("z_velocity")),

  _grad_u_velocity(coupledGradient("x_velocity")),
  _grad_v_velocity(coupledGradient("y_velocity")),
  _grad_w_velocity(coupledGradient("z_velocity"))

{
}

RealVectorValue
Incompressibility::computeQpVelocity()
{
  switch (_dim)
  {
    case 2:
      return RealVectorValue(_u_velocity[_qp], _v_velocity[_qp]);
    case 3:
      return RealVectorValue(_u_velocity[_qp], _v_velocity[_qp], _w_velocity[_qp]);
    default:
      mooseError("Invalid dimension value, should be 2 or 3, from computeQpVelocity");
  }
}

Real 
Incompressibility::computeQpDivVelocity()
{
  switch (_dim)
  {
    case 2:
      return _grad_u_velocity[_qp](0) + _grad_v_velocity[_qp](1);
    case 3:
      return _grad_u_velocity[_qp](0) + _grad_v_velocity[_qp](1) + _grad_w_velocity[_qp](2);
    default:
      mooseError("Invalid dimension value, should be 2 or 3, from computeQpDivVelocity");
  }
}

// 计算残差项
Real
Incompressibility::computeQpResidual()
{
  return computeQpDivVelocity() * _test[_i][_qp];
}

// 计算雅可比矩阵项
Real
Incompressibility::computeQpJacobian()
{
  return 0.0;
}

Real
Incompressibility::computeQpOffDiagJacobian(unsigned jvar)
{
  switch (_dim)
  {
    case 2:
      if (jvar == _u_velocity_var)
        return _grad_phi[_j][_qp](0) * _test[_i][_qp];

      if (jvar == _v_velocity_var)
        return _grad_phi[_j][_qp](1) * _test[_i][_qp];

      return 0.0;
      
    case 3:
      if (jvar == _u_velocity_var)
        return _grad_phi[_j][_qp](0) * _test[_i][_qp];

      if (jvar == _v_velocity_var)
        return _grad_phi[_j][_qp](1) * _test[_i][_qp];

      if (jvar == _w_velocity_var)
        return _grad_phi[_j][_qp](2) * _test[_i][_qp];
      
      return 0.0;
  
    default:
      mooseError("Invalid dimension value, should be 2 or 3, from computeQpOffDiagJacobian");
  }
}