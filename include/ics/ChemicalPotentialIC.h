/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 16:19:22
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-06 18:33:41
 * @FilePath: /viscosity_sintering/include/ics/ChemicalPotentialIC.h
 * @Description: Header file for ChemicalPotentialIC class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "InitialCondition.h"

class ChemicalPotentialIC : public InitialCondition
{
public:
  ChemicalPotentialIC(const InputParameters & parameters);
  static InputParameters validParams();

private:
  virtual Real value(const Point & p) override;

  const MaterialProperty<Real> & _Kappa_C;
  const MaterialProperty<Real> & _alpha;
  const VariableValue & _c;
  const VariableGradient & _grad_c;
};
