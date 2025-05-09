/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:39:09
 * @FilePath: /viscosity_sintering/include/kernels/Incompressibility.h
 * @Description: Header file for Incompressibility class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

# pragma once

#include "Kernel.h"
#include "MooseEnum.h"

class Incompressibility : public Kernel
{
	public:
		Incompressibility(const InputParameters & parameters);
		static InputParameters validParams();

	protected:
		MooseEnum _dim;
		virtual RealVectorValue computeQpVelocity();
		virtual Real computeQpDivVelocity();

		virtual Real computeQpResidual() override;
		virtual Real computeQpJacobian() override;
		virtual Real computeQpOffDiagJacobian(unsigned jvar) override;
	
	private:
		const unsigned int _u_velocity_var;
		const unsigned int _v_velocity_var;
		const unsigned int _w_velocity_var;

		const VariableValue & _u_velocity;
		const VariableValue & _v_velocity;
		const VariableValue & _w_velocity;

		const VariableGradient & _grad_u_velocity;
		const VariableGradient & _grad_v_velocity;
		const VariableGradient & _grad_w_velocity;
 
};