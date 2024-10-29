/*
 * @Author: Bo Qian
 * @Date: 2024-10-29 11:01:43
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:39:29
 * @LastEditors: Bo Qian
 * @Description: Materials for Viscosity Sintering App
 * @FilePath: /viscosity_sintering/src/materials/ViscositySinteringMaterial.C
 */

#include "ViscositySinteringMaterial.h"

registerMaterial(ViscositySinteringAPP, ViscositySinteringMaterial);

InputParameters
ViscositySinteringMaterial::validParams()
{
  InputParameters params = Material::validParams();
  params.addClassDescription("Material Parameters of Viscosity Sintering App");
  params.addParam<Real>("mu_volume", 0.4, "The volume viscosity of the system");
  params.addParam<Real>("mu_ratio", 0.001, "The ratio of the vapor viscosity to the volume viscosity");
  params.addParam<Real>("epsilon_Nc", 3.01, "epsilon in the interpolation function N(C)");
  params.addParam<Real>("M", 0.005, "The mobility of the system, or relaxation parameter");
  params.addParam<Real>("alpha", 120.00, "The alpha parameter of the system");
  params.addParam<Real>("kappa_C", 135.00, "The kappa_C parameter of the system");
  params.addRequiredCoupledVar("c", "Concentration variable");
  return params;
}
