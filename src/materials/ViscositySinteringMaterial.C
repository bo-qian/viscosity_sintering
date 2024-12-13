/*
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:43
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-12 15:15:40
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
	params.addRequiredParam<Real>("delta", "The thinkness of  interface");
	params.addRequiredParam<Real>("radius", "The radius of the particles");
	params.addRequiredParam<int>("number_x", "The number of particles in the x direction");
	params.addRequiredParam<int>("number_y", "The number of particles in the y direction");
	params.addRequiredParam<Real>("omega", "The omega coordinate of the center of the particles");
	params.addRequiredParam<std::vector<int>>("domain", "The domain size as a vector {width, height}");

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
	_kappa_C(getParam<Real>("kappa_C")),

	_delta(getParam<Real>("delta")), 
  _radius(getParam<Real>("radius")), 
  _number_x(getParam<int>("number_x")),
  _number_y(getParam<int>("number_y")),
  _omega(getParam<Real>("omega")),
  _domain(getParam<std::vector<int>>("domain")), // 获取domain参数

	_c(coupledValue("cvar")),
	_c_var(coupled("cvar")),
	_c_name(getVar("cvar", 0)->name()),
	_Nc(declareProperty<Real>("Nc")),
	_dNdc(declareProperty<Real>("dNc")),
	_F_loc(declareProperty<Real>("F_loc")),
	_dF_loc(declareProperty<Real>("dF_loc")),
	_dF2_loc(declareProperty<Real>("dF2_loc")),
	_mu_eff(declareProperty<Real>("mu_eff")),
	_dmu_eff(declareProperty<Real>("dmu_eff")),
	_kappa_c(declareProperty<Real>("kappa_C")),
	_mobility(declareProperty<Real>("M")),
	_C_IC(declareProperty<Real>("C_IC"))
{

	// Compute total number of particles
	int particle_number_total = _number_x * _number_y;

	// Call the particle centers function
	auto result = particleCentersWithoutTemplate(_radius, particle_number_total, _number_x, _number_y, _domain);

	// Store the results in the class members
	_particle_centers_coordinate = result.first;
	_particle_radius = result.second;

	_C_initial.resize(_domain[0] * _domain[1], 0.0);
	for (int y = 0; y < _domain[1]; ++y)
	{
		for (int x = 0; x < _domain[0]; ++x)
		{
			Real total_value = 0.0;
			for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
			{
				double distance = std::sqrt(
					std::pow(x - _particle_centers_coordinate[i].first, 2) +
					std::pow(y - _particle_centers_coordinate[i].second, 2)) - _particle_radius[i];

				double argument = distance * 2 * std::atanh(1 - 2 * _omega) / _delta;
				total_value += 0.5 * (1.0 - std::tanh(argument));
			}
			_C_initial[y * _domain[0] + x] = total_value;
		}
	}




	// 静态变量确保只打印一次
  static bool isPrinted = false;
  if (!isPrinted)
  {
		// Print the particle centers and radii
    std::cout << std::endl;
		std::cout << "Particle centers and radii" << std::endl;
		std::cout << "---------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(25) << "Center" 
              << std::setw(25) << "Radius" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
    {
        std::cout << std::setw(25) << "(" << _particle_centers_coordinate[i].first 
                  << ", " << _particle_centers_coordinate[i].second << ")"
                  <<  std::setw(25) << _particle_radius[i] << std::endl;
    }
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << std::endl;

    // 打印表头
		std::cout << "Material Parameters of Viscosity Sintering App" << std::endl;
		std::cout << "---------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(25) << "Parameter" 
              << std::setw(25) << "Value" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;

    // 打印物料参数
    std::cout << std::setw(25) << "mu_volume" 
              << std::setw(25) << _mu_volume << std::endl;
    std::cout << std::setw(25) << "mu_ratio" 
              << std::setw(25) << _mu_ratio << std::endl;
    std::cout << std::setw(25) << "epsilon_Nc" 
              << std::setw(25) << _epsilon_Nc << std::endl;
    std::cout << std::setw(25) << "mobility" 
              << std::setw(25) << _M << std::endl;
    std::cout << std::setw(25) << "alpha" 
              << std::setw(25) << _alpha << std::endl;
    std::cout << std::setw(25) << "kappa_C" 
              << std::setw(25) << _kappa_C << std::endl;
		
		std::cout << "---------------------------------------------------" << std::endl;
		std::cout << std::endl;

    // 设置标志位，防止后续打印
    isPrinted = true;
	} 
}

std::pair<std::vector<std::pair<int, int>>, std::vector<int>> 
ViscositySinteringMaterial::particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, const std::vector<int>& domain)
{
	std::vector<int> particle_radius(particle_number_total, radius_particle);
	std::vector<std::pair<int, int>> particle_centers_coordinate;

	for (int j = 0; j < number_y; ++j) 
	{
		for (int i = 0; i < number_x; ++i) 
		{
			float x_coordinate = domain[0] / 2.0f + (i + (1.0f - number_x) / 2.0f) * radius_particle * 2.0f;
			float y_coordinate = domain[1] / 2.0f + (j + (1.0f - number_y) / 2.0f) * radius_particle * 2.0f;
			particle_centers_coordinate.push_back({static_cast<int>(x_coordinate), static_cast<int>(y_coordinate)});
		}
	}

	return {particle_centers_coordinate, particle_radius};
}


void
ViscositySinteringMaterial::computeQpProperties(const Point & p)
{

	// Compute N(C)
	_Nc[_qp] = _c[_qp] * _c[_qp] * (1 + 2 * (1 - _c[_qp]) + _epsilon_Nc * (1 - _c[_qp]) * (1 - _c[_qp]));

	// Compute dNdc
	_dNdc[_qp] = 2 * _c[_qp] * (1 - _c[_qp]) * (3 + _epsilon_Nc * (1 - 2 * _c[_qp]));

	// Compute F_loc
	_F_loc[_qp] = _alpha * _c[_qp] * (1 - _c[_qp]) * (1 - _c[_qp]);

	// Compute dF_loc
	_dF_loc[_qp] = 2 * _alpha * _c[_qp] * (1 - _c[_qp]) * (1 - 2 * _c[_qp]);

	// Compute dF2_loc
	_dF2_loc[_qp] = 2 * _alpha * (1 - 2 * _c[_qp]) * (1 - 2 * _c[_qp]) - 4 * _alpha * _c[_qp] * (1 - _c[_qp]);

	// Compute mu_eff
	_mu_eff[_qp] = _mu_volume * (_mu_ratio + (1 - _mu_ratio) * _Nc[_qp]);
	
	// Compute dmu_eff
	_dmu_eff[_qp] = _mu_volume * (1 - _mu_ratio) * _dNdc[_qp];

	// compute kappa_C
	_kappa_c[_qp] = _kappa_C;

	_mobility[_qp] = _M;

	int x_index = static_cast<int>(p(0));
	int y_index = static_cast<int>(p(1));

	// Use precomputed C initial value
	_C_IC[_qp] = _C_initial[y_index * _domain[0] + x_index];	
}