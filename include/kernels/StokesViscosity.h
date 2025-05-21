/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-20 09:23:26
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-21 14:19:50
 * @FilePath: /viscosity_sintering/include/kernels/StokesViscosity.h
 * @Description: Header file for the vector kernel of the viscosity term in the Stokes equation
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */
#ifdef COMPILE_THIS_FILE

#pragma once

#include "ADVectorKernel.h"

class StoeksViscosity : public ADVectorKernel
{
public:
  StoeksViscosity(const InputParameters & parameters);
  static InputParameters validParams();

protected:
  ADReal computeQpResidual() override;
  // virtual Real computeQpJacobian() override;
  // virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  const MaterialProperty<Real> & _mu_eff;

};
#endif