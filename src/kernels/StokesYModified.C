/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 15:37:28
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-13 08:58:35
 * @FilePath: /viscosity_sintering/src/kernels/StokesYModified.C
 * @Description: Kernel of y-component of the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "StokesYModified.h"

registerMooseObject("viscosity_sinteringApp", StokesYModified);

InputParameters
StokesYModified::validParams()
{
  InputParameters params = Kernel::validParams();
  MooseEnum dims("2=2 3");
  params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
  params.addClassDescription("Kernel of y-component of the Stokes equation");
  params.addRequiredCoupledVar("x_velocity", "x-velocity variable");
  params.addCoupledVar("z_velocity", 0, "z-velocity variable");
  params.addRequiredCoupledVar("pressure", "Pressure variable");
  params.addRequiredCoupledVar("phase_field", "variant of phase field");
  params.addRequiredCoupledVar("chemical_potential", "chemical potential variable");
  return params;
}

StokesYModified::StokesYModified(const InputParameters & parameters)
  : Kernel(parameters),
    _dim(getParam<MooseEnum>("dim")),
    _mu_eff(getMaterialProperty<Real>("mu_eff")),
    _alpha(getMaterialProperty<Real>("alpha_value")),
    _kappa_c(getMaterialProperty<Real>("kappa_C_value")),
    _f_loc(getMaterialProperty<Real>("F_loc")),
    _f_grad(getMaterialProperty<Real>("F_grad")),
    _cvar(coupled("phase_field")),
    _c(coupledValue("phase_field")),
    _grad_c(coupledGradient("phase_field")),
    _mu(coupledValue("chemical_potential")),
    _grad_mu(coupledGradient("chemical_potential")),
    _pvar(coupled("pressure")),
    _p(coupledValue("pressure")),
    // Coupled variables
    _u_vel_var(coupled("x_velocity")),
    _u_vel(coupledValue("x_velocity")),
    _w_vel_var(coupled("z_velocity")),
    _w_vel(coupledValue("z_velocity")),
    _grad_u_vel(coupledGradient("x_velocity")),
    _grad_w_vel(coupledGradient("z_velocity"))
{
}

Real 
StokesYModified::velocityTermY()
{
  switch (_dim)
  {
    case 2:
      return (_grad_u[_qp](0) + _grad_u_vel[_qp](1)) * _grad_test[_i][_qp](0)
           + (_grad_u[_qp](1) + _grad_u[_qp](1)) * _grad_test[_i][_qp](1);
    case 3:
      return (_grad_u[_qp](0) + _grad_u_vel[_qp](1)) * _grad_test[_i][_qp](0)
           + (_grad_u[_qp](1) + _grad_u[_qp](1)) * _grad_test[_i][_qp](1)
           + (_grad_u[_qp](2) + _grad_w_vel[_qp](1)) * _grad_test[_i][_qp](2);
    default:
      mooseError("type must be three_dimension or three_dimension");
  }
}

Real
StokesYModified::pressureTermY()
{
  return (_p[_qp] - _c[_qp] * _mu[_qp] + (_f_loc[_qp] + _f_grad[_qp])) * _grad_test[_i][_qp](1);
}

Real
StokesYModified::surfaceTensionTermY()
{
  switch (_dim)
  {
    case 2:
      return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](0)
                            + _grad_c[_qp](1) * _grad_c[_qp](1) * _grad_test[_i][_qp](1));
    case 3:
      return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](0)
                            + _grad_c[_qp](1) * _grad_c[_qp](1) * _grad_test[_i][_qp](1)
                            + _grad_c[_qp](2) * _grad_c[_qp](1) * _grad_test[_i][_qp](2));
    default:
      mooseError("type must be three_dimension or three_dimension");
  }
}

Real 
StokesYModified::computeQpResidual()
{
  return _mu_eff[_qp] * velocityTermY() + pressureTermY() - surfaceTensionTermY();
}

Real
StokesYModified::computeQpJacobian()
{
  switch (_dim)
  {
    case 2:
      return _mu_eff[_qp] * (_grad_phi[_j][_qp](0) * _grad_test[_i][_qp](0)
                           + 2 * _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](1));
    case 3:
      return _mu_eff[_qp] * (_grad_phi[_j][_qp](0) * _grad_test[_i][_qp](0)
                           + 2 * _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](1)
                           + _grad_phi[_j][_qp](2) * _grad_test[_i][_qp](2));
    default:
      mooseError("Invalid dimension value, should be 2 or 3");
  }
}

Real
StokesYModified::computeQpOffDiagJacobian(unsigned jvar)
{
  switch (_dim)
  {
    case 2:
      if (jvar == _u_vel_var)
        return _mu_eff[_qp] * _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](0);

      if (jvar == _pvar)
        return _phi[_j][_qp] * _grad_test[_i][_qp](1);
      
      return 0.0;

    case 3:
      if (jvar == _u_vel_var)
        return _mu_eff[_qp] * _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](0);
      
      if (jvar == _w_vel_var)
        return _mu_eff[_qp] * _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](2);

      if (jvar == _pvar)
        return _phi[_j][_qp] * _grad_test[_i][_qp](1);
      
      return 0.0;
    default:
      mooseError("Invalid dimension value, should be 2 or 3");
  }
}