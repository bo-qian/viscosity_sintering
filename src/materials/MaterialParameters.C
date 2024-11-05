/*
 * @Author: Bo Qian
 * @Date: 2024-10-21 09:54:40
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 14:35:01
 * @LastEditors: Bo Qian
 * @Description: 
 * @FilePath: /viscosity_sintering/src/materials/MaterialParameters.C
 */

#ifndef EXCLUDE_THIS_FILE

# include "MaterialParameters.h"

registerMooseObject("viscosity_sinteringApp", MaterialParameters);

InputParameters
MaterialParameters::validParams()
{
  InputParameters params = DerivativeMaterialInterface<Material>::validParams();
  params.addClassDescription("Material Parameters of Viscosity Sintering App by using DerivativeMaterialInterface");
  params.addParam<Real>("mu_volume", 0.4, "The volume viscosity of the system");
  params.addParam<Real>("mu_ratio", 0.001, "The ratio of the vapor viscosity to the volume viscosity");
  params.addParam<Real>("epsilon_Nc", 3.01, "epsilon in the interpolation function N(C)");
  params.addParam<Real>("M", 0.005, "The mobility of the system, or relaxation parameter");
  params.addParam<Real>("alpha", 120.00, "The alpha parameter of the system");
  params.addParam<Real>("kappa_C", 135.00, "The kappa_C parameter of the system");
  params.addRequiredCoupledVar("c", "Concentration variable");
	return params;
}

MaterialParameters::MaterialParameters(const InputParameters & parameters)
	: DerivativeMaterialInterface<Material>(parameters),
	_mu_volume(getParam<Real>("mu_volume")),
	_mu_ratio(getParam<Real>("mu_ratio")),
	_epsilon_Nc(getParam<Real>("epsilon_Nc")),
	_M(getParam<Real>("M")),
	_alpha(getParam<Real>("alpha")),
	_kappa_C(getParam<Real>("kappa_C")),
	_Nc(declareProperty<Real>("Nc")),
	_c(coupledValue("c")),
	_c_var(coupled("c")),
	_c_name(getVar("c", 0)->name()),
	_dNdc(declarePropertyDerivative<Real>("Nc", _c_name)),
	_F_loc(declareProperty<Real>("F_loc")),
	_dF_loc(declarePropertyDerivative<Real>("F_loc", _c_name)),
	_mu_eff(declareProperty<Real>("mu_eff")),
	_dmu_eff(declarePropertyDerivative<Real>("mu_eff", _c_name))
{

}

void
MaterialParameters::ComputeQpNc()
{
	// Compute N(C)
  _Nc[_qp] = _c[_qp]* _c[_qp] * (1 + 2 * (1 - _c[_qp]) + _epsilon_Nc * (1 - _c[_qp]) * (1 - _c[_qp]));
}

void
MaterialParameters::ComputeQpFloc()
{	
	// Compute f_loc
	_F_loc[_qp] = _alpha * _c[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]);
}

void
MaterialParameters::ComputeQpMuEff()
{
	// Compute mu_eff
	_mu_eff[_qp] = (_mu_ratio + (1 - _mu_ratio) * _Nc[_qp]) * _mu_volume;
}

#endif