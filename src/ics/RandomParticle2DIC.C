/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-03-20 17:42:15
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:33:22
 * @FilePath: /viscosity_sintering/src/ics/RandomParticle2DIC.C
 * @Description: Initial Condition for Random Particle Distribution in 2D
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "RandomParticle2DIC.h"
#include <random>
#include <cmath>
#include <algorithm>
#include "libmesh/parallel_algebra.h"
#include <iomanip>

registerMooseObject("viscosity_sinteringApp", RandomParticle2DIC);

InputParameters
RandomParticle2DIC::validParams()
{
  InputParameters params = InitialCondition::validParams();
  params.addRequiredParam<std::vector<Real>>("domain", "Domain size (width, height)");
  params.addRequiredParam<unsigned int>("num_particles", "Number of particles to generate");
  params.addRequiredParam<Real>("radius_min", "Minimum particle radius");
  params.addRequiredParam<Real>("radius_max", "Maximum particle radius");
  params.addParam<unsigned int>("max_attempts", 1000, "Maximum placement attempts per particle");
  params.addParam<Real>("edge_factor", 0.05, "Edge clearance factor (0-0.5)");
  params.addRequiredParam<Real>("delta", "Interface thickness");
  params.addRequiredParam<Real>("omega", "Order parameter value for matrix");
  return params;
}

RandomParticle2DIC::RandomParticle2DIC(const InputParameters & parameters)
  : InitialCondition(parameters),
    _domain(getParam<std::vector<Real>>("domain")),
    _num_particles(getParam<unsigned int>("num_particles")),
    _radius_min(getParam<Real>("radius_min")),
    _radius_max(getParam<Real>("radius_max")),
    _max_attempts(getParam<unsigned int>("max_attempts")),
    _edge_factor(getParam<Real>("edge_factor")),
    _delta(getParam<Real>("delta")),
    _omega(getParam<Real>("omega"))
{
  // Domain validation
  if (_domain.size() != 2)
    mooseError("Domain must be 2D (width, height)");

  // Synchronize random seed
  if (processor_id() == 0)
    _global_seed = std::random_device{}();
  _communicator.broadcast(_global_seed);

  // Generate and broadcast particle data
  generateAndBroadcastParticles();

  // Final validation
  if (_centers.size() != _num_particles)
    mooseError("Particle generation failed: ", _centers.size(), " of ", _num_particles);
  
  // Particle count validation
  printParticleInfo();
}

void
RandomParticle2DIC::generateAndBroadcastParticles()
{
  // Generate data on the master process
  if (processor_id() == 0)
  {
    auto result = generateRandomParticles();
    _centers = result.first;
    _radii = result.second;
  }

  // Synchronize particle count
  unsigned int num_particles = _centers.size();
  _communicator.broadcast(num_particles);

  // Prepare data for transmission
  std::vector<Real> centers_data;
  if (processor_id() == 0)
  {
    centers_data.reserve(3 * num_particles);
    for (const auto & p : _centers)
    {
      centers_data.push_back(p(0));
      centers_data.push_back(p(1));
      centers_data.push_back(p(2));
    }
  }

  // Broadcast data
  _communicator.broadcast(centers_data);
  _communicator.broadcast(_radii);

  // Receive data on other processes
  if (processor_id() != 0)
  {
    // Check data integrity
    if (centers_data.size() != 3 * num_particles)
      mooseError("Invalid particle coordinate data received");
    if (_radii.size() != num_particles)
      mooseError("Invalid particle radius data received");

    _centers.resize(num_particles);
    for (unsigned int i = 0; i < num_particles; ++i)
      _centers[i] = Point(centers_data[3*i], centers_data[3*i+1], centers_data[3*i+2]);
  }
}

std::pair<std::vector<Point>, std::vector<Real>> 
RandomParticle2DIC::generateRandomParticles()
{
  std::vector<Point> centers;
  std::vector<Real> radii;
  std::mt19937 gen(_global_seed);  // Use synchronized seed

  // Generate the first particle
  std::uniform_real_distribution<Real> x_dist(_radius_min + _domain[0]*_edge_factor, 
                                              _domain[0]*(1 - _edge_factor) - _radius_min);
  std::uniform_real_distribution<Real> y_dist(_radius_min + _domain[1]*_edge_factor, 
                                              _domain[1]*(1 - _edge_factor) - _radius_min);
  std::uniform_real_distribution<Real> r_dist(_radius_min, _radius_max);

  Real x = x_dist(gen);
  Real y = y_dist(gen);
  Real r = r_dist(gen);
  centers.emplace_back(x, y, 0.0);
  radii.push_back(r);

  // Generate subsequent particles
  for (unsigned int i = 1; i < _num_particles; ++i)
  {
    bool placed = false;
    unsigned int attempts = 0;

    while (!placed && attempts++ < _max_attempts)
    {
      // Select parent particle
      std::uniform_int_distribution<> parent_dist(0, centers.size()-1);
      unsigned int parent_idx = parent_dist(gen);
      Point parent = centers[parent_idx];
      Real parent_r = radii[parent_idx];

      // Generate candidate radius
      Real current_min = std::max(_radius_min, parent_r * 0.5);
      Real current_max = std::min(_radius_max, parent_r * 2.0);
      std::uniform_real_distribution<Real> current_r_dist(current_min, current_max);
      Real candidate_r = current_r_dist(gen);

      // Generate tangent position
      std::uniform_real_distribution<Real> angle_dist(0.0, 2.0 * M_PI);
      Real angle = angle_dist(gen);
      Real dx = (parent_r + candidate_r) * std::cos(angle);
      Real dy = (parent_r + candidate_r) * std::sin(angle);
      Point candidate(parent(0) + dx, parent(1) + dy, 0.0);

      // Boundary check
      if (candidate(0) < candidate_r + _domain[0]*_edge_factor || 
          candidate(0) > _domain[0]*(1 - _edge_factor) - candidate_r ||
          candidate(1) < candidate_r + _domain[1]*_edge_factor || 
          candidate(1) > _domain[1]*(1 - _edge_factor) - candidate_r)
        continue;

      // Collision check
      bool collision = false;
      for (unsigned int j = 0; j < centers.size(); ++j)
      {
        Real dx = candidate(0) - centers[j](0);
        Real dy = candidate(1) - centers[j](1);
        Real distance = std::sqrt(dx*dx + dy*dy);
        Real min_distance = radii[j] + candidate_r;

        if (distance < min_distance * 0.999)
        {
          collision = true;
          break;
        }
      }

      if (!collision)
      {
        centers.push_back(candidate);
        radii.push_back(candidate_r);
        placed = true;
      }
    }

    if (!placed)
      mooseError("Failed to place particle ", i+1, " after ", _max_attempts, " attempts");
  }

  return {centers, radii};
}

void
RandomParticle2DIC::printParticleInfo() const
{
  const int table_width = 46;
  const int index_width = 7;
  const int coord_width = 24;
  const int radius_width = 11;

  auto center_text = [](const std::string & text, int width) -> std::string {
    int padding = width - text.length();
    int pad_left = padding / 2;
    int pad_right = padding - pad_left;
    return std::string(pad_left, ' ') + text + std::string(pad_right, ' ');
  };

  std::cout << "\n" << std::string(table_width, '=') << "\n";
  std::ostringstream title;
  title << "Random Particle Configuration (2D)";
  std::cout << center_text(title.str(), table_width) << "\n";  
  std::cout << std::string(table_width, '-') << "\n";

  // Header
  std::cout << "|"
            << center_text("Index", index_width) << "|"
            << center_text("Coordinate", coord_width) << "|"
            << center_text("Radius", radius_width) << "|\n";
  std::cout << std::string(table_width, '-') << "\n";

  // Content
  for (size_t i = 0; i < _centers.size(); ++i)
  {
    auto & center = _centers[i];
    std::ostringstream coord;
    coord << "(" << std::setw(8) << std::fixed << std::setprecision(4) << center(0) << ", "
                 << std::setw(8) << std::fixed << std::setprecision(4) << center(1);
    coord << ")";

    std::ostringstream radius;
    radius << std::fixed << std::setprecision(4) << _radii[i];

    std::cout << "|"
              << center_text(std::to_string(i + 1), index_width) << "|"
              << center_text(coord.str(), coord_width) << "|"
              << center_text(radius.str(), radius_width) << "|\n";
  }
  std::cout << std::string(table_width, '=') << "\n\n";
}

Real
RandomParticle2DIC::value(const Point & p)
{
  Real sum = 0.0;
  const Real factor = 2.0 * std::atanh(1.0 - 2.0 * _omega) / _delta;

  for (unsigned int i = 0; i < _centers.size(); ++i)
  {
    Real dx = p(0) - _centers[i](0);
    Real dy = p(1) - _centers[i](1);
    Real distance = std::sqrt(dx*dx + dy*dy) - _radii[i];
    sum += 0.5 * (1.0 - std::tanh(distance * factor));
  }

  return sum;
}