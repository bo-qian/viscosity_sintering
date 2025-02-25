/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:08
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-21 15:21:24
 * @LastEditors: Bo Qian
 * @Description: Header file for Viscosity Sintering Material
 * @FilePath: /viscosity_sintering/include/materials/ViscositySinteringMaterial.h
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

	private:

		const Real _mu_volume;
		const Real _mu_ratio;
		const Real _epsilon_Nc;
		const Real _M;
		const Real _alpha;
		const Real _kc;
		const Real _theta;

		const VariableValue & _c;
		const VariableValue & _c_old;

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

};

extern template class DerivativeMaterialInterface<Material>;