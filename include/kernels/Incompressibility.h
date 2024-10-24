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