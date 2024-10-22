#ifndef EXCLUDE_THIS_FILE

#include "StokVisco.h"
#include "MaterialParameters.h"


registerMooseObject("viscosity_sinteringApp", StokVisco);

InputParameters
StokVisco::validParams()
{
  InputParameters params = Kernel::validParams();
  // InputParameters params = MaterialParameters::validParams();
  params.addClassDescription("Split formulation Cahn-Hilliard Kernel that uses a DerivativeMaterial Free Energy");
  params.addRequiredCoupledVar("c", "Concentration variable");

  return params;
}

StokVisco::StokVisco(const InputParameters & parameters)
  : Kernel(parameters)
    // MaterialParameters(parameters),
    // _c(coupledValue("c")),
    // _c_var(coupled("c"))
    // _grad_u(Kernel::grad_var("u")),
    // materialParameters(parameters)
{

}

Real
StokVisco::computeQpResidual()
{
    // 创建 MaterialParameters 的实例并传递参数
    MaterialParameters materialParams(this->parameters());
    
    // 计算 N(C)、f_loc 和 mu_eff
    materialParams.ComputeQpNc();
    materialParams.ComputeQpFloc();
    materialParams.ComputeQpMuEff();
    
    // 访问 _mu_eff
    Real mu_eff = materialParams._mu_eff[_qp];
  
  return mu_eff * ((_grad_u[_qp] + _grad_u[_qp].transpose()) * _grad_test[_i][_qp]);
}

Real
StokVisco::computeQpJacobian()
{
    // 创建 MaterialParameters 的实例并传递参数
    MaterialParameters materialParams(this->parameters());
    
    // 计算 N(C)、f_loc 和 mu_eff
    materialParams.ComputeQpNc();
    materialParams.ComputeQpFloc();
    materialParams.ComputeQpMuEff();
    
    // 访问 _mu_eff
    Real mu_eff = materialParams._mu_eff[_qp];

	return -mu_eff * ((_grad_phi[_j][_qp] + _grad_phi[_j][_qp].transpose()) * _grad_test[_i][_qp]);
}

// Real
// StokVisco::computeQpOffDiagJacobian(unsigned int jvar)
// {
//   if (jvar == _w_var)
//     return SplitCHCRes::computeQpOffDiagJacobian(jvar);

//   // get the coupled variable jvar is referring to
//   const unsigned int cvar = mapJvarToCvar(jvar);

//   return (*_d2Fdcdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
// }


#endif