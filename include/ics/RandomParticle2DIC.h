/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-03-20 17:42:46
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:33:02
 * @FilePath: /viscosity_sintering/include/ics/RandomParticle2DIC.h
 * @Description: Header file for RandomParticle2DIC class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */


#pragma once
#include "InitialCondition.h"
#include "libmesh/parallel.h"  // Add parallel support header

class RandomParticle2DIC : public InitialCondition
{
public:
  static InputParameters validParams();
  RandomParticle2DIC(const InputParameters & parameters);
  virtual Real value(const Point & p) override;

protected:
  void generateAndBroadcastParticles();
  std::pair<std::vector<Point>, std::vector<Real>> generateRandomParticles();

  const std::vector<Real> _domain;
  const unsigned int _num_particles;
  const Real _radius_min;
  const Real _radius_max;
  const unsigned int _max_attempts;
  const Real _edge_factor;
  const Real _delta;
  const Real _omega;

  std::vector<Point> _centers;
  std::vector<Real> _radii;
  unsigned int _global_seed;  // Add global seed
  
  // Information printing method
  void printParticleInfo() const;
};