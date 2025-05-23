/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-03-18 19:38:24
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:29:17
 * @FilePath: /viscosity_sintering/src/ics/MultiParticlesIC.C
 * @Description: Initial Condition for MultiParticlesIC
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "MultiParticlesIC.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <utility>

registerMooseObject("viscosity_sinteringApp", MultiParticlesIC);

InputParameters
MultiParticlesIC::validParams()
{
  InputParameters params = InitialCondition::validParams();
  MooseEnum dims("2=2 3");
  params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
  params.addRequiredParam<Real>("delta", "The thickness of the interface");
  params.addRequiredParam<Real>("radius", "The radius of the particles");
  params.addRequiredParam<int>("number_x", "The number of particles in the x direction");
  params.addRequiredParam<int>("number_y", "The number of particles in the y direction");
  params.addParam<int>("number_z", 0, "The number of particles in the z direction");
  params.addRequiredParam<Real>("omega", "The omega coordinate of the center of the particles");
  params.addRequiredParam<std::vector<int>>("domain", "The domain size as a vector {width, height}");
  return params;
}

MultiParticlesIC::MultiParticlesIC(const InputParameters & parameters)
  : InitialCondition(parameters),
    _dim(getParam<MooseEnum>("dim")),
    _delta(getParam<Real>("delta")), 
    _radius(getParam<Real>("radius")), 
    _number_x(getParam<int>("number_x")),
    _number_y(getParam<int>("number_y")),
    _number_z(getParam<int>("number_z")),
    _omega(getParam<Real>("omega")),
    _domain(getParam<std::vector<int>>("domain"))
{
  // Parameter validation
  if (_dim == 3 && _number_z <= 0)
    mooseError("For 3D simulations, 'number_z' must be provided and greater than 0.");
  if (_dim == 2 && _domain.size() < 2)
    mooseError("For 2D simulations, 'domain' must have at least two elements.");
  if (_dim == 3 && _domain.size() < 3)
    mooseError("For 3D simulations, 'domain' must have three elements.");

  // Generate particle coordinates and radii
  auto result = generateParticleCenters(_radius, _number_x, _number_y, _number_z, _domain, _dim);
  _particle_centers = result.first;
  _particle_radii = result.second;

  printParticleInfo();
}

std::pair<std::vector<std::tuple<int, int, int>>, std::vector<int>>
MultiParticlesIC::generateParticleCenters(
    Real radius, int nx, int ny, int nz, const std::vector<int>& domain, const MooseEnum &dim)
{
  std::vector<std::tuple<int, int, int>> centers;
  std::vector<int> radii;
  const int total_particles = (dim == "3") ? nx * ny * nz : nx * ny;

  radii.resize(total_particles, radius);

  // Generate coordinates
  auto generate = [&](auto z_loop) {
    for (int i = 0; i < nx; ++i) {
      float x = domain[0]/2.0f + (i + (1.0f - nx)/2.0f) * radius * 2.0f;
      for (int j = 0; j < ny; ++j) {
        float y = domain[1]/2.0f + (j + (1.0f - ny)/2.0f) * radius * 2.0f;
        z_loop(x, y);
      }
    }
  };

  if (dim == "3") {
    for (int k = 0; k < nz; ++k) {
      float z = domain[2]/2.0f + (k + (1.0f - nz)/2.0f) * radius * 2.0f;
      generate([&](float x, float y) {
        centers.emplace_back(x, y, z);
      });
    }
  } else {
    generate([&](float x, float y) {
      centers.emplace_back(x, y, 0); // z=0 in 2D
    });
  }

  return {centers, radii};
}

void
MultiParticlesIC::printParticleInfo() const
{
  const int table_width = 46;
  const int index_width = 7;
  const int coord_width = 25;
  const int radius_width = 10;

  auto center_text = [](const std::string & text, int width) -> std::string {
    int padding = width - text.length();
    int pad_left = padding / 2;
    int pad_right = padding - pad_left;
    return std::string(pad_left, ' ') + text + std::string(pad_right, ' ');
  };

  std::cout << "\n" << std::string(table_width, '=') << "\n";
  std::ostringstream title;
  title << "Particle Configuration (" << _dim << "D)";
  std::cout << center_text(title.str(), table_width) << "\n";  
  std::cout << std::string(table_width, '-') << "\n";

  // Header
  std::cout << "|"
            << center_text("Index", index_width) << "|"
            << center_text("Coordinate", coord_width) << "|"
            << center_text("Radius", radius_width) << "|\n";
  std::cout << std::string(table_width, '-') << "\n";

  // Content
  for (size_t i = 0; i < _particle_centers.size(); ++i)
  {
    auto & center = _particle_centers[i];
    std::ostringstream coord;
    coord << "(" << std::get<0>(center) << ", " << std::get<1>(center);
    if (_dim == "3")
      coord << ", " << std::get<2>(center);
    coord << ")";

    std::ostringstream radius;
    radius << _particle_radii[i];

    std::cout << "|"
              << center_text(std::to_string(i + 1), index_width) << "|"
              << center_text(coord.str(), coord_width) << "|"
              << center_text(radius.str(), radius_width) << "|\n";
  }
  std::cout << std::string(table_width, '=') << "\n\n";
}

Real
MultiParticlesIC::value(const Point & p)
{
  Real total = 0.0;
  const Real factor = 2.0 * std::atanh(1.0 - 2.0 * _omega) / _delta;

  for (size_t i = 0; i < _particle_centers.size(); ++i) {
    const auto &center = _particle_centers[i];
    const Real dx = p(0) - std::get<0>(center);
    const Real dy = p(1) - std::get<1>(center);
    const Real dz = (_dim == "3") ? (p(2) - std::get<2>(center)) : 0.0;

    const Real distance = std::sqrt(dx*dx + dy*dy + dz*dz) - _particle_radii[i];
    total += 0.5 * (1.0 - std::tanh(distance * factor));
  }

  return total;
}
