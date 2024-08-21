#include "viscosity_sinteringApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
viscosity_sinteringApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

viscosity_sinteringApp::viscosity_sinteringApp(InputParameters parameters) : MooseApp(parameters)
{
  viscosity_sinteringApp::registerAll(_factory, _action_factory, _syntax);
}

viscosity_sinteringApp::~viscosity_sinteringApp() {}

void
viscosity_sinteringApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<viscosity_sinteringApp>(f, af, s);
  Registry::registerObjectsTo(f, {"viscosity_sinteringApp"});
  Registry::registerActionsTo(af, {"viscosity_sinteringApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
viscosity_sinteringApp::registerApps()
{
  registerApp(viscosity_sinteringApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
viscosity_sinteringApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  viscosity_sinteringApp::registerAll(f, af, s);
}
extern "C" void
viscosity_sinteringApp__registerApps()
{
  viscosity_sinteringApp::registerApps();
}
