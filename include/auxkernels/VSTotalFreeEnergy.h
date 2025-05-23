/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-13 19:12:47
 * @FilePath: /viscosity_sintering/include/auxkernels/VSTotalFreeEnergy.h
 * @Description: Header file for VSTotalFreeEnergy class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "AuxKernel.h"

class VSTotalFreeEnergy : public AuxKernel
{
  public:
    static InputParameters validParams();

    VSTotalFreeEnergy(const InputParameters & parameters);

  protected:
    virtual Real computeValue() override;
    
    const MaterialProperty<Real> & _F_loc;
    const MaterialProperty<Real> & _F_grad;
};
