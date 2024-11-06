/*
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:43
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-06 22:59:00
 * @LastEditors: Bo Qian
 * @Description: Materials for Viscosity Sintering App
 * @FilePath: /viscosity_sintering/src/materials/ViscositySinteringMaterial.C
 */

#include "ViscositySinteringMaterial.h"

registerMooseObject("viscosity_sinteringApp", ViscositySinteringMaterial);

InputParameters
ViscositySinteringMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material Parameters of Viscosity Sintering App");
  params.addParam<Real>("mu_volume", 0.4, "The volume viscosity of the system");
  params.addParam<Real>("mu_ratio", 0.001, "The ratio of the vapor viscosity to the volume viscosity");
  params.addParam<Real>("epsilon_Nc", 3.01, "epsilon in the interpolation function N(C)");
  params.addParam<Real>("M", 0.005, "The mobility of the system, or relaxation parameter");
  params.addParam<Real>("alpha", 120.00, "The alpha parameter of the system");
  params.addParam<Real>("kappa_C", 135.00, "The kappa_C parameter of the system");
	// params.addParam<Real>("dimension", "Only can choose 2 or 3");
  params.addRequiredCoupledVar("c", "Concentration variable");
  return params;
}

ViscositySinteringMaterial::ViscositySinteringMaterial(const InputParameters & parameters)
	: DerivativeMaterialInterface<Material>(parameters),
	_mu_volume(getParam<Real>("mu_volume")),
	_mu_ratio(getParam<Real>("mu_ratio")),
	_epsilon_Nc(getParam<Real>("epsilon_Nc")),
	_M(getParam<Real>("M")),
	_alpha(getParam<Real>("alpha")),
	_kappa_C(getParam<Real>("kappa_C")),
	// _dimension(getParam<Real>("dimension")),
	_Nc(declareProperty<Real>("Nc")),
	_c(coupledValue("c")),
	_c_var(coupled("c")),
	_c_name(getVar("c", 0)->name()),
	_dNdc(declarePropertyDerivative<Real>("Nc", _c_name)),
	_F_loc(declareProperty<Real>("F_loc")),
	_dF_loc(declarePropertyDerivative<Real>("F_loc", _c_name)),
	_mu_eff(declareProperty<Real>("mu_eff")),
	_dmu_eff(declarePropertyDerivative<Real>("mu_eff", _c_name)),
	_kappa_c(declareProperty<Real>("kappa_C"))
{
}

void
ViscositySinteringMaterial::computeQpProperties()
{
	// Compute N(C)
	_Nc[_qp] = _c[_qp] * _c[_qp] * (1 + 2 * (1 - _c[_qp]) + _epsilon_Nc * (1 - _c[_qp]) * (1 - _c[_qp]));

	// Compute F_loc
	_F_loc[_qp] = _alpha * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]);

	// Compute mu_eff
	_mu_eff[_qp] = _mu_volume + _mu_ratio * _c[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]);

	// compute kappa_C
	_kappa_c[_qp] = _kappa_C;
}