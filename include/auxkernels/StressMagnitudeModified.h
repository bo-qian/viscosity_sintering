/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-03-05 18:56:17
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-12 16:44:13
 * @FilePath: /viscosity_sintering/include/auxkernels/StressMagnitudeModified.h
 * @Description: Stress Magnitude AuxKernel
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "AuxKernel.h"

class StressMagnitudeModified : public AuxKernel
{
  public:
    static InputParameters validParams();

    StressMagnitudeModified(const InputParameters & parameters);

  protected:
  
    virtual Real computeValue() override;
    
    const MaterialProperty<Real> & _stress_xx_modified;
    const MaterialProperty<Real> & _stress_xy;
    const MaterialProperty<Real> & _stress_xz;
    const MaterialProperty<Real> & _stress_yx;
    const MaterialProperty<Real> & _stress_yy_modified;
    const MaterialProperty<Real> & _stress_yz;
    const MaterialProperty<Real> & _stress_zx;
    const MaterialProperty<Real> & _stress_zy;
    const MaterialProperty<Real> & _stress_zz_modified;
};