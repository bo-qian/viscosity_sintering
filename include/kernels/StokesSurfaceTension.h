/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 10:17:17
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:20:01
 * @FilePath: /viscosity_sintering/include/kernels/StokesSurfaceTension.h
 * @Description: Header file for the vector kernel of the surface tension term in the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#pragma once

#include "ADVectorKernel.h"

class StokesSurfaceTension : public ADVectorKernel
{
public:
  StokesSurfaceTension(const InputParameters & parameters);
  static InputParameters validParams();

protected:
  ADReal computeQpResidual() override;
  // virtual Real computeQpJacobian() override;
  // virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  const MaterialProperty<Real> & _kappa_c;

  // const unsigned int _cvar;
  const VariableValue & _c;
  const VariableGradient & _grad_c;
  // const unsigned int _pvar;
  const VariableValue & _p;

};
#endif