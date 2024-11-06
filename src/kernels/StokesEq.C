/*
 * @Author: Bo Qian
 * @Date: 2024-11-05 20:07:01
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 20:40:34
 * @LastEditors: Bo Qian
 * @Description: Kernel of the Stokes equation
 * @FilePath: /viscosity_sintering/src/kernels/StokesEq.C
 */

#ifndef EXCLUDE_THIS_FILE

#include "StokesEq.h"
#include "MooseEnum.h"

registerMooseObject("viscosity_sinteringApp", StokesEq);

InputParameters
StokesEq::validParams()
{
  InputParameters params = StokesX::validParams();
	MooseEnum dimension("2D 3D");
  params.addClassDescription("Kernel of the Stokes equation");
	params.addRequiredParam<MooseEnum>("Dimension", dimension, "The X, Y, or Z component");
  return params;
}

StokesEq::StokesEq(const InputParameters & parameters)
  : StokesX(parameters)
{
  
}

Real
StokesEq::computeQpResidual()
{ 
	Real stokesx = StokesX::ResidualX();
	Real stokesy = StokesY::ResidualY();
	Real stokesz = StokesZ::ResidualZ();
  return stokesx + stokesy + stokesz;
}

#endif