/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-24 09:08:53
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-28 17:24:32
 * @LastEditors: Bo Qian
 * @Description: Header file for Incompressibility Kernel
 * @FilePath: /viscosity_sintering/include/kernels/Incompressibility.h
 */

// # ifndef EXCLUDE_THIS_FILE

# pragma once

#include "Kernel.h"

class Incompressibility : public Kernel
{
	public:
		Incompressibility(const InputParameters & parameters);
		static InputParameters validParams();

	protected:
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