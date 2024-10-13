#include "CHCoupV.h"

registerMooseObject("viscosity_sinteringApp", CHCoupV);


InputParameters 
CHCoupV::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addRequiredCoupledVar("v", "The velocity variable coupled into CH equation");
  return params;
}

CHCoupV::CHCoupV(const InputParameters & parameters)
  : 
    Kernel(parameters),
    _v(coupledVectorValue("v"))  // 初始化速度矢量变量
{
}

// 计算残差项
Real
CHCoupV::computeQpResidual()
{
  return -_u[_qp] * (_v[_qp] * _grad_test[_i][_qp]);
}

// 计算雅可比矩阵项
Real 
CHCoupV::computeQpJacobian()
{
  return -_phi[_j][_qp] * (_v[_qp] * _grad_test[_i][_qp]);  // 残差对变量 C 的导数
}
