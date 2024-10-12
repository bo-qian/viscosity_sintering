#pragma once

#include "Kernel.h"

class CHCoupV : public Kernel
{
public:
  static InputParameters validParams();
  CHCoupV(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;  // 计算残差
  virtual Real computeQpJacobian() override;  // 计算雅可比矩阵

private:
  const VectorValue & _v;  // 耦合速度变量
};

#endif // CUSTOMKERNEL_H
