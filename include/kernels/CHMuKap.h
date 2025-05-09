/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:39:33
 * @FilePath: /viscosity_sintering/include/kernels/CHMuKap.h
 * @Description: Header file for CHMuKap class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "Kernel.h"

class CHMuKap : public Kernel
{
public:
  static InputParameters validParams();

  CHMuKap(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<Real> & _kappa_c;
  const MaterialProperty<Real> & _theta;

   
  // Coupled variables
  const unsigned int _cvar;
  const VariableValue & _c;
  const VariableValue & _cvar_old;
  const VariableGradient & _grad_c;
  const VariableGradient & _grad_c_old;
};