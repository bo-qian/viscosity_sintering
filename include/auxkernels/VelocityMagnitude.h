/*** 
 * @Author: Bo Qian
 * @Date: 2025-02-13 14:19:45
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-13 14:31:03
 * @LastEditors: Bo Qian
 * @Description: VelocityMagnitude.h
 * @FilePath: /viscosity_sintering/include/auxkernels/VelocityMagnitude.h
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