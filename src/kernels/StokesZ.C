/*
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:57:41
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 20:41:47
 * @LastEditors: Bo Qian
 * @Description: Kernel of z-component of the Stokes equation
 * @FilePath: /viscosity_sintering/src/kernels/StokesZ.C
 */

#include "StokesZ.h"

registerMooseObject("viscosity_sinteringApp", StokesZ);

InputParameters
StokesZ::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of x-component of the Stokes equation");
  params.addRequiredCoupledVar("u", "x-velocity variable");
  params.addRequiredCoupledVar("v", "y-velocity variable");
	params.addRequiredCoupledVar("p", "Pressure variable");
  return params;
}

StokesZ::StokesZ(const InputParameters & parameters)
  : Kernel(parameters),
  _mu_eff(getMaterialProperty<Real>("mu_eff")),
  _kappa_C(getMaterialProperty<Real>("kappa_C")),

  // Coupled variables
  _u_vel(coupledValue("u")),
  _v_vel(coupledValue("v")),

  _c(coupledValue("c")),
	_grad_c(coupledGradient("c")),
	_p(coupledValue("p")),

	_grad_u_vel(coupledGradient("u")),
	_grad_v_vel(coupledGradient("v"))

{
	
}

Real 
StokesZ::velocityTermZ()
{
	return (_grad_u[_qp](0) + _grad_u_vel[_qp](2)) * _grad_test[_i][_qp](0)
				+ (_grad_u[_qp](1) + _grad_v_vel[_qp](2)) * _grad_test[_i][_qp](1)
				+ (_grad_u[_qp](2) + _grad_u[_qp](2)) * _grad_test[_i][_qp](2);
}

Real
StokesZ::pressureTermZ()
{
	return -_p[_qp] * _grad_test[_i][_qp](2);
}

Real
StokesZ::surfaceTensionTermZ()
{
	return _kappa_C[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](2) * _grad_test[_i][_qp](0)
													+ _grad_c[_qp](1) * _grad_c[_qp](2) * _grad_test[_i][_qp](1)
													+ _grad_c[_qp](2) * _grad_c[_qp](2) * _grad_test[_i][_qp](2));
}

Real
StokesZ::ResidualZ()
{
	return -_mu_eff[_qp] * velocityTermZ() + pressureTermZ() + surfaceTensionTermZ();
}

Real 
StokesZ::computeQpResidual()
{
	return ResidualZ();
}