/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:46:25
 * @LastEditors: Bo Qian
 * @Description: Head File of MultiParticles_3D
 * @FilePath: /viscosity_sintering/include/ics/MultiParticles_3D.h
 */

#pragma once

// MOOSE Includes
#include "InitialCondition.h"
#include "MooseTypes.h"

/**
 * MultiParticles_3D just returns a constant value in 3D space.
 */
class MultiParticles_3D : public InitialCondition
{
public:
  /**
   * Constructor: Same as the rest of the MOOSE Objects
   */
  MultiParticles_3D(const InputParameters & parameters);

  static InputParameters validParams();

  /**
   * The value of the variable at a point in 3D space.
   */
  virtual Real value(const Point & p) override;

private:
  const Real _delta;
  const Real _radius;
  const int _number_x;
  const int _number_y;
  const int _number_z;  // 添加第三维度的粒子数量
  const Real _omega;
  std::vector<int> _domain; 
  std::vector<std::tuple<int, int, int>> _particle_centers_coordinate; // 使用 tuple 存储三维坐标
  std::vector<int> _particle_radius;

  /**
   * 计算粒子中心坐标和半径
   */
  std::pair<std::vector<std::tuple<int, int, int>>, std::vector<int>> 
  particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, int number_z, const std::vector<int>& domain);
};
