/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-13 16:38:56
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 14:37:57
 * @LastEditors: Bo Qian
 * @Description: Header file for the viscosity term in the Stokes equation
 * @FilePath: /viscosity_sintering/include/kernels/StokVisco.h
 */

#ifndef EXCLUDE_THIS_FILE

#pragma once

#include "Kernel.h"
// #include "MaterialParameters.h"

class StokVisco : public Kernel
{
public:
  StokVisco(const InputParameters & parameters);
  static InputParameters validParams();

protected:

  virtual RealVectorValue computeQpVelocity();
  virtual RealTensorValue computeQpGradVelocity();

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  // // 如果需要计算其他变量的雅可比矩阵，可以取消注释下面的函数
  // virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<Real> & _mu_eff;

  const VariableValue & _u_vel;
  const VariableValue & _v_vel;
  const VariableValue & _w_vel;

  const VariableGradient & _grad_u_vel;
  const VariableGradient & _grad_v_vel;
  const VariableGradient & _grad_w_vel;


  // 定义耦合变量和材料属性
  // const VariableValue & _c;   // 耦合变量 c_var
  // unsigned int _c_var;        // 耦合变量索引
  // std::string _c_name;

};

#endif
