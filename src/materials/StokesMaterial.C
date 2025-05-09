/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-21 15:25:19
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:28:28
 * @FilePath: /viscosity_sintering/src/materials/StokesMaterial.C
 * @Description: Materials for Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "StokesMaterial.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

registerMooseObject("viscosity_sinteringApp", StokesMaterial);

InputParameters
StokesMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material Parameters of Stokes App");
  params.addParam<Real>("mu_volume", 0.4, "The volume viscosity of the system");
  params.addParam<Real>("mu_ratio", 0.001, "The ratio of the vapor viscosity to the volume viscosity");
  params.addParam<Real>("epsilon_Nc", 3.01, "epsilon in the interpolation function N(C)");
  params.addParam<Real>("alpha", 120.00, "The alpha parameter of the system");
  params.addParam<Real>("kappa_C", 135.00, "The kappa_C parameter of the system");
  params.addRequiredCoupledVar("cvar", "Concentration variable");
  params.addRequiredCoupledVar("x_velocity", "x-velocity variable");
  params.addRequiredCoupledVar("y_velocity", "y-velocity variable");
  params.addCoupledVar("z_velocity", 0, "z-velocity variable");
  params.addRequiredCoupledVar("pressure", "Pressure variable");
  return params;
}

StokesMaterial::StokesMaterial(const InputParameters & parameters)
  : DerivativeMaterialInterface<Material>(parameters),
    _mu_volume(getParam<Real>("mu_volume")),
    _mu_ratio(getParam<Real>("mu_ratio")),
    _epsilon_Nc(getParam<Real>("epsilon_Nc")),
    _alpha(getParam<Real>("alpha")),
    _kc(getParam<Real>("kappa_C")),
    _c(coupledValue("cvar")),
    _u(coupledValue("x_velocity")),
    _v(coupledValue("y_velocity")),
    _w(coupledValue("z_velocity")),
    _p(coupledValue("pressure")),
    _grad_c(coupledGradient("cvar")),
    _grad_u(coupledGradient("x_velocity")),
    _grad_v(coupledGradient("y_velocity")),
    _grad_w(coupledGradient("z_velocity")),
    _grad_p(coupledGradient("pressure")),
    _Nc(declareProperty<Real>("Nc")),
    _mu_eff(declareProperty<Real>("mu_eff")),
    _kappa_C(declareProperty<Real>("kappa_C_value")),
    _alpha_var(declareProperty<Real>("alpha_value"))
{
}

void
StokesMaterial::computeQpProperties()
{
  // Compute N(C)
  _Nc[_qp] = _c[_qp] * _c[_qp] * (1 + 2 * (1 - _c[_qp]) + _epsilon_Nc * (1 - _c[_qp]) * (1 - _c[_qp]));

  // Compute mu_eff
  _mu_eff[_qp] = _mu_volume * (_mu_ratio + (1 - _mu_ratio) * _Nc[_qp]);

  // Compute kappa_C
  _kappa_C[_qp] = _kc;

  // Compute alpha
  _alpha_var[_qp] = _alpha;
}