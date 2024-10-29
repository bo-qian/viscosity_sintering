/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-24 09:08:53
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-10-29 13:46:02
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
		virtual Real computeQpResidual();
		virtual Real computeQpJacobian();
	
	private:
		const VectorVariableValue & _v;
		const unsigned int _v_var;
		const VariableValue & _div_v;
};