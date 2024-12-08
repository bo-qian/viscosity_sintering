/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:08
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-29 13:55:23
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
		const Real _kappa_C;
		// const Real _dimension;

		const VariableValue & _c;
		unsigned int  _c_var;
		std::string _c_name;

		MaterialProperty<Real> & _Nc;
		MaterialProperty<Real> & _dNdc;
		MaterialProperty<Real> & _F_loc;
		MaterialProperty<Real> & _dF_loc;
		MaterialProperty<Real> & _dF2_loc;
		MaterialProperty<Real> & _mu_eff;
		MaterialProperty<Real> & _dmu_eff;
		MaterialProperty<Real> & _kappa_c;
		MaterialProperty<Real> & _mobility;

};

extern template class DerivativeMaterialInterface<Material>;