/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:45:31
 * @LastEditors: Bo Qian
 * @Description: Head File of MultiParticles_2D
 * @FilePath: /viscosity_sintering/include/ics/MultiParticles_2D.h
 */

#pragma once

// MOOSE Includes
#include "InitialCondition.h"
#include "MooseTypes.h"            // 添加该头文件

/**
 * MultiParticles just returns a constant value.
 */
class MultiParticles_2D : public InitialCondition
{
public:
  /**
   * Constructor: Same as the rest of the MOOSE Objects
   */
  MultiParticles_2D(const InputParameters & parameters);

  static InputParameters validParams();  // 确保函数声明拼写正确

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p) override;  // 确保 Point 类型正确包含

private:
    const Real _delta;
    const Real _radius;
    const int _number_x;  // 修改为 int 类型
    const int _number_y;  // 修改为 int 类型
    const Real _omega;
    std::vector<int> _domain; 
    std::vector<std::pair<int, int>> _particle_centers_coordinate;
    std::vector<int> _particle_radius;

    // 函数声明的参数类型需要与函数定义保持一致
    std::pair<std::vector<std::pair<int, int>>, std::vector<int>> 
    particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, const std::vector<int>& domain);
};
