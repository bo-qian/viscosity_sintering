/*
 * @Author: Bo Qian
 * @Date: 2024-11-11 15:16:57
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-25 15:08:15
 * @LastEditors: Bo Qian
 * @Description: Kernel of the kappa term of Cahn-Hilliard equation
 * @FilePath: /viscosity_sintering/src/kernels/CHMuKap.C
 */

#include "CHMuKap.h"

registerMooseObject("viscosity_sinteringApp", CHMuKap);

InputParameters
CHMuKap::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of the kappa term of Cahn-Hilliard equation");
  params.addRequiredCoupledVar("coupledvar", "coupled variable");
  return params;
}

CHMuKap::CHMuKap(const InputParameters & parameters)
  : Kernel(parameters),
  _kappa_c(getMaterialProperty<Real>("kappa_C_value")),
  _theta(getMaterialProperty<Real>("theta_value")),
  _cvar(coupled("coupledvar")),
  _c(coupledValue("coupledvar")),
  _grad_c(coupledGradient("coupledvar"))
{
  
}

Real
CHMuKap::computeQpResidual()
{
  return - _theta[_qp] * _kappa_c[_qp] * (_grad_c[_qp] * _grad_test[_i][_qp]);
}

Real
CHMuKap::computeQpJacobian()
{
  return 0;
}

Real
CHMuKap::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _cvar)
    return - _theta[_qp] * _kappa_c[_qp] * (_grad_phi[_j][_qp] * _grad_test[_i][_qp]);
  return 0.0;
}
