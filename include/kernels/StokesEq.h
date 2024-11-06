/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 20:06:31
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 20:40:27
 * @LastEditors: Bo Qian
 * @Description: Kernel of the Stokes equation
 * @FilePath: /viscosity_sintering/include/kernels/StokesEq.h
 */

#ifndef EXCLUDE_THIS_FILE

#pragma once

#include "StokesX"
// #include "StokesY"
// #include "StokesZ"

class StokesEq : public StokesX, public StokesY, public StokesZ
{
	public:
		StokesEq(const InputParameters & parameters);
		static InputParameters validParams();

	protected:
		const Real 
		virtual Real computeQpResidual();
		// virtual Real computeQpJacobian();

};

#endif