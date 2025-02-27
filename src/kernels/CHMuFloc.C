/*
 * @Author: Bo Qian
 * @Date: 2024-11-11 14:38:40
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-18 20:49:31
 * @LastEditors: Bo Qian
 * @Description: Kernel of the local free energy term of Cahn-Hilliard equation
 * @FilePath: /viscosity_sintering/src/kernels/CHMuFloc.C
 */

# include "CHMuFloc.h"

registerMooseObject("viscosity_sinteringApp", CHMuFloc);

InputParameters
CHMuFloc::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Kernel of the local free energy term of Cahn-Hilliard equation");
  params.addRequiredCoupledVar("coupledvar", "coupled variable");
  return params;
}

CHMuFloc::CHMuFloc(const InputParameters & parameters)
  : Kernel(parameters),
  _dfloc_dc(getMaterialProperty<Real>("dF_loc")),
  _df2loc_dc(getMaterialProperty<Real>("dF2_loc")),
  _cvar(coupled("coupledvar")),
  _c(coupledValue("coupledvar"))
{
  
}

Real
CHMuFloc::computeQpResidual()
{
  return (_u[_qp] - _dfloc_dc[_qp]) * _test[_i][_qp];
}

Real
CHMuFloc::computeQpJacobian()
{
  return _phi[_j][_qp] * _test[_i][_qp];
}

Real 
CHMuFloc::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _cvar)
    return 0.0;
  return 0.0;
}
