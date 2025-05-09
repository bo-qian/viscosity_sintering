/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 18:28:51
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-03 18:29:03
 * @FilePath: /viscosity_sintering/include/auxkernels/RealPressure.h
 * @Description: Header file for RealPressure class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "AuxKernel.h"

class RealPressure : public AuxKernel
{
public:
  static InputParameters validParams();

  RealPressure(const InputParameters & parameters);
  
protected:
  virtual Real computeValue() override;

  const MaterialProperty<Real> & _alpha;
  const MaterialProperty<Real> & _kappa_c;
  const VariableValue & _p;
  const VariableValue & _c;
  const VariableGradient & _grad_c;
  const VariableValue & _mu;
};