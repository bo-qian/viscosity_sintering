#include "CHCoupV.h"

registerMooseObject("viscosity_sinteringApp", CHCoupV);

InputParameters validParams<CHCoupV>()
{
  InputParameters params = validParams<Kernel>();
  params.addRequiredCoupledVar("v", "The velocity variable coupled into CH equation");
  return params;
}

CHCoupV::CHCoupV(const InputParameters & parameters)
  : Kernel(parameters),
    _v(coupledVectorValue("v"))  // 耦合速度变量
{
}

// 计算残差项
Real CHCoupV::computeQpResidual()
{
  Real residual = -_u[_qp] * _v[_qp] * _grad_test[_i][_qp];

  return residual;
}

// 计算雅可比矩阵项
Real CHCoupV::computeQpJacobian()
{
  // 计算残差对变量的导数
  // 在这里，假设导数为 - ∇ψ_C，具体导数形式取决于方程的形式
  return -_phi[_j][_qp] * _grad_test[_i][_qp];  // 残差对变量 C 的导数
}
