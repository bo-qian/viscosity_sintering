/*
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:42:40
 * @LastEditors: Bo Qian
 * @Description: 2D Initial Condition for MultiParticles
 * @FilePath: /viscosity_sintering/src/ics/MultiParticles_2D.C
 */

#include "MultiParticles_2D.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

registerMooseObject("viscosity_sinteringApp", MultiParticles_2D);

InputParameters
MultiParticles_2D::validParams()
{
    InputParameters params = InitialCondition::validParams();
    params.addRequiredParam<Real>("delta", "The thinkness of  interface");
    params.addRequiredParam<Real>("radius", "The radius of the particles");
    params.addRequiredParam<int>("number_x", "The number of particles in the x direction");
    params.addRequiredParam<int>("number_y", "The number of particles in the y direction");
    params.addRequiredParam<Real>("omega", "The omega coordinate of the center of the particles");
    params.addRequiredParam<std::vector<int>>("domain", "The domain size as a vector {width, height}");
    return params;
}

MultiParticles_2D::MultiParticles_2D(const InputParameters & parameters)
  : InitialCondition(parameters), 
  _delta(getParam<Real>("delta")), 
  _radius(getParam<Real>("radius")), 
  _number_x(getParam<int>("number_x")),
  _number_y(getParam<int>("number_y")),
  _omega(getParam<Real>("omega")),
  _domain(getParam<std::vector<int>>("domain")) // 获取domain参数
{
    // Compute total number of particles
    int particle_number_total = _number_x * _number_y;

    // Call the particle centers function
    auto result = particleCentersWithoutTemplate(_radius, particle_number_total, _number_x, _number_y, _domain);

    // Store the results in the class members
    _particle_centers_coordinate = result.first;
    _particle_radius = result.second;

    // Print the particle centers and radii
    std::cout << "Particle centers and radii:" << std::endl;
    for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
    {
        std::cout << "Center: (" << _particle_centers_coordinate[i].first 
                  << ", " << _particle_centers_coordinate[i].second << "), "
                  << "Radius: " << _particle_radius[i] << std::endl;
    }
    std::cout << std::endl;
}

std::pair<std::vector<std::pair<int, int>>, std::vector<int>> 
MultiParticles_2D::particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, const std::vector<int>& domain)
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

// This is the primary function custom ICs must implement.
Real
MultiParticles_2D::value(const Point & p)
{
    Real total_value = 0.0;

    // Iterate over all particle centers
    for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
    {
        // Compute the Euclidean distance between (x, y) and the current particle center
        double distance = std::sqrt(
            std::pow(p(0) - _particle_centers_coordinate[i].first, 2) + 
            std::pow(p(1) - _particle_centers_coordinate[i].second, 2)) - _particle_radius[i];
        
        // Compute the argument of the tanh function
        double argument = distance * 2 * std::atanh(1 - 2 * _omega) / _delta;
        
        // Compute the value for this particle and add it to the total value
        total_value += 0.5 * (1.0 - std::tanh(argument));
    }

    return total_value;
}