/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:40:10
 * @FilePath: /viscosity_sintering/include/kernels/CHCoupV.h
 * @Description: Header file for CHCoupV class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#pragma once

#include "Kernel.h"

class CHCoupV : public Kernel
{
public:
  CHCoupV(const InputParameters & parameters);
  static InputParameters validParams();

protected:
  virtual RealVectorValue computeQpVelocity();
  virtual RealVectorValue computeQpVelocityOld();

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
  const VariableValue & _u_velocity_old;
  const VariableValue & _v_velocity_old;
  const VariableValue & _w_velocity_old;
};
