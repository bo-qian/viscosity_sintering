/*
 * @Author: Bo Qian
 * @Date: 2024-11-05 14:11:36
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-06 23:27:41
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
	MooseEnum dims("2=2 3");
	params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
	params.addClassDescription("Kernel of x-component of the Stokes equation");
  params.addRequiredCoupledVar("v", "y-velocity variable"); // required in 2D and 3D
  params.addCoupledVar("w", 0, "z-velocity variable"); // only required in 3D
	params.addRequiredCoupledVar("p", "Pressure variable");
	params.addRequiredCoupledVar("c", "variant of phase field");
  return params;
}

StokesX::StokesX(const InputParameters & parameters)
  : Kernel(parameters),
	_dim(getParam<MooseEnum>("dim")),

  _mu_eff(getMaterialProperty<Real>("mu_eff")),
  _kappa_c(getMaterialProperty<Real>("kappa_C")),

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
StokesX::velocityTermX(Dimension type)
{
	switch (type)
	{
		case two_dimension:
			return (_grad_u[_qp](0) + _grad_u[_qp](0)) * _grad_test[_i][_qp](0)
						+ (_grad_u[_qp](1) + _grad_v_vel[_qp](0)) * _grad_test[_i][_qp](1);
		case three_dimension:
			return (_grad_u[_qp](0) + _grad_u[_qp](0)) * _grad_test[_i][_qp](0)
						+ (_grad_u[_qp](1) + _grad_v_vel[_qp](0)) * _grad_test[_i][_qp](1)
						+ (_grad_u[_qp](2) + _grad_w_vel[_qp](0)) * _grad_test[_i][_qp](2);
    default:
      mooseError("type must be three_dimension or three_dimension");
	}
}

Real
StokesX::pressureTermX()
{
	return -_p[_qp] * _grad_test[_i][_qp](0);
}

Real
StokesX::surfaceTensionTermX(Dimension type)
{
	switch (type)
	{
		case two_dimension:
			return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](0) * _grad_test[_i][_qp](0)
													+ _grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](1));
		case three_dimension:
			return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](0) * _grad_test[_i][_qp](0)
													+ _grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](1)
													+ _grad_c[_qp](0) * _grad_c[_qp](2) * _grad_test[_i][_qp](2));
    default:
      mooseError("type must be three_dimension or three_dimension");
	}
}

Real
StokesX::ResidualX(Dimension type)
{
	switch (type)
	{
		case two_dimension:
			return -_mu_eff[_qp] * velocityTermX(Dimension::two_dimension) 
							+ pressureTermX() + surfaceTensionTermX(Dimension::two_dimension);

		case three_dimension:
			return -_mu_eff[_qp] * velocityTermX(Dimension::three_dimension) 
							+ pressureTermX() + surfaceTensionTermX(Dimension::three_dimension);

		default:
			mooseError("type must be three_dimension or three_dimension");
	}
}

Real 
StokesX::computeQpResidual()
{
	switch (_dim)
	{
		case 2:
			return ResidualX(Dimension::two_dimension);
		case 3:
			return ResidualX(Dimension::three_dimension);
		default:
			mooseError("Invalid dimension value, should be 2 or 3");
	}
}