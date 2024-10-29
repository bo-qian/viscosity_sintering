/*
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:46:43
 * @LastEditors: Bo Qian
 * @Description: 3D Initial Condition for MultiParticles
 * @FilePath: /viscosity_sintering/src/ics/MultiParticles_3D.C
 */

#include "MultiParticles_3D.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>
#include <tuple>

registerMooseObject("viscosity_sinteringApp", MultiParticles_3D);

InputParameters
MultiParticles_3D::validParams()
{
    InputParameters params = InitialCondition::validParams();
    params.addRequiredParam<Real>("delta", "The thickness of the interface");
    params.addRequiredParam<Real>("radius", "The radius of the particles");
    params.addRequiredParam<int>("number_x", "The number of particles in the x direction");
    params.addRequiredParam<int>("number_y", "The number of particles in the y direction");
    params.addRequiredParam<int>("number_z", "The number of particles in the z direction");  // 添加z方向粒子数量参数
    params.addRequiredParam<Real>("omega", "The omega coordinate of the center of the particles");
    params.addRequiredParam<std::vector<int>>("domain", "The domain size as a vector {width, height, depth}");  // 修改域尺寸为3D
    return params;
}

MultiParticles_3D::MultiParticles_3D(const InputParameters & parameters)
  : InitialCondition(parameters), 
    _delta(getParam<Real>("delta")), 
    _radius(getParam<Real>("radius")), 
    _number_x(getParam<int>("number_x")),
    _number_y(getParam<int>("number_y")),
    _number_z(getParam<int>("number_z")),  // 初始化第三维度数量
    _omega(getParam<Real>("omega")),
    _domain(getParam<std::vector<int>>("domain"))
{
    // Compute total number of particles in 3D
    int particle_number_total = _number_x * _number_y * _number_z;

    // Call the particle centers function
    auto result = particleCentersWithoutTemplate(_radius, particle_number_total, _number_x, _number_y, _number_z, _domain);

    // Store the results in the class members
    _particle_centers_coordinate = result.first;
    _particle_radius = result.second;

    // Print the particle centers and radii
    std::cout << "3D Particle centers and radii:" << std::endl;
    for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
    {
        std::cout << "Center: (" << std::get<0>(_particle_centers_coordinate[i]) 
                  << ", " << std::get<1>(_particle_centers_coordinate[i])
                  << ", " << std::get<2>(_particle_centers_coordinate[i]) << "), "
                  << "Radius: " << _particle_radius[i] << std::endl;
    }
    std::cout << std::endl;
}

std::pair<std::vector<std::tuple<int, int, int>>, std::vector<int>> 
MultiParticles_3D::particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, int number_z, const std::vector<int>& domain)
{
    std::vector<int> particle_radius(particle_number_total, radius_particle);
    std::vector<std::tuple<int, int, int>> particle_centers_coordinate;

    for (int k = 0; k < number_z; ++k)
    {
        for (int j = 0; j < number_y; ++j)
        {
            for (int i = 0; i < number_x; ++i)
            {
                float x_coordinate = domain[0] / 2.0f + (i + (1.0f - number_x) / 2.0f) * radius_particle * 2.0f;
                float y_coordinate = domain[1] / 2.0f + (j + (1.0f - number_y) / 2.0f) * radius_particle * 2.0f;
                float z_coordinate = domain[2] / 2.0f + (k + (1.0f - number_z) / 2.0f) * radius_particle * 2.0f;
                particle_centers_coordinate.push_back({static_cast<int>(x_coordinate), static_cast<int>(y_coordinate), static_cast<int>(z_coordinate)});
            }
        }
    }

    return {particle_centers_coordinate, particle_radius};
}

// This is the primary function custom ICs must implement for 3D.
Real
MultiParticles_3D::value(const Point & p)
{
    Real total_value = 0.0;

    // Iterate over all particle centers in 3D
    for (size_t i = 0; i < _particle_centers_coordinate.size(); ++i)
    {
        // Compute the Euclidean distance between (x, y, z) and the current particle center
        double distance = std::sqrt(
            std::pow(p(0) - std::get<0>(_particle_centers_coordinate[i]), 2) + 
            std::pow(p(1) - std::get<1>(_particle_centers_coordinate[i]), 2) +
            std::pow(p(2) - std::get<2>(_particle_centers_coordinate[i]), 2)) - _particle_radius[i];
        
        // Compute the argument of the tanh function
        double argument = distance * 2 * std::atanh(1 - 2 * _omega) / _delta;
        
        // Compute the value for this particle and add it to the total value
        total_value += 0.5 * (1.0 - std::tanh(argument));
    }

    return total_value;
}
