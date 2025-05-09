/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-03-05 18:56:17
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-07 16:04:27
 * @FilePath: /viscosity_sintering/include/auxkernels/StressMagnitude.h
 * @Description: Stress Magnitude AuxKernel
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "AuxKernel.h"

class StressMagnitude : public AuxKernel
{
  public:
    static InputParameters validParams();

    StressMagnitude(const InputParameters & parameters);

  protected:
  
    virtual Real computeValue() override;
    
    const MaterialProperty<Real> & _stress_xx;
    const MaterialProperty<Real> & _stress_xy;
    const MaterialProperty<Real> & _stress_xz;
    const MaterialProperty<Real> & _stress_yx;
    const MaterialProperty<Real> & _stress_yy;
    const MaterialProperty<Real> & _stress_yz;
    const MaterialProperty<Real> & _stress_zx;
    const MaterialProperty<Real> & _stress_zy;
    const MaterialProperty<Real> & _stress_zz;
};