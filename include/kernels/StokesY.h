/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-05 19:46:54
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-28 16:34:01
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
	virtual Real computeQpJacobian() override;
	virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

	virtual Real velocityTermY(Dimension type);
	virtual Real pressureTermY();
	virtual Real surfaceTensionTermY(Dimension type);
	virtual Real ResidualY(Dimension type);

	const MaterialProperty<Real> & _mu_eff;
	const MaterialProperty<Real> & _dmu_eff;
	const MaterialProperty<Real> & _kappa_c;

	const unsigned int _cvar;
	const VariableValue & _c;
	const VariableGradient & _grad_c;
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