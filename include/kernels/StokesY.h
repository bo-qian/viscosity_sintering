/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:46:54
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-06 23:17:58
 * @LastEditors: Bo Qian
 * @Description: Kernel of y-component of the Stokes equation
 * @FilePath: /viscosity_sintering/include/kernels/StokesY.h
 */

#pragma once

#include "Kernel.h"
#include "MooseEnum.h"

class StokesY : public Kernel
{
public:
	StokesY(const InputParameters & parameters);
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

	virtual Real velocityTermY(Dimension type);
	virtual Real pressureTermY();
	virtual Real surfaceTensionTermY(Dimension type);
	virtual Real ResidualY(Dimension type);

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _kappa_c;

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