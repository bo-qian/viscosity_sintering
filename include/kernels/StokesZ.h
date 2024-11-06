/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:57:14
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-06 23:00:42
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

	virtual Real velocityTermZ();
	virtual Real pressureTermZ();
	virtual Real surfaceTensionTermZ();
	virtual Real ResidualZ();

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _kappa_c;

	const VariableValue & _c;
	const VariableGradient & _grad_c;
	const VariableValue & _p;

	const VariableValue & _u_vel;
	const VariableValue & _v_vel;

	const VariableGradient & _grad_u_vel;
	const VariableGradient & _grad_v_vel;

};