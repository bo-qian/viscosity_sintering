/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 18:28:51
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-14 09:12:18
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

  const VariableValue & _p;
  
  const MaterialProperty<Real> & _f_loc;
  const MaterialProperty<Real> & _f_grad;
};