/*
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-21 19:48:57
 * @LastEditors: Bo Qian
 * @Description: Kernel for CH equation coupled with velocity
 * @FilePath: /viscosity_sintering/src/kernels/CHCoupV.C
 */

#include "CHCoupV.h"

registerMooseObject("viscosity_sinteringApp", CHCoupV);

InputParameters 
CHCoupV::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("velocity coupled into CH equation");
  params.addRequiredCoupledVar("x_velocity", "The x-velocity variable coupled into CH equation");
  params.addRequiredCoupledVar("y_velocity", "The y-velocity variable coupled into CH equation");
  params.addCoupledVar("z_velocity", 0, "The z-velocity variable coupled into CH equation");
  // params.addRequiredCoupledVar("v", "The velocity variable coupled into CH equation");
  return params;
}

CHCoupV::CHCoupV(const InputParameters & parameters)
  : Kernel(parameters),
  _u_velocoty_var(coupled("x_velocity")),
  _v_velocity_var(coupled("y_velocity")),
  _w_velocity_var(coupled("z_velocity")),

  _u_velocity(coupledValue("x_velocity")),  
  _v_velocity(coupledValue("y_velocity")),  
  _w_velocity(coupledValue("z_velocity")),

  _u_velocity_old(coupledValueOld("x_velocity")),
  _v_velocity_old(coupledValueOld("y_velocity")),
  _w_velocity_old(coupledValueOld("z_velocity"))
  // _v(coupledVectorValue("v"))  // 初始化速度矢量变量
{
}

RealVectorValue
CHCoupV::computeQpVelocity()
{
  return RealVectorValue(_u_velocity[_qp], _v_velocity[_qp], _w_velocity[_qp]);
}

RealVectorValue
CHCoupV::computeQpVelocityOld()
{
  return RealVectorValue(_u_velocity_old[_qp], _v_velocity_old[_qp], _w_velocity_old[_qp]);
}

// 计算残差项
Real
CHCoupV::computeQpResidual()
{
  return -_u[_qp] * (computeQpVelocityOld() * _grad_test[_i][_qp]);
}

// 计算雅可比矩阵项
Real
CHCoupV::computeQpJacobian()
{
  return -_phi[_j][_qp] * (computeQpVelocityOld() * _grad_test[_i][_qp]);  // 残差对变量 C 的导数
}

Real 
CHCoupV::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _u_velocoty_var)
    return 0.0;
  if (jvar == _v_velocity_var)
    return 0.0;
  if (jvar == _w_velocity_var)
    return 0.0;
  return 0.0;
}
