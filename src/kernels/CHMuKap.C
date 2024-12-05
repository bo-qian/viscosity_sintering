/*
 * @Author: Bo Qian
 * @Date: 2024-11-11 15:16:57
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-12-03 14:31:39
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
  _kappa_c(getMaterialProperty<Real>("kappa_C")),
  _cvar(coupled("coupledvar")),
  _c(coupledValue("coupledvar")),
  _grad_c(coupledGradient("coupledvar"))
{
  
}

Real
CHMuKap::computeQpResidual()
{
  return _kappa_c[_qp] * (_grad_c[_qp](0) * _grad_test[_i][_qp](0) 
                          + _grad_c[_qp](1) * _grad_test[_i][_qp](1)
                          + _grad_c[_qp](2) * _grad_test[_i][_qp](2));
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
    return _kappa_c[_qp] * (_grad_phi[_j][_qp](0) * _grad_test[_i][_qp](0) 
                            + _grad_phi[_j][_qp](1) * _grad_test[_i][_qp](1)
                            + _grad_phi[_j][_qp](2) * _grad_test[_i][_qp](2));
  return 0.0;
}
