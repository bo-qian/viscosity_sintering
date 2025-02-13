/*
 * @Author: Bo Qian
 * @Date: 2025-02-13 14:47:57
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-13 15:52:04
 * @LastEditors: Bo Qian
 * @Description: StressTensor.C
 * @FilePath: /viscosity_sintering/src/auxkernels/StressTensor.C
 */

#ifndef EXCLUDE_THIS_FILE

#include "StressTensor.h"
#include "RankTwoTensor.h"

registerMooseObject("viscosity_sinteringApp", StressTensor);

InputParameters
StressTensor::validParams()
{
  InputParameters params = AuxKernel::validParams();
  MooseEnum dims("2=2 3");
  params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
  params.addClassDescription("StressTensor");
  params.addRequiredCoupledVar("x_velocity", "The x-component of the velocity");
  params.addRequiredCoupledVar("y_velocity", "The y-component of the velocity");
  params.addCoupledVar("z_velocity", 0, "The z-component of the velocity");
  params.addRequiredCoupledVar("pressure", "The pressure variable");
  return params;
}

StressTensor::StressTensor(const InputParameters & parameters)
  : AuxKernel(parameters),
  _dim(getParam<MooseEnum>("dim")),
  _u(coupledValue("x_velocity")),
  _v(coupledValue("y_velocity")),
  _w(coupledValue("z_velocity")),
  _p(coupledValue("pressure")),
  _grad_u(coupledGradient("x_velocity")),
  _grad_v(coupledGradient("y_velocity")),
  _grad_w(coupledGradient("z_velocity")),
  _mu_eff(getMaterialProperty<Real>("mu_eff")),
  _F_loc(getMaterialProperty<Real>("F_loc"))
{
}

// RealVectorValue
// StressTensor::computeQpVelocityGrad()
// {
//   switch (_dim)
//   {
//     case 2:
//       return RealVectorValue(_grad_u[_qp], _grad_v[_qp]);
//     case 3:
//       return RealVectorValue(_grad_u[_qp], _grad_v[_qp], _grad_w[_qp]);
//     default:
//       mooseError("Invalid dimension value, should be 2 or 3, from computeQpVelocity/StressTensor.C");
//   }
// }



Real StressTensor::computeValue()
{
  RankTwoTensor grad_velocity;
  switch (_dim)
  {
    case 2:
      grad_velocity.fillFromGradient(_grad_u[_qp], _grad_v[_qp]);
      break;
    case 3:
      grad_velocity.fillFromGradient(_grad_u[_qp], _grad_v[_qp], _grad_w[_qp]);
      break;
    default:
      mooseError("Invalid dimension value, should be 2 or 3, from computeValue/StressTensor.C");
  }
  
  RankTwoTensor stress;
  stress = _mu_eff[_qp] * (grad_velocity + grad_velocity.transpose());
  stress += (_p[_qp] - _F_loc[_qp]) * RankTwoTensor::identity(); 

  return stress;
}

#endif