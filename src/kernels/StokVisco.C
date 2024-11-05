/*
 * @Author: Bo Qian
 * @Date: 2024-10-13 16:38:35
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 14:37:59
 * @LastEditors: Bo Qian
 * @Description: Kernel for the viscosity term in the Stokes equation
 * @FilePath: /viscosity_sintering/src/kernels/StokVisco.C
 */

#ifndef EXCLUDE_THIS_FILE

#include "StokVisco.h"
// #include "MaterialParameters.h"


registerMooseObject("viscosity_sinteringApp", StokVisco);

InputParameters
StokVisco::validParams()
{
  InputParameters params = Kernel::validParams();
  // InputParameters params = MaterialParameters::validParams();
  params.addClassDescription("Split formulation Cahn-Hilliard Kernel that uses a DerivativeMaterial Free Energy");
  // params.addRequiredCoupledVar("c", "Concentration variable");
  params.addRequiredCoupledVar("u", "x-velocity variable");
  params.addCoupledVar("v", "y-velocity variable");
  params.addCoupledVar("w", "z-velocity variable");

  return params;
}

StokVisco::StokVisco(const InputParameters & parameters)
  : Kernel(parameters),
  _mu_eff(getMaterialProperty<Real>("mu_eff")),

  // Coupled variables
  _u_vel(coupledValue("u")),
  _v_vel(coupledValue("v")),
  _w_vel(coupledValue("w")),

  //
  _grad_u_vel(coupledGradient("u")),
  _grad_v_vel(coupledGradient("v")),
  _grad_w_vel(coupledGradient("w"))

{

}

RealVectorValue
StokVisco::computeQpVelocity()
{
  // 计算速度矢量
  return RealVectorValue(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
}

RealTensorValue
StokVisco::computeQpGradVelocity()
{
  // 计算速度梯度张量
  return RealTensorValue(_grad_u_vel[_qp](0), _grad_u_vel[_qp](1), _grad_u_vel[_qp](2),
                         _grad_v_vel[_qp](0), _grad_v_vel[_qp](1), _grad_v_vel[_qp](2),
                         _grad_w_vel[_qp](0), _grad_w_vel[_qp](1), _grad_w_vel[_qp](2));
}


Real
StokVisco::computeQpResidual()
{
    // 创建 MaterialParameters 的实例并传递参数
    // MaterialParameters materialParams(this->parameters());
    
    // // 计算 N(C)、f_loc 和 mu_eff
    // materialParams.ComputeQpNc();
    // materialParams.ComputeQpFloc();
    // materialParams.ComputeQpMuEff();
    
    // 访问 _mu_eff
    // Real mu_eff = materialParams._mu_eff[_qp];
    RealVectorValue _U = computeQpVelocity();
    RealTensorValue _grad_U = computeQpGradVelocity();
  
  return _mu_eff[_qp] * (_grad_U * _grad_test[_i][_qp] + _grad_U.transpose() * _grad_test[_i][_qp]);
}

Real
StokVisco::computeQpJacobian()
{
    // 创建 MaterialParameters 的实例并传递参数
    // MaterialParameters materialParams(this->parameters());
    
    // // 计算 N(C)、f_loc 和 mu_eff
    // materialParams.ComputeQpNc();
    // materialParams.ComputeQpFloc();
    // materialParams.ComputeQpMuEff();
    
    // 访问 _mu_eff
    // Real mu_eff = materialParams._mu_eff[_qp];

	return -_mu_eff[_qp] * ((_grad_phi[_j][_qp] + _grad_phi[_j][_qp].transpose()) * _grad_test[_i][_qp]);
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