/*
 * @Author: Bo Qian
 * @Date: 2025-02-13 14:47:31
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-13 14:47:31
 * @LastEditors: Bo Qian
 * @Description: StressTensor.h
 * @FilePath: /viscosity_sintering/include/auxkernels/StressTensor.h
 */
#ifndef EXCLUDE_THIS_FILE

#pragma once

#include "AuxKernel.h"
#include "MooseEnum.h"

class StressTensor : public AuxKernel
{
  public:
    static InputParameters validParams();

    StressTensor(const InputParameters & parameters);

  protected:
    MooseEnum _dim;

    virtual RealVectorValue computeQpVelocity();
    virtual Real computeValue() override;
    
    const VariableValue & _u;
    const VariableValue & _v;
    const VariableValue & _w;
    const VariableValue & _p;
    const VariableGradient & _grad_u;
    const VariableGradient & _grad_v;
    const VariableGradient & _grad_w;
    const MaterialProperty<Real> & _mu_eff;
    const MaterialProperty<Real> & _F_loc;

};

#endif