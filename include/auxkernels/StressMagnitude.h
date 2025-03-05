/*** 
 * @Author: Bo Qian
 * @Date: 2025-03-05 18:56:17
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-03-05 19:02:33
 * @LastEditors: Bo Qian
 * @Description: 
 * @FilePath: /viscosity_sintering/include/auxkernels/StressMagnitude.h
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