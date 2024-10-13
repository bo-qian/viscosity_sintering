#pragma once

#include "Kernel.h"

/**
 * Define the Kernel for a convection operator that looks like:
 *
 * (-u * v , grad(test))
 *
 * where V is a given constant velocity field.
 */
class CHCoupV : public Kernel
{
public:
  CHCoupV(const InputParameters & parameters);
  static InputParameters validParams();

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

private:
  const VectorVariableValue & _v;
};
