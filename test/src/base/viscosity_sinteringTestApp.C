//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "viscosity_sinteringTestApp.h"
#include "viscosity_sinteringApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
viscosity_sinteringTestApp::validParams()
{
  InputParameters params = viscosity_sinteringApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

viscosity_sinteringTestApp::viscosity_sinteringTestApp(InputParameters parameters) : MooseApp(parameters)
{
  viscosity_sinteringTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

viscosity_sinteringTestApp::~viscosity_sinteringTestApp() {}

void
viscosity_sinteringTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  viscosity_sinteringApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"viscosity_sinteringTestApp"});
    Registry::registerActionsTo(af, {"viscosity_sinteringTestApp"});
  }
}

void
viscosity_sinteringTestApp::registerApps()
{
  registerApp(viscosity_sinteringApp);
  registerApp(viscosity_sinteringTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
viscosity_sinteringTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  viscosity_sinteringTestApp::registerAll(f, af, s);
}
extern "C" void
viscosity_sinteringTestApp__registerApps()
{
  viscosity_sinteringTestApp::registerApps();
}
