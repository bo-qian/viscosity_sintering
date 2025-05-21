/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 11:06:47
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:19:39
 * @FilePath: /viscosity_sintering/include/kernels/StokesMass.h
 * @Description: Header file for the incompressibility kernel
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#pragma once

#include "ADKernel.h"

class StokesMass : public ADKernel
{
public:
  StokesMass(const InputParameters & parameters);
  static InputParameters validParams();

protected:
  ADReal computeQpResidual() override;
  // virtual Real computeQpJacobian() override;
  // virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  // const unsigned int _velvar;
  const VectorVariableValue & _vel;
  const VectorVariableGradient & _grad_vel;

};
#endif