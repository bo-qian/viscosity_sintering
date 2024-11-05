/*
 * @Author: Bo Qian
 * @Date: 2024-11-05 14:11:36
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 20:41:21
 * @LastEditors: Bo Qian
 * @Description: Kernel of x-component of the Stokes equation
 * @FilePath: /viscosity_sintering/src/kernels/StokesX.C
 */


#include "StokesX.h"

registerMooseObject("viscosity_sinteringApp", StokesX);

InputParameters
StokesX::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of x-component of the Stokes equation");
  params.addRequiredCoupledVar("v", "y-velocity variable");
  params.addRequiredCoupledVar("w", "z-velocity variable");
	params.addRequiredCoupledVar("p", "Pressure variable");
  return params;
}

StokesX::StokesX(const InputParameters & parameters)
  : Kernel(parameters),
  _mu_eff(getMaterialProperty<Real>("mu_eff")),
  _kappa_C(getMaterialProperty<Real>("kappa_C")),

  // Coupled variables
  _v_vel(coupledValue("v")),
  _w_vel(coupledValue("w")),

  _c(coupledValue("c")),
	_grad_c(coupledGradient("c")),
	_p(coupledValue("p")),

	_grad_v_vel(coupledGradient("v")),
	_grad_w_vel(coupledGradient("w"))

{
	
}

Real 
StokesX::velocityTermX()
{
	return (_grad_u[_qp](0) + _grad_u[_qp](0)) * _grad_test[_i][_qp](0)
					+ (_grad_u[_qp](1) + _grad_v_vel[_qp](0)) * _grad_test[_i][_qp](1)
					+ (_grad_u[_qp](2) + _grad_w_vel[_qp](0)) * _grad_test[_i][_qp](2);
}

Real
StokesX::pressureTermX()
{
	return -_p[_qp] * _grad_test[_i][_qp](0);
}

Real
StokesX::surfaceTensionTermX()
{
	return _kappa_C[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](0) * _grad_test[_i][_qp](0)
													+ _grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](1)
													+ _grad_c[_qp](0) * _grad_c[_qp](2) * _grad_test[_i][_qp](2));
}

Real
StokesX::ResidualX()
{
	// 计算残差项
	return -_mu_eff[_qp] * velocityTermX() + pressureTermX() + surfaceTensionTermX();
}

Real 
StokesX::computeQpResidual()
{
	// 计算残差项
	return ResidualX();
}