/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 16:19:47
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-06 20:11:32
 * @FilePath: /viscosity_sintering/src/ics/ChemicalPotentialIC.C
 * @Description: Initial Condition for Chemical Potential
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "ChemicalPotentialIC.h"

registerMooseObject("viscosity_sinteringApp", ChemicalPotentialIC);

InputParameters
ChemicalPotentialIC::validParams()
{
  InputParameters params = InitialCondition::validParams();
  // params.addRequiredParam<MaterialPropertyName>("kappa_C_value", "Name of κ_C material property");
  // params.addRequiredParam<MaterialPropertyName>("df_dC", "Name of dF/dC material property");
  params.addRequiredCoupledVar("phase_field", "Phase field variable");
  return params;
}

ChemicalPotentialIC::ChemicalPotentialIC(const InputParameters & parameters)
  : InitialCondition(parameters),
    _Kappa_C(getMaterialProperty<Real>("kappa_C_value")),
    _alpha(getMaterialProperty<Real>("alpha_value")),
    _c(coupledValue("phase_field")),
    _grad_c(coupledGradient("phase_field"))
{
}

Real
ChemicalPotentialIC::value(const Point & p)
{
  const Real _df_dc = 2 * _alpha[_qp] * _c[_qp] * (1 - _c[_qp]) * (1 - 2 * _c[_qp]); 
  return _df_dc -_Kappa_C[_qp];
}