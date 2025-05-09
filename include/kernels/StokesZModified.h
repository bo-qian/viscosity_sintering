/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 15:38:49
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:33:59
 * @FilePath: /viscosity_sintering/include/kernels/StokesZModified.h
 * @Description: Header file for StokesZModified class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */


#pragma once

#include "Kernel.h"

class StokesZModified : public Kernel
{
public:
	StokesZModified(const InputParameters & parameters);
	static InputParameters validParams();

protected:

	virtual Real computeQpResidual() override;
	virtual Real computeQpJacobian() override;
	virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

	virtual Real velocityTermZ();
	virtual Real pressureTermZ();
	virtual Real surfaceTensionTermZ();

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _alpha;
	const MaterialProperty<Real> & _kappa_c;

	const unsigned int _cvar;
	const VariableValue & _c;
	const VariableGradient & _grad_c;
	const VariableValue & _mu;
	const VariableGradient & _grad_mu;
	const unsigned int _pvar;
	const VariableValue & _p;

	const unsigned int _u_vel_var;
	const VariableValue & _u_vel;
	const unsigned int _v_vel_var;
	const VariableValue & _v_vel;

	const VariableGradient & _grad_u_vel;
	const VariableGradient & _grad_v_vel;

};