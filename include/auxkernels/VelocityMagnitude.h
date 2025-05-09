/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-13 14:19:45
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:40:34
 * @FilePath: /viscosity_sintering/include/auxkernels/VelocityMagnitude.h
 * @Description: Header file for VelocityMagnitude class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

 #pragma once

 #include "AuxKernel.h"
 #include "MooseEnum.h"

 class VelocityMagnitude : public AuxKernel
 {
   public:
    static InputParameters validParams();

    VelocityMagnitude(const InputParameters & parameters);

   protected:
    MooseEnum _dim;
    
    virtual Real computeValue() override;
    
    const VariableValue & _u;
    const VariableValue & _v;
    const VariableValue & _w;
 };