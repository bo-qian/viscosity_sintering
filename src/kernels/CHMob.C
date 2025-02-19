/*
 * @Author: Bo Qian
 * @Date: 2024-11-11 12:27:32
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-18 15:56:15
 * @LastEditors: Bo Qian
 * @Description: Kernel of the mobility term of Cahn-Hilliard equation
 * @FilePath: /viscosity_sintering/src/kernels/CHMob.C
 */

#include "CHMob.h"

registerMooseObject("viscosity_sinteringApp", CHMob);

InputParameters
CHMob::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of the mobility term of Cahn-Hilliard equation");
  params.addRequiredCoupledVar("coupledvar", "variant of phase field");
  return params;
}

CHMob::CHMob(const InputParameters & parameters)
  : Kernel(parameters),
  _mobility(getMaterialProperty<Real>("M_value")),
  _muvar(coupled("coupledvar")),
  _mu(coupledValue("coupledvar")),
  _grad_mu(coupledGradient("coupledvar"))
{
  
}

Real 
CHMob::computeQpResidual()
{
  return _mobility[_qp] * _grad_mu[_qp] * _grad_test[_i][_qp];
}

Real
CHMob::computeQpJacobian()
{
  return 0.0;
}

Real
CHMob::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _muvar)
    return _mobility[_qp] * _grad_phi[_j][_qp] * _grad_test[_i][_qp];
  return 0.0;
}
