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
		MaterialProperty<Real> & _dNdc;
		MaterialProperty<Real> & _F_loc;
		MaterialProperty<Real> & _dF_loc;
		MaterialProperty<Real> & _dF2_loc;
		MaterialProperty<Real> & _mu_eff;
		MaterialProperty<Real> & _dmu_eff;
		MaterialProperty<Real> & _alpha_var;
		MaterialProperty<Real> & _kappa_C;
		MaterialProperty<Real> & _mu_vol;
		MaterialProperty<Real> & _mu_rat;
		MaterialProperty<Real> & _epsilon_Nc_var;
    MaterialProperty<Real> & _stress_xx;
    MaterialProperty<Real> & _stress_xy;
    MaterialProperty<Real> & _stress_xz;
    MaterialProperty<Real> & _stress_yx;
    MaterialProperty<Real> & _stress_yy;
    MaterialProperty<Real> & _stress_yz;
    MaterialProperty<Real> & _stress_zx;
    MaterialProperty<Real> & _stress_zy;
    MaterialProperty<Real> & _stress_zz;
    MaterialProperty<RankTwoTensor> & _stress;

};

extern template class DerivativeMaterialInterface<Material>;