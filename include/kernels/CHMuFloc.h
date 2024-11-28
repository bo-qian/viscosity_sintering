/*** 
 * @Author: Bo Qian
 * @Date: 2024-11-11 14:39:10
 * @Email: bqian@shu.edu.cn
 * @Location: Shanghai University
 * @LastEditTime: 2024-11-11 15:15:06
 * @LastEditors: Bo Qian
 * @Description: Kernel of the local free energy term of Cahn-Hilliard equation
 * @FilePath: /viscosity_sintering/include/kernels/CHMuFloc.h
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