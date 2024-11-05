/*
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:46:17
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 20:49:09
 * @LastEditors: Bo Qian
 * @Description: Kernel of y-component of the Stokes equation
 * @FilePath: /viscosity_sintering/src/kernels/StokesY.C
 */

#include "StokesY.h"

registerMooseObject("viscosity_sinteringApp", StokesY);

InputParameters
StokesY::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of y-component of the Stokes equation");
  params.addRequiredCoupledVar("u", "x-velocity variable");
  params.addRequiredCoupledVar("w", "z-velocity variable");
  params.addRequiredCoupledVar("p", "Pressure variable");
  return params;
}

StokesY::StokesY(const InputParameters & parameters)
  : Kernel(parameters),
  _mu_eff(getMaterialProperty<Real>("mu_eff")),
  _kappa_C(getMaterialProperty<Real>("kappa_C")),

  // Coupled variables
  _u_vel(coupledValue("u")),
  _w_vel(coupledValue("w")),

  _c(coupledValue("c")),
	_grad_c(coupledGradient("c")),
	_p(coupledValue("p")),

	_grad_u_vel(coupledGradient("u")),
	_grad_w_vel(coupledGradient("w"))

{
	
}

Real 
StokesY::velocityTermY()
{
	return (_grad_u[_qp](0) + _grad_u_vel[_qp](1)) * _grad_test[_i][_qp](0)
					+ (_grad_u[_qp](1) + _grad_u[_qp](1)) * _grad_test[_i][_qp](1)
					+ (_grad_u[_qp](2) + _grad_w_vel[_qp](1)) * _grad_test[_i][_qp](2);
}

Real
StokesY::pressureTermY()
{
	return -_p[_qp] * _grad_test[_i][_qp](1);
}

Real
StokesY::surfaceTensionTermY()
{
	return _kappa_C[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](0)
													+ _grad_c[_qp](1) * _grad_c[_qp](1) * _grad_test[_i][_qp](1)
													+ _grad_c[_qp](2) * _grad_c[_qp](1) * _grad_test[_i][_qp](2));
}

Real
StokesY::ResidualY()
{
	return -_mu_eff[_qp] * velocityTermY() + pressureTermY() + surfaceTensionTermY();
}

Real
StokesY::computeQpResidual()
{
  return ResidualY();
}