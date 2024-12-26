/*** 
 * @Author: Bo Qian
 * @Date: 2024-12-26 14:03:10
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-26 14:30:24
 * @LastEditors: Bo Qian
 * @Description: 
 * @FilePath: /viscosity_sintering/include/auxkernels/VSTotalFreeEnergy.h
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
    
    const VariableValue & _c;
    const VariableGradient & _grad_c;
    const MaterialProperty<Real> & _F_loc;
    const MaterialProperty<Real> & _kappa_C;
};
