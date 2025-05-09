/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 15:37:26
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:35:24
 * @FilePath: /viscosity_sintering/src/kernels/StokesXModified.C
 * @Description: Kernel of x-component of the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */


#include "StokesXModified.h"

registerMooseObject("viscosity_sinteringApp", StokesXModified);

InputParameters
StokesXModified::validParams()
{
  InputParameters params = Kernel::validParams();
  MooseEnum dims("2=2 3");
  params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
  params.addClassDescription("Kernel of x-component of the Stokes equation");
  params.addRequiredCoupledVar("y_velocity", "y-velocity variable"); // required in 2D and 3D
  params.addCoupledVar("z_velocity", 0, "z-velocity variable"); // only required in 3D
  params.addRequiredCoupledVar("pressure", "Pressure variable");
  params.addRequiredCoupledVar("phase_field", "variant of phase field");
  params.addRequiredCoupledVar("chemical_potential", "chemical potential variable");
  return params;
}

StokesXModified::StokesXModified(const InputParameters & parameters)
  : Kernel(parameters),
    _dim(getParam<MooseEnum>("dim")),
    _mu_eff(getMaterialProperty<Real>("mu_eff")),
    _alpha(getMaterialProperty<Real>("alpha_value")),
    _kappa_c(getMaterialProperty<Real>("kappa_C_value")),
    _cvar(coupled("phase_field")),
    _c(coupledValue("phase_field")),
    _grad_c(coupledGradient("phase_field")),
    _mu(coupledValue("chemical_potential")),
    _grad_mu(coupledGradient("chemical_potential")),
    _pvar(coupled("pressure")),
    _p(coupledValue("pressure")),
    // Coupled variables
    _v_vel_var(coupled("y_velocity")),
    _v_vel(coupledValue("y_velocity")),
    _w_vel_var(coupled("z_velocity")),
    _w_vel(coupledValue("z_velocity")),
    _grad_v_vel(coupledGradient("y_velocity")),
    _grad_w_vel(coupledGradient("z_velocity"))
{
}

Real
StokesXModified::velocityTermX()
{
  switch (_dim)
  {
    case 2:
      return (_grad_u[_qp](0) + _grad_u[_qp](0)) * _grad_test[_i][_qp](0)
           + (_grad_u[_qp](1) + _grad_v_vel[_qp](0)) * _grad_test[_i][_qp](1);
    case 3:
      return (_grad_u[_qp](0) + _grad_u[_qp](0)) * _grad_test[_i][_qp](0)
           + (_grad_u[_qp](1) + _grad_v_vel[_qp](0)) * _grad_test[_i][_qp](1)
           + (_grad_u[_qp](2) + _grad_w_vel[_qp](0)) * _grad_test[_i][_qp](2);
    default:
      mooseError("Invalid dimension value, should be 2 or 3");
  }
}

Real
StokesXModified::pressureTermX()
{
  const Real f = _alpha[_qp] * _c[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]) +
                 0.5 * _kappa_c[_qp] * _grad_c[_qp].norm_sq();
  return (_p[_qp] - _c[_qp] * _mu[_qp] + f) * _grad_test[_i][_qp](0);
}

Real
StokesXModified::surfaceTensionTermX()
{
  switch (_dim)
  {
    case 2:
      return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](0) * _grad_test[_i][_qp](0)
                            + _grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](1));
    case 3:
      return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](0) * _grad_test[_i][_qp](0)
                            + _grad_c[_qp](0) * _grad_c[_qp](1) * _grad_test[_i][_qp](1)
                            + _grad_c[_qp](0) * _grad_c[_qp](2) * _grad_test[_i][_qp](2));
    default:
      mooseError("Invalid dimension value, should be 2 or 3");
  }
}

Real 
StokesXModified::computeQpResidual()
{
  return _mu_eff[_qp] * velocityTermX() + pressureTermX() - surfaceTensionTermX();
}

Real 
StokesXModified::computeQpJacobian()
{
  switch (_dim)
  {
    case 2:
      return _mu_eff[_qp] * (2 * _grad_phi[_j][_qp](0) * _grad_test[_i][_qp](0)
                           + _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](1));
    case 3:
      return _mu_eff[_qp] * (2 * _grad_phi[_j][_qp](0) * _grad_test[_i][_qp](0)
                           + _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](1)
                           + _grad_phi[_j][_qp](2) * _grad_test[_i][_qp](2));
    default:
      mooseError("Invalid dimension value, should be 2 or 3");
  }
}

Real
StokesXModified::computeQpOffDiagJacobian(unsigned jvar)
{
  switch (_dim)
  {
    case 2:
      if (jvar == _v_vel_var)
        return _mu_eff[_qp] * _grad_phi[_j][_qp](0) * _grad_test[_i][_qp](1);

      if (jvar == _pvar)
        return _phi[_j][_qp] * _grad_test[_i][_qp](0);
      
      return 0.0;

    case 3:
      if (jvar == _v_vel_var)
        return _mu_eff[_qp] * _grad_phi[_j][_qp](0) * _grad_test[_i][_qp](1);
      
      if (jvar == _w_vel_var)
        return _mu_eff[_qp] * _grad_phi[_j][_qp](0) * _grad_test[_i][_qp](2);

      if (jvar == _pvar)
        return _phi[_j][_qp] * _grad_test[_i][_qp](0);
      
      return 0.0;
    default:
      mooseError("Invalid dimension value, should be 2 or 3");
  }
}
