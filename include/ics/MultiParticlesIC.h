/*
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-03-18 19:40:17
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @Description: Header File of MultiParticlesIC
 * @FilePath: /viscosity_sintering/include/ics/MultiParticlesIC.h
 */

#pragma once

#include "InitialCondition.h"
#include "MooseTypes.h"
#include "MooseEnum.h"

class MultiParticlesIC : public InitialCondition
{
public:
  MultiParticlesIC(const InputParameters & parameters);

  static InputParameters validParams();
  virtual Real value(const Point & p) override;

protected:
  MooseEnum _dim;

private:
  const Real _delta;
  const Real _radius;
  const int _number_x;
  const int _number_y;
  const int _number_z;
  const Real _omega;
  std::vector<int> _domain; 
  
  // Store coordinates as tuples
  std::vector<std::tuple<int, int, int>> _particle_centers;  // Unified storage of coordinates
  std::vector<int> _particle_radii;                          // Unified naming in plural form

  // Core initialization method
  std::pair<std::vector<std::tuple<int, int, int>>, std::vector<int>> 
  generateParticleCenters(Real radius, int nx, int ny, int nz, 
                        const std::vector<int>& domain, const MooseEnum &dim);

  // Information printing method
  void printParticleInfo() const;
};
