#ifndef EXCLUDE_THIS_FILE

#pragma once

#include "Kernel.h"
// #include "MaterialParameters.h"

class StokVisco : public Kernel
{
public:
  StokVisco(const InputParameters & parameters);
  static InputParameters validParams();

  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();

  // // 如果需要计算其他变量的雅可比矩阵，可以取消注释下面的函数
  // virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

protected:
  // 定义耦合变量和材料属性
  // const VariableValue & _c;   // 耦合变量 c_var
  // unsigned int _c_var;        // 耦合变量索引
  // std::string _c_name;

};

#endif
