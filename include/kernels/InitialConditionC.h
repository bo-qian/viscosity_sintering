/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-19 10:08:14
 * @LastEditors: Bo Qian
 * @Description: Head File of InitialConditionC
 * @FilePath: /viscosity_sintering/include/kernels/InitialConditionC.h
 */

#pragma once

// MOOSE Includes
#include "Kernel.h"
#include "MooseTypes.h"            // 添加该头文件

/**
 * MultiParticles just returns a constant value.
 */
class InitialConditionC : public Kernel
{
public:
  /**
   * Constructor: Same as the rest of the MOOSE Objects
   */
  InitialConditionC(const InputParameters & parameters);

  static InputParameters validParams();  // 确保函数声明拼写正确

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  // virtual void computeQpProperties(const Point & p);
  virtual Real value(const Point & p);  // 确保 Point 类型正确包含

protected:

	virtual Real computeQpResidual() override;
	// virtual Real computeQpJacobian() override;
  
	// virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

	// virtual Real computeQpResidual() override
	// {
	// 	return 0.0;
	// }

  virtual Real computeQpJacobian() override;
	// {
	// 	return 0.0;
	// }

  virtual Real computeQpOffDiagJacobian(unsigned jvar) override
	{
		return 0.0;
	}

private:
    const Real _delta;
    const Real _radius;
    const int _number_x;  // 修改为 int 类型
    const int _number_y;  // 修改为 int 类型
    const Real _omega;
    std::vector<int> _domain; 
    std::vector<std::pair<int, int>> _particle_centers_coordinate;
    std::vector<int> _particle_radius;

    // const Real _C_Initial;

		// MaterialProperty<Real> & _C_Initial;

    // 函数声明的参数类型需要与函数定义保持一致
    std::pair<std::vector<std::pair<int, int>>, std::vector<int>> 
    particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, const std::vector<int>& domain);
};
