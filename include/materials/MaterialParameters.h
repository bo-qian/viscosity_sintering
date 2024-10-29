/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-21 09:55:05
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:46:12
 * @LastEditors: Bo Qian
 * @Description: Header file for Material Parameters
 * @FilePath: /viscosity_sintering/include/materials/MaterialParameters.h
 */

# pragma once

# include "DerivativeMaterialInterface.h"
# include "Material.h"

class MaterialParameters : public DerivativeMaterialInterface<Material>
{
public:
	static InputParameters validParams();
	MaterialParameters(const InputParameters & parameters);
	// void ComputeQpProperties();
	virtual void ComputeQpNc();
	virtual void ComputeQpFloc();
	virtual void ComputeQpMuEff();

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