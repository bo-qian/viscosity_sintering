/*
 * @Author: Bo Qian
 * @Date: 2024-12-12 15:19:30
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-19 10:22:38
 * @LastEditors: Bo Qian
 * @Description: 
 * @FilePath: /viscosity_sintering/src/kernels/InitialConditionC.C
 */

#include "InitialConditionC.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

registerMooseObject("viscosity_sinteringApp", InitialConditionC);

InputParameters
InitialConditionC::validParams()
{
    InputParameters params = Kernel::validParams();
    params.addRequiredParam<Real>("delta", "The thinkness of  interface");
    params.addRequiredParam<Real>("radius", "The radius of the particles");
    params.addRequiredParam<int>("number_x", "The number of particles in the x direction");
    params.addRequiredParam<int>("number_y", "The number of particles in the y direction");
    params.addRequiredParam<Real>("omega", "The omega coordinate of the center of the particles");
    params.addRequiredParam<std::vector<int>>("domain", "The domain size as a vector {width, height}");
    return params;
}

InitialConditionC::InitialConditionC(const InputParameters & parameters)
  : Kernel(parameters),
  _delta(getParam<Real>("delta")), 
  _radius(getParam<Real>("radius")), 
  _number_x(getParam<int>("number_x")),
  _number_y(getParam<int>("number_y")),
  _omega(getParam<Real>("omega")),
  _domain(getParam<std::vector<int>>("domain")) // 获取domain参数
  // _C_Initial(declareProperty<Real>("C_initial"))
{
  // Compute total number of particles
  int particle_number_total = _number_x * _number_y;

  // Call the particle centers function
  auto result = particleCentersWithoutTemplate(_radius, particle_number_total, _number_x, _number_y, _domain);

  // Store the results in the class members
  _particle_centers_coordinate = result.first;
  _particle_radius = result.second;

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
}

std::pair<std::vector<std::pair<int, int>>, std::vector<int>> 
InitialConditionC::particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, const std::vector<int>& domain)
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

// void
// InitialConditionC::computeQpProperties(const Point & p)
// {
//   Real total_value = 0.0;
//   // Iterate over all particle centers
//   for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
//   {
//     // Compute the Euclidean distance between (x, y) and the current particle center
//     double distance = std::sqrt(
//         std::pow(p(0) - _particle_centers_coordinate[i].first, 2) + 
//         std::pow(p(1) - _particle_centers_coordinate[i].second, 2)) - _particle_radius[i];
    
//     // Compute the argument of the tanh function
//     double argument = distance * 2 * std::atanh(1 - 2 * _omega) / _delta;
    
//     // Compute the value for this particle and add it to the total value
//     total_value += 0.5 * (1.0 - std::tanh(argument));
//   }
//   _C_Initial[_qp] = total_value;
// }

Real
InitialConditionC::value(const Point & p)
{
    // Real total_value = 0.0;

    // // Iterate over all particle centers
    // for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
    // {
    //     // Compute the Euclidean distance between (x, y) and the current particle center
    //     double distance = std::sqrt(
    //         std::pow(p(0) - _particle_centers_coordinate[i].first, 2) + 
    //         std::pow(p(1) - _particle_centers_coordinate[i].second, 2)) - _particle_radius[i];
        
    //     // Compute the argument of the tanh function
    //     double argument = distance * 2 * std::atanh(1 - 2 * _omega) / _delta;
        
    //     // Compute the value for this particle and add it to the total value
    //     total_value += 0.5 * (1.0 - std::tanh(argument));
    // }

    // // _C_Initial[_qp] = total_value;

    // return total_value;

  Real total_value = 0.0;

  for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
  {
      Real dx = p(0) - _particle_centers_coordinate[i].first;
      Real dy = p(1) - _particle_centers_coordinate[i].second;
      Real distance = std::sqrt(dx * dx + dy * dy) - _particle_radius[i];

      if (_omega <= 0.5 && _omega >= 0.0)
      {
          Real argument = distance * 2 * std::atanh(1 - 2 * _omega) / _delta;
          total_value += 0.5 * (1.0 - std::tanh(argument));
      }
      else
      {
          mooseError("Invalid omega value. It must be between 0 and 0.5.");
      }
  }

  return total_value;
}

Real
InitialConditionC::computeQpResidual()
{
  return _u[_qp] - value(_q_point[_qp]);
}

Real
InitialConditionC::computeQpJacobian()
{
  return _phi[_j][_qp];
}