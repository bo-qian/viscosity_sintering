/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:39:59
 * @FilePath: /viscosity_sintering/include/kernels/CHMob.h
 * @Description: Header file for CHMob class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "Kernel.h"

class CHMob : public Kernel
{
public:
  static InputParameters validParams();

  CHMob(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<Real> & _mobility;

  // Coupled variables
  const unsigned int _muvar;
  const VariableValue & _mu;
  const VariableGradient & _grad_mu;
};