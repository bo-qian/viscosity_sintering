/*
 * @Author: Bo Qian
 * @Date: 2024-10-24 09:08:32
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:45:42
 * @LastEditors: Bo Qian
 * @Description: Incompressibility Kernel
 * @FilePath: /viscosity_sintering/src/kernels/Incompressibility.C
 */

# include "Incompressibility.h"

registerMooseObject("viscosity_sinteringApp", Incompressibility);

InputParameters
Incompressibility::validParams()
{
  InputParameters params = Kernel::validParams();
  params.addClassDescription("Incompressibility Kernel");
  params.addRequiredCoupledVar("v", "The velocity variable coupled into CH equation");
  return params;
}

Incompressibility::Incompressibility(const InputParameters & parameters)
  : Kernel(parameters),
    _v(coupledVectorValue("v")),  // 初始化速度矢量变量
    _v_var(coupled("v")),
    _div_v(coupledDiv("v"))
{
}

// 计算残差项
Real
Incompressibility::computeQpResidual()
{
  return _div_v[_qp] * _test[_i][_qp];
}

// 计算雅可比矩阵项
Real
Incompressibility::computeQpJacobian()
{
  return _div_v[_qp] * _test[_i][_qp];
}