// This file is part of the MOOSE framework
// https://www.mooseframework.org
//
// All rights reserved, see COPYRIGHT for full restrictions
// https://github.com/idaholab/moose/blob/master/COPYRIGHT
//
// Licensed under LGPL 2.1, please see LICENSE for details
// https://www.gnu.org/licenses/lgpl-2.1.html

#include "MultiParticles.h"
#include <cmath>

registerMooseObject("viscosity_sinteringApp", MultiParticles);

InputParameters
MultiParticles::validParams()
{
    InputParameters params = InitialCondition::validParams();
    params.addRequiredParam<Real>("delta", "The thinkness of  interface");
    params.addRequiredParam<Real>("radius", "The radius of the particles");
    params.addRequiredParam<Real>("center_x", "The x coordinate of the center of the particles");
    params.addRequiredParam<Real>("center_y", "The y coordinate of the center of the particles");
    params.addRequiredParam<Real>("center_omega", "The omega coordinate of the center of the particles");
    return params;
}

MultiParticles::MultiParticles(const InputParameters & parameters)
  : InitialCondition(parameters), 
  _delta(getParam<Real>("delta")), 
  _radius(getParam<Real>("radius")), 
  _center_x(getParam<Real>("center_x")), 
  _center_y(getParam<Real>("center_y")), 
  _center_omega(getParam<Real>("center_omega"))
{
}

// This is the primary function custom ICs must implement.
Real
MultiParticles::value(const Point & p)
{
    // The Point class is defined in libMesh.  The spatial coordinates x,y,z can be accessed
    // individually using the parenthesis operator and a numeric index from 0..2

    // Compute the Euclidean distance between (x, y) and (x0, y0)
    double distance = std::sqrt((p(0) - _center_x) * (p(0) - _center_x) + (p(1) - _center_y) * (p(1) - _center_y)) - _radius;
    
    // Compute the argument of the tanh function
    double argument = distance * 2 * std::atanh(_center_omega) / _delta;
    
    // Compute the final result using the formula
    double result = 0.5 * (1.0 - std::tanh(argument));

    return result;
}