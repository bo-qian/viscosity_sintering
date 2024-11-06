/*
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:46:17
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-06 23:29:28
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
  MooseEnum dims("2=2 3");
	params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
	params.addClassDescription("Kernel of y-component of the Stokes equation");
  params.addRequiredCoupledVar("u", "x-velocity variable");
  params.addRequiredCoupledVar("w", "z-velocity variable");
  params.addRequiredCoupledVar("p", "Pressure variable");
	params.addRequiredCoupledVar("c", "variant of phase field");
  return params;
}

StokesY::StokesY(const InputParameters & parameters)
  : Kernel(parameters),
	_dim(getParam<MooseEnum>("dim")),
  
  _mu_eff(getMaterialProperty<Real>("mu_eff")),
  _kappa_c(getMaterialProperty<Real>("kappa_C")),

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
StokesY::velocityTermY(Dimension type)
{
  switch (type)
  {
    case two_dimension:
      return (_grad_u[_qp](0) + _grad_u_vel[_qp](1)) * _grad_test[_i][_qp](0)
					 + (_grad_u[_qp](1) + _grad_u[_qp](1)) * _grad_test[_i][_qp](1);
    case three_dimension:
      return (_grad_u[_qp](0) + _grad_u_vel[_qp](1)) * _grad_test[_i][_qp](0)
					 + (_grad_u[_qp](1) + _grad_u[_qp](1)) * _grad_test[_i][_qp](1)
					 + (_grad_u[_qp](2) + _grad_w_vel[_qp](1)) * _grad_test[_i][_qp](2);
    default:
      mooseError("type must be three_dimension or three_dimension");
  }
}

Real
StokesY::pressureTermY()
{
	return -_p[_qp] * _grad_test[_i][_qp](1);
}

Real
StokesY::surfaceTensionTermY(Dimension type)
{
  switch (type)
  {
    case two_dimension:
      return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](0)
                              + _grad_c[_qp](1) * _grad_c[_qp](1) * _grad_test[_i][_qp](1));
    case three_dimension:
      return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](0)
                              + _grad_c[_qp](1) * _grad_c[_qp](1) * _grad_test[_i][_qp](1)
                              + _grad_c[_qp](2) * _grad_c[_qp](1) * _grad_test[_i][_qp](2));
    default:
      mooseError("type must be three_dimension or three_dimension");
  }
}

Real
StokesY::ResidualY(Dimension type)
{
  switch (type)
  {
    case two_dimension:
      return -_mu_eff[_qp] * velocityTermY(Dimension::two_dimension) 
              + pressureTermY() + surfaceTensionTermY(Dimension::two_dimension);

    case three_dimension:
      return -_mu_eff[_qp] * velocityTermY(Dimension::three_dimension) 
              + pressureTermY() + surfaceTensionTermY(Dimension::three_dimension);
  
    default:
      mooseError("type must be three_dimension or three_dimension");
  }
}

Real 
StokesY::computeQpResidual()
{
	switch (_dim)
	{
		case 2:
			return ResidualY(Dimension::two_dimension);
		case 3:
			return ResidualY(Dimension::three_dimension);
		default:
			mooseError("Invalid dimension value, should be 2 or 3");
	}
}