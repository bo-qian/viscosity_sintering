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