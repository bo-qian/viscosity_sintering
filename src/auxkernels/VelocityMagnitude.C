/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-13 14:20:19
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:37:18
 * @FilePath: /viscosity_sintering/src/auxkernels/VelocityMagnitude.C
 * @Description: Auxkernel of the velocity magnitude
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */


#include "VelocityMagnitude.h"

registerMooseObject("viscosity_sinteringApp", VelocityMagnitude);

InputParameters
VelocityMagnitude::validParams()
{
  InputParameters params = AuxKernel::validParams();
  MooseEnum dims("2=2 3");
  params.addRequiredParam<MooseEnum>("dim", dims, "The dimension of the simulation");
  params.addClassDescription("VelocityMagnitude");
  params.addRequiredCoupledVar("x_velocity", "The x-component of the velocity");
  params.addRequiredCoupledVar("y_velocity", "The y-component of the velocity");
  params.addCoupledVar("z_velocity", 0, "The z-component of the velocity");
  return params;
}

VelocityMagnitude::VelocityMagnitude(const InputParameters & parameters)
  : AuxKernel(parameters),
  _dim(getParam<MooseEnum>("dim")),
  _u(coupledValue("x_velocity")),
  _v(coupledValue("y_velocity")),
  _w(coupledValue("z_velocity"))
{
}

Real
VelocityMagnitude::computeValue()
{
  switch (_dim)
  {
    case 2:
      return std::sqrt(_u[_qp] * _u[_qp] + _v[_qp] * _v[_qp]);
    case 3:
      return std::sqrt(_u[_qp] * _u[_qp] + _v[_qp] * _v[_qp] + _w[_qp] * _w[_qp]);
    default:
      mooseError("Invalid dimension value, should be 2 or 3, from VelocityMagnitude.C");
  }
}