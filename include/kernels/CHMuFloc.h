/*
 * @Author: bo-qian bqian@shu.edu.cn
 * @Date: 2025-02-11 17:10:06
 * @LastEditors: bo-qian bqian@shu.edu.cn
 * @LastEditTime: 2025-05-08 12:39:47
 * @FilePath: /viscosity_sintering/include/kernels/CHMuFloc.h
 * @Description: Header file for CHMuFloc class
 * Copyright (c) 2025 by Bo Qian, All Rights Reserved. 
 */

#include "Kernel.h"

class CHMuFloc : public Kernel
{
  public:
    static InputParameters validParams();

    CHMuFloc(const InputParameters & parameters);

  protected:
    virtual Real computeQpResidual() override;
    virtual Real computeQpJacobian() override;
    virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

    const MaterialProperty<Real> & _dfloc_dc;
    const MaterialProperty<Real> & _df2loc_dc;

    // Coupled variables
    const unsigned int _cvar;
    const VariableValue & _c;
};