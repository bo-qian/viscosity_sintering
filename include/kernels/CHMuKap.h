/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-11 15:17:07
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2025-02-21 15:17:50
 * @LastEditors: Bo Qian
 * @Description: Kernel of the kappa term of Cahn-Hilliard equation
 * @FilePath: /viscosity_sintering/include/kernels/CHMuKap.h
 */

#include "Kernel.h"

class CHMuKap : public Kernel
{
public:
  static InputParameters validParams();

  CHMuKap(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<Real> & _kappa_c;
  const MaterialProperty<Real> & _theta;

   
  // Coupled variables
  const unsigned int _cvar;
  const VariableValue & _c;
  const VariableValue & _cvar_old;
  const VariableGradient & _grad_c;
  const VariableGradient & _grad_c_old;
};