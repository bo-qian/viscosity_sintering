/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:44:42
 * @FilePath: /viscosity_sintering/include/materials/ViscositySinteringMaterial.h
 * @Description: Header file for ViscositySinteringMaterial class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "Material.h"
#include "DerivativeMaterialInterface.h"

class ViscositySinteringMaterial : public DerivativeMaterialInterface<Material>
{
	public:
		static InputParameters validParams();
		ViscositySinteringMaterial(const InputParameters & parameters);

	protected:
	
		virtual void computeQpProperties() override;

		const Real _mu_volume;
		const Real _mu_ratio;
		const Real _epsilon_Nc;
		const Real _M;
		const Real _alpha;
		const Real _kc;
		const Real _theta;

		const VariableValue & _c;
		const VariableValue & _c_old;
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
		MaterialProperty<Real> & _mobility;
		MaterialProperty<Real> & _theta_var;
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
		MaterialProperty<Real> & _stress_xx_modified;
		MaterialProperty<Real> & _stress_yy_modified;
		MaterialProperty<Real> & _stress_zz_modified;
		MaterialProperty<RankTwoTensor> & _stress_modified;

};

extern template class DerivativeMaterialInterface<Material>;