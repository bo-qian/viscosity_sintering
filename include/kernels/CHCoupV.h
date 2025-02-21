/*** 
 * @Author: Bo Qian
 * @Date: 2024-10-12 20:20:46
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-28 15:06:38
 * @LastEditors: Bo Qian
 * @Description: Header file for CH equation coupled with velocity
 * @FilePath: /viscosity_sintering/include/kernels/CHCoupV.h
 */

#pragma once

#include "Kernel.h"

/**
 * Define the Kernel for a convection operator that looks like:
 *
 * (-u * v , grad(test))
 *
 * where v is a coupled.
 */
class CHCoupV : public Kernel
{
public:
  CHCoupV(const InputParameters & parameters);
  static InputParameters validParams();

protected:
  virtual RealVectorValue computeQpVelocity();

  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

private:
  // const VectorVariableValue & _v;
  const unsigned int _u_velocoty_var;
  const unsigned int _v_velocity_var;
  const unsigned int _w_velocity_var;
  const VariableValue & _u_velocity;
  const VariableValue & _v_velocity;
  const VariableValue & _w_velocity;
};
