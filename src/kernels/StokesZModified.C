/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 15:37:37
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:34:37
 * @FilePath: /viscosity_sintering/src/kernels/StokesZModified.C
 * @Description: Kernel of z-component of the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "StokesZModified.h"

registerMooseObject("viscosity_sinteringApp", StokesZModified);

InputParameters
StokesZModified::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of x-component of the Stokes equation");
  params.addRequiredCoupledVar("x_velocity", "x-velocity variable");
  params.addRequiredCoupledVar("y_velocity", "y-velocity variable");
  params.addRequiredCoupledVar("pressure", "Pressure variable");
  params.addRequiredCoupledVar("phase_field", "variant of phase field");
  params.addRequiredCoupledVar("chemical_potential", "chemical potential variable");
  return params;
}

StokesZModified::StokesZModified(const InputParameters & parameters)
  : Kernel(parameters),
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
    _u_vel_var(coupled("x_velocity")),
    _u_vel(coupledValue("x_velocity")),
    _v_vel_var(coupled("y_velocity")),
    _v_vel(coupledValue("y_velocity")),
    _grad_u_vel(coupledGradient("x_velocity")),
    _grad_v_vel(coupledGradient("y_velocity"))
{
}

Real 
StokesZModified::velocityTermZ()
{
  return (_grad_u[_qp](0) + _grad_u_vel[_qp](2)) * _grad_test[_i][_qp](0)
       + (_grad_u[_qp](1) + _grad_v_vel[_qp](2)) * _grad_test[_i][_qp](1)
       + (_grad_u[_qp](2) + _grad_u[_qp](2)) * _grad_test[_i][_qp](2);
}

Real
StokesZModified::pressureTermZ()
{
  const Real f = _alpha[_qp] * _c[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]) +
                 0.5 * _kappa_c[_qp] * _grad_c[_qp].norm_sq();
  return (_p[_qp] - _c[_qp] * _mu[_qp] + f) * _grad_test[_i][_qp](2);
}

Real
StokesZModified::surfaceTensionTermZ()
{
  return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_c[_qp](2) * _grad_test[_i][_qp](0)
                        + _grad_c[_qp](1) * _grad_c[_qp](2) * _grad_test[_i][_qp](1)
                        + _grad_c[_qp](2) * _grad_c[_qp](2) * _grad_test[_i][_qp](2));
}

Real 
StokesZModified::computeQpResidual()
{
  return _mu_eff[_qp] * velocityTermZ() + pressureTermZ() - surfaceTensionTermZ();
}

Real
StokesZModified::computeQpJacobian()
{
  return _mu_eff[_qp] * (_grad_phi[_j][_qp](0) * _grad_test[_i][_qp](0)
                       + _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](1)
                       + 2 * _grad_phi[_j][_qp](2) * _grad_test[_i][_qp](2));
}

Real
StokesZModified::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _u_vel_var)
    return _mu_eff[_qp] * _grad_phi[_j][_qp](2) * _grad_test[_i][_qp](0);
  
  if (jvar == _v_vel_var)
    return _mu_eff[_qp] * _grad_phi[_j][_qp](2) * _grad_test[_i][_qp](1);

  if (jvar == _pvar)
    return _phi[_j][_qp] * _grad_test[_i][_qp](2);
  
  return 0.0;
}