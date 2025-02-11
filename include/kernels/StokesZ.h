/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:57:14
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-11 19:37:47
 * @LastEditors: Bo Qian
 * @Description: Kernel of z-component of the Stokes equation
 * @FilePath: /viscosity_sintering/include/kernels/StokesZ.h
 */

#pragma once

#include "Kernel.h"

class StokesZ : public Kernel
{
public:
	StokesZ(const InputParameters & parameters);
	static InputParameters validParams();

protected:

	virtual Real computeQpResidual() override;
	virtual Real computeQpJacobian() override;
	virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

	virtual Real velocityTermZ();
	virtual Real pressureTermZ();
	virtual Real surfaceTensionTermZ();
	virtual Real muEffPhiZ();

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _dmu_eff;
	const MaterialProperty<Real> & _kappa_c;
	const MaterialProperty<Real> & _mu_volume;
	const MaterialProperty<Real> & _mu_ratio;
	const MaterialProperty<Real> & _epsilon_Nc;

	const unsigned int _cvar;
	const VariableValue & _c;
	const VariableGradient & _grad_c;
	const unsigned int _pvar;
	const VariableValue & _p;

	const unsigned int _u_vel_var;
	const VariableValue & _u_vel;
	const unsigned int _v_vel_var;
	const VariableValue & _v_vel;

	const VariableGradient & _grad_u_vel;
	const VariableGradient & _grad_v_vel;

};