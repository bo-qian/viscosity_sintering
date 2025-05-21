/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 10:57:21
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:19:44
 * @FilePath: /viscosity_sintering/include/kernels/StokesPressure.h
 * @Description: Header file for the vector kernel of the pressure term in the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#pragma once

#include "ADVectorKernel.h"

class StokesPressure : public ADVectorKernel
{
public:
  StokesPressure(const InputParameters & parameters);
  static InputParameters validParams();

protected:
  ADReal computeQpResidual() override;
  // virtual Real computeQpJacobian() override;
  // virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  // const unsigned int _pvar;
  const VariableValue & _p;
  const VariableGradient & _grad_p;

};
#endif