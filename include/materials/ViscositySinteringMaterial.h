/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:08
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-12 15:09:46
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
		virtual void computeQpProperties(const Point & p);

	private:

		const Real _mu_volume;
		const Real _mu_ratio;
		const Real _epsilon_Nc;
		const Real _M;
		const Real _alpha;
		const Real _kappa_C;

    const Real _delta;
    const Real _radius;
    const int _number_x;  // 修改为 int 类型
    const int _number_y;  // 修改为 int 类型
    const Real _omega;
    std::vector<int> _domain; 
    std::vector<std::pair<int, int>> _particle_centers_coordinate;
    std::vector<int> _particle_radius;
		std::vector<double> _C_initial;

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
		MaterialProperty<Real> & _C_IC;

    std::pair<std::vector<std::pair<int, int>>, std::vector<int>> 
    particleCentersWithoutTemplate(int radius_particle, int particle_number_total, int number_x, int number_y, const std::vector<int>& domain);

};

extern template class DerivativeMaterialInterface<Material>;