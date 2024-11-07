/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 14:05:01
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-07 14:28:17
 * @LastEditors: Bo Qian
 * @Description: Kernel of x-component of the Stokes equation
 * @FilePath: /viscosity_sintering/include/kernels/StokesX.h
 */

#pragma once

#include "Kernel.h"
#include "MooseEnum.h"

class StokesX : public Kernel
{
public:
	StokesX(const InputParameters & parameters);
	static InputParameters validParams();

protected:

	MooseEnum _dim;
	enum Dimension { two_dimension, three_dimension };

	// virtual Real computeQpResidual() override
	// {
	// 	return 0.0;
	// }

  // virtual Real computeQpJacobian() override
	// {
	// 	return 0.0;
	// }

  // virtual Real computeQpOffDiagJacobian(unsigned jvar) override
	// {
	// 	return 0.0;
	// }

	virtual Real computeQpResidual() override;
	virtual Real computeQpJacobian() override;

	virtual Real velocityTermX(Dimension type);
	virtual Real pressureTermX();
	virtual Real surfaceTensionTermX(Dimension type);
	virtual Real ResidualX(Dimension type);

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _kappa_c;

	const VariableValue & _c;
	const VariableGradient & _grad_c;
	const VariableValue & _p;

	const VariableValue & _v_vel;
	const VariableValue & _w_vel;

	const VariableGradient & _grad_v_vel;
	const VariableGradient & _grad_w_vel;

};