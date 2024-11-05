/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 14:05:01
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 20:42:32
 * @LastEditors: Bo Qian
 * @Description: Kernel of x-component of the Stokes equation
 * @FilePath: /viscosity_sintering/include/kernels/StokesX.h
 */

#pragma once

#include "Kernel.h"

class StokesX : public Kernel
{
public:
	StokesX(const InputParameters & parameters);
	static InputParameters validParams();

protected:

	// virtual Real computeQpResidual() override
	// {
	// 	return 0.0;
	// }

  // virtual Real computeQpJacobian() 
	// {
	// 	return 0.0;
	// }

  // virtual Real computeQpOffDiagJacobian(unsigned jvar) override
	// {
	// 	return 0.0;
	// }

	virtual Real computeQpResidual() override;

	virtual Real velocityTermX();
	virtual Real pressureTermX();
	virtual Real surfaceTensionTermX();
	virtual Real ResidualX();

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _kappa_C;

	const VariableValue & _c;
	const VariableGradient & _grad_c;
	const VariableValue & _p;

	const VariableValue & _v_vel;
	const VariableValue & _w_vel;

	const VariableGradient & _grad_v_vel;
	const VariableGradient & _grad_w_vel;

};