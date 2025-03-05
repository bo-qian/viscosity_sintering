/*
 * @Author: Bo Qian
 * @Date: 2025-02-21 15:25:19
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-03-05 18:37:59
 * @LastEditors: Bo Qian
 * @Description: Materials for Stokes equation
 * @FilePath: /viscosity_sintering/src/materials/StokesMaterial.C
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
	_dNdc(declareProperty<Real>("dNc")),
	_F_loc(declareProperty<Real>("F_loc")),
	_dF_loc(declareProperty<Real>("dF_loc")),
	_dF2_loc(declareProperty<Real>("dF2_loc")),
	_mu_eff(declareProperty<Real>("mu_eff")),
	_dmu_eff(declareProperty<Real>("dmu_eff")),
	_alpha_var(declareProperty<Real>("alpha_value")),
	_kappa_C(declareProperty<Real>("kappa_C_value")),
	_mu_vol(declareProperty<Real>("mu_volume_value")),
	_mu_rat(declareProperty<Real>("mu_ratio_value")),
	_epsilon_Nc_var(declareProperty<Real>("epsilon_Nc_value")),
	_stress_xx(declareProperty<Real>("stress_xx")),
	_stress_xy(declareProperty<Real>("stress_xy")),
	_stress_xz(declareProperty<Real>("stress_xz")),
	_stress_yx(declareProperty<Real>("stress_yx")),
	_stress_yy(declareProperty<Real>("stress_yy")),
	_stress_yz(declareProperty<Real>("stress_yz")),
	_stress_zx(declareProperty<Real>("stress_zx")),
	_stress_zy(declareProperty<Real>("stress_zy")),
	_stress_zz(declareProperty<Real>("stress_zz")),
	_stress(declareProperty<RankTwoTensor>("stress"))
{
  
}

void
StokesMaterial::computeQpProperties()
{
	// Compute N(C)
	_Nc[_qp] = _c[_qp] * _c[_qp] * (1 + 2 * (1 - _c[_qp]) + _epsilon_Nc * (1 - _c[_qp]) * (1 - _c[_qp]));

	// Compute dNdc
	_dNdc[_qp] = 2 * _c[_qp] * (1 - _c[_qp]) * (3 + _epsilon_Nc * (1 - 2 * _c[_qp]));

	// Compute F_loc
	_F_loc[_qp] = _alpha * _c[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]);

	// Compute dF_loc
	_dF_loc[_qp] = 2 * _alpha * _c[_qp] * (1 - _c[_qp]) * (1 - 2 * _c[_qp]);
	// _dF_loc[_qp] = 2 * _alpha * _c_old[_qp] * (1 - _c_old[_qp]) * (1 - 2 * _c_old[_qp]);

	// Compute dF2_loc
	_dF2_loc[_qp] = 2 * _alpha * (1 - 2 * _c[_qp]) * (1 - 2 * _c[_qp]) - 4 * _alpha * _c[_qp] * (1 - _c[_qp]);

	// Compute mu_eff
	_mu_eff[_qp] = _mu_volume * (_mu_ratio + (1 - _mu_ratio) * _Nc[_qp]);
	
	// Compute dmu_eff
	_dmu_eff[_qp] = _mu_volume * (1 - _mu_ratio) * _dNdc[_qp];

  // compute alpha_var
	_alpha_var[_qp] = _alpha;

	// compute kappa_C
	_kappa_C[_qp] = _kc;

	_mu_vol[_qp] = _mu_volume;

	_mu_rat[_qp] = _mu_ratio;

	_epsilon_Nc_var[_qp] = _epsilon_Nc;

	_stress_xx[_qp] = 2 * _mu_eff[_qp] * _grad_u[_qp](0) + (_p[_qp] - _F_loc[_qp] - 0.5 * _kappa_C[_qp] * _grad_c[_qp].norm_sq());
	_stress_xy[_qp] = _mu_eff[_qp] * (_grad_v[_qp](0) + _grad_u[_qp](1));
	_stress_xz[_qp] = _mu_eff[_qp] * (_grad_w[_qp](0) + _grad_u[_qp](2));
	_stress_yx[_qp] = _mu_eff[_qp] * (_grad_u[_qp](1) + _grad_v[_qp](0));
	_stress_yy[_qp] = 2 * _mu_eff[_qp] * _grad_v[_qp](1) + (_p[_qp] - _F_loc[_qp] - 0.5 * _kappa_C[_qp] * _grad_c[_qp].norm_sq());
	_stress_yz[_qp] = _mu_eff[_qp] * (_grad_w[_qp](1) + _grad_v[_qp](2));
	_stress_zx[_qp] = _mu_eff[_qp] * (_grad_u[_qp](2) + _grad_w[_qp](0));
	_stress_zy[_qp] = _mu_eff[_qp] * (_grad_v[_qp](2) + _grad_w[_qp](1));
	_stress_zz[_qp] = 2 * _mu_eff[_qp] * _grad_w[_qp](2) + (_p[_qp] - _F_loc[_qp] - 0.5 * _kappa_C[_qp] * _grad_c[_qp].norm_sq());
	_stress[_qp] = RankTwoTensor(
		_stress_xx[_qp], _stress_xy[_qp], _stress_xz[_qp],
		_stress_yx[_qp], _stress_yy[_qp], _stress_yz[_qp],
		_stress_zx[_qp], _stress_zy[_qp], _stress_zz[_qp]
	);
}