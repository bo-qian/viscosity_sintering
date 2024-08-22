// This file is part of the MOOSE framework
// https://www.mooseframework.org
//
// All rights reserved, see COPYRIGHT for full restrictions
// https://github.com/idaholab/moose/blob/master/COPYRIGHT
//
// Licensed under LGPL 2.1, please see LICENSE for details
// https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// MOOSE Includes
#include "InitialCondition.h"

/**
 * MultiParticles just returns a constant value.
 */
class MultiParticles : public InitialCondition
{
public:
  /**
   * Constructor: Same as the rest of the MOOSE Objects
   */
  MultiParticles(const InputParameters & parameters);

  static InputParameters validParams();

  /**
   * The value of the variable at a point.
   *
   * This must be overriden by derived classes.
   */
  virtual Real value(const Point & p) override;

private:
    const Real _delta;
    const Real _radius;
    const Real _center_x;
    const Real _center_y;
    const Real _center_omega;
};