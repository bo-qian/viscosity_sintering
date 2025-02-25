/*
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:43
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-21 15:21:28
 * @LastEditors: Bo Qian
 * @Description: Materials for Viscosity Sintering App
 * @FilePath: /viscosity_sintering/src/materials/ViscositySinteringMaterial.C
 */

#include "ViscositySinteringMaterial.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

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
	params.addParam<Real>("theta", 0.5, "The theta value of the system");

  params.addCoupledVar("cvar", "Concentration variable");
  return params;
}

ViscositySinteringMaterial::ViscositySinteringMaterial(const InputParameters & parameters)
	: DerivativeMaterialInterface<Material>(parameters),
	_mu_volume(getParam<Real>("mu_volume")),
	_mu_ratio(getParam<Real>("mu_ratio")),
	_epsilon_Nc(getParam<Real>("epsilon_Nc")),
	_M(getParam<Real>("M")),
	_alpha(getParam<Real>("alpha")),
	_kc(getParam<Real>("kappa_C")),
	_theta(getParam<Real>("theta")),

	_c(coupledValue("cvar")),
	_c_old(coupledValueOld("cvar")),
	
	_Nc(declareProperty<Real>("Nc")),
	_dNdc(declareProperty<Real>("dNc")),
	_F_loc(declareProperty<Real>("F_loc")),
	_dF_loc(declareProperty<Real>("dF_loc")),
	_dF2_loc(declareProperty<Real>("dF2_loc")),
	_mu_eff(declareProperty<Real>("mu_eff")),
	_dmu_eff(declareProperty<Real>("dmu_eff")),
	_alpha_var(declareProperty<Real>("alpha_value")),
	_kappa_C(declareProperty<Real>("kappa_C_value")),
	_mobility(declareProperty<Real>("M_value")),
	_theta_var(declareProperty<Real>("theta_value")),
	_mu_vol(declareProperty<Real>("mu_volume_value")),
	_mu_rat(declareProperty<Real>("mu_ratio_value")),
	_epsilon_Nc_var(declareProperty<Real>("epsilon_Nc_value"))
{
	// 静态变量确保只打印一次
  // static bool isPrinted = false;
  // if (!isPrinted)
  // {
	// 	// Print the particle centers and radii
  //   std::cout << std::endl;
	// 	std::cout << "Particle centers and radii" << std::endl;
	// 	std::cout << "---------------------------------------------------" << std::endl;
  //   std::cout << std::left << std::setw(25) << "Center" 
  //             << std::setw(25) << "Radius" << std::endl;
  //   std::cout << "---------------------------------------------------" << std::endl;
  //   for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
  //   {
  //       std::cout << std::setw(25) << "(" << _particle_centers_coordinate[i].first 
  //                 << ", " << _particle_centers_coordinate[i].second << ")"
  //                 <<  std::setw(25) << _particle_radius[i] << std::endl;
  //   }
  //   std::cout << "---------------------------------------------------" << std::endl;
  //   std::cout << std::endl;

  //   // 打印表头
	// 	std::cout << "Material Parameters of Viscosity Sintering App" << std::endl;
	// 	std::cout << "---------------------------------------------------" << std::endl;
  //   std::cout << std::left << std::setw(25) << "Parameter" 
  //             << std::setw(25) << "Value" << std::endl;
  //   std::cout << "---------------------------------------------------" << std::endl;

  //   // 打印物料参数
  //   std::cout << std::setw(25) << "mu_volume" 
  //             << std::setw(25) << _mu_volume << std::endl;
  //   std::cout << std::setw(25) << "mu_ratio" 
  //             << std::setw(25) << _mu_ratio << std::endl;
  //   std::cout << std::setw(25) << "epsilon_Nc" 
  //             << std::setw(25) << _epsilon_Nc << std::endl;
  //   std::cout << std::setw(25) << "mobility" 
  //             << std::setw(25) << _M << std::endl;
  //   std::cout << std::setw(25) << "alpha" 
  //             << std::setw(25) << _alpha << std::endl;
  //   std::cout << std::setw(25) << "kappa_C" 
  //             << std::setw(25) << _kappa_C << std::endl;
		
	// 	std::cout << "---------------------------------------------------" << std::endl;
	// 	std::cout << std::endl;

  //   // 设置标志位，防止后续打印
  //   isPrinted = true;
	// } 
}

void
ViscositySinteringMaterial::computeQpProperties()
{
	// std::cout << "Computing C Properties" << std::endl;
	// std::cout << _c[_qp] << std::endl;

	// Compute N(C)
	_Nc[_qp] = _c[_qp] * _c[_qp] * (1 + 2 * (1 - _c[_qp]) + _epsilon_Nc * (1 - _c[_qp]) * (1 - _c[_qp]));

	// Compute dNdc
	_dNdc[_qp] = 2 * _c[_qp] * (1 - _c[_qp]) * (3 + _epsilon_Nc * (1 - 2 * _c[_qp]));

	// Compute F_loc
	_F_loc[_qp] = _alpha * _c[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]);

	// Compute dF_loc
	// _dF_loc[_qp] = 2 * _alpha * _c[_qp] * (1 - _c[_qp]) * (1 - 2 * _c[_qp]);
	_dF_loc[_qp] = 2 * _alpha * _c_old[_qp] * (1 - _c_old[_qp]) * (1 - 2 * _c_old[_qp]);

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

	_mobility[_qp] = _M;

	_theta_var[_qp] = _theta;

	_mu_vol[_qp] = _mu_volume;

	_mu_rat[_qp] = _mu_ratio;

	_epsilon_Nc_var[_qp] = _epsilon_Nc;

	// std::cout << "kc" << std::endl;
	// std::cout << _kappa_C[_qp] << std::endl;
	
}