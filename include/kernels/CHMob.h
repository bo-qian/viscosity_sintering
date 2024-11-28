/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-11 12:27:49
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-28 14:49:24
 * @LastEditors: Bo Qian
 * @Description: Kernel of the mobility term of Cahn-Hilliard equation
 * @FilePath: /viscosity_sintering/include/kernels/CHMob.h
 */

#include "Kernel.h"

class CHMob : public Kernel
{
public:
  static InputParameters validParams();

  CHMob(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;

  const MaterialProperty<Real> & _mobility;

  // Coupled variables
  const unsigned int _muvar;
  const VariableValue & _mu;
  const VariableGradient & _grad_mu;
};