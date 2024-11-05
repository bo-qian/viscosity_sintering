/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:46:54
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-05 20:42:23
 * @LastEditors: Bo Qian
 * @Description: Kernel of y-component of the Stokes equation
 * @FilePath: /viscosity_sintering/include/kernels/StokesY.h
 */

#pragma once

#include "Kernel.h"

class StokesY : public Kernel
{
public:
	StokesY(const InputParameters & parameters);
	static InputParameters validParams();

protected:

// 	virtual Real computeQpResidual() override
// 	{
// 		return 0.0;
// 	}

//   virtual Real computeQpJacobian() 
// 	{
// 		return 0.0;
// 	}

//   virtual Real computeQpOffDiagJacobian(unsigned jvar) override
// 	{
// 		return 0.0;
// 	}

	virtual Real computeQpResidual() override;

	virtual Real velocityTermY();
	virtual Real pressureTermY();
	virtual Real surfaceTensionTermY();
	virtual Real ResidualY();

	// virtual Real computeQpJacobian() override;
	// // If you need to calculate the Jacobian matrix of other variables, you can uncomment the following function
	// virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _kappa_C;

	const VariableValue & _c;
	const VariableGradient & _grad_c;
	const VariableValue & _p;

	// const VariableValue & _u_vel;
	const VariableValue & _u_vel;
	const VariableValue & _w_vel;

	// const VariableGradient & _grad_u_vel;
	const VariableGradient & _grad_u_vel;
	const VariableGradient & _grad_w_vel;

};