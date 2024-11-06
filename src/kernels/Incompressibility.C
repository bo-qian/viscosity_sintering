/*
 * @Author: Bo Qian
 * @Date: 2024-10-24 09:08:32
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-06 22:51:19
 * @LastEditors: Bo Qian
 * @Description: Incompressibility Kernel
 * @FilePath: /viscosity_sintering/src/kernels/Incompressibility.C
 */

# include "Incompressibility.h"

registerMooseObject("viscosity_sinteringApp", Incompressibility);

InputParameters
Incompressibility::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Incompressibility Kernel");
  params.addRequiredCoupledVar("x_velocity", "The x-velocity variable coupled into CH equation");
  params.addRequiredCoupledVar("y_velocity", "The y-velocity variable coupled into CH equation");
  params.addCoupledVar("z_velocity", 0, "The z-velocity variable coupled into CH equation");
  return params;
}

Incompressibility::Incompressibility(const InputParameters & parameters)
  : Kernel(parameters),
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
  return RealVectorValue(_u_velocity[_qp], _v_velocity[_qp], _w_velocity[_qp]);
}

Real 
Incompressibility::computeQpDivVelocity()
{
  return _grad_u_velocity[_qp](0) + _grad_v_velocity[_qp](1) + _grad_w_velocity[_qp](2);
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
  return computeQpDivVelocity() * _test[_i][_qp];
}