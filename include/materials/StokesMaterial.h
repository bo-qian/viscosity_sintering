/*** 
 * @Author: Bo Qian
 * @Date: 2025-02-21 15:26:07
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-03-05 18:39:40
 * @LastEditors: Bo Qian
 * @Description: Materials for Stokes equation
 * @FilePath: /viscosity_sintering/include/materials/StokesMaterial.h
 */

#pragma once

#include "Material.h"
#include "DerivativeMaterialInterface.h"

class StokesMaterial : public DerivativeMaterialInterface<Material>
{
  public:
    static InputParameters validParams();
    StokesMaterial(const InputParameters & parameters);

  protected:
    virtual void computeQpProperties() override;

    const Real _mu_volume;
    const Real _mu_ratio;
    const Real _epsilon_Nc;
    const Real _alpha;
    const Real _kc;

    const VariableValue & _c;
    const VariableValue & _u;
    const VariableValue & _v;
    const VariableValue & _w;
    const VariableValue & _p;
    const VariableGradient & _grad_c;
    const VariableGradient & _grad_u;
    const VariableGradient & _grad_v;
    const VariableGradient & _grad_w;
    const VariableGradient & _grad_p;
    
		MaterialProperty<Real> & _Nc;
		MaterialProperty<Real> & _mu_eff;
		MaterialProperty<Real> & _kappa_C;

};

extern template class DerivativeMaterialInterface<Material>;