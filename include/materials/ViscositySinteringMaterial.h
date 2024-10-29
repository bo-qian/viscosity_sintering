/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:08
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:40:47
 * @LastEditors: Bo Qian
 * @Description: Header file for Viscosity Sintering Material
 * @FilePath: /viscosity_sintering/include/materials/ViscositySinteringMaterial.h
 */

#pragma once

#include "Material.h"

class ViscositySinteringMaterial : public Material
{
	public:
		ViscositySinteringMaterial(const InputParameters & parameters);

		static InputParameters validParams();

	protected:
		virtual void computeQpProperties() override;

	private:

		const Real _mu_volume;
		const Real _mu_ratio;
		const Real _epsilon_Nc;
		const Real _M;
		const Real _alpha;
		const Real _kappa_C;

		const VariableValue & _c;
		unsigned int  _c_var;
		std::string _c_name;

		MaterialProperty<Real> & _Nc;
		MaterialProperty<Real> & _dNdc;
		MaterialProperty<Real> & _F_loc;
		MaterialProperty<Real> & _dF_loc;
		MaterialProperty<Real> & _mu_eff;
		MaterialProperty<Real> & _dmu_eff;

};