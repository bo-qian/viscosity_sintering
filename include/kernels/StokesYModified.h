/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-05-03 15:38:47
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:34:17
 * @FilePath: /viscosity_sintering/include/kernels/StokesYModified.h
 * @Description: Header file for StokesYModified class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "Kernel.h"
#include "MooseEnum.h"

class StokesYModified : public Kernel
{
public:
	StokesYModified(const InputParameters & parameters);
	static InputParameters validParams();

protected:

	MooseEnum _dim;

	virtual Real computeQpResidual() override;
	virtual Real computeQpJacobian() override;
	virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

	virtual Real velocityTermY();
	virtual Real pressureTermY();
	virtual Real surfaceTensionTermY();

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

	// const VariableValue & _u_vel;
	const unsigned int _u_vel_var;
	const VariableValue & _u_vel;
	const unsigned int _w_vel_var;
	const VariableValue & _w_vel;

	// const VariableGradient & _grad_u_vel;
	const VariableGradient & _grad_u_vel;
	const VariableGradient & _grad_w_vel;

};