/*
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-03-18 19:40:20
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @Description: 2D Initial Condition for MultiParticles
 * @FilePath: /viscosity_sintering/src/ics/MultiParticlesIC.C
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
  const int width = 35;
  std::cout << "\n" << std::string(width, '=') << "\n";
  std::cout << "Particle Configuration (" << _dim << "D)\n";
  std::cout << std::string(width, '-') << "\n";

  for (size_t i = 0; i < _particle_centers.size(); ++i) {
    auto &center = _particle_centers[i];
    std::cout << "Particle " << i+1 << ": ("
              << std::get<0>(center) << ", " 
              << std::get<1>(center);
    if (_dim == "3") 
      std::cout << ", " << std::get<2>(center);
    std::cout << "), Radius: " << _particle_radii[i] << "\n";
  }
  std::cout << std::string(width, '=') << "\n\n";
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
