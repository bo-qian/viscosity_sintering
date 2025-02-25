
[Mesh]
  file = viscosity_sintering_IC_2D_out.e
[]

[Variables]
  [./c]
    order = FIRST
    family = LAGRANGE
    initial_from_file_var = c
  [../]
  [./mu]
    order = FIRST
    family = LAGRANGE
    [./InitialCondition]
        type = ConstantIC
        value = 0.0
    [../]
  [../]
[]

[AuxVariables]
  [./u]
    order = SECOND
    family = LAGRANGE
    initial_from_file_var = u
  [../]
  [./v]
    order = SECOND
    family = LAGRANGE
    initial_from_file_var = v
  [../]
  [./p]
    order = FIRST
    family = LAGRANGE
    initial_from_file_var = p
  [../]
  [./F_density]
    order = FIRST
    family = MONOMIAL
    # initial_from_file_var = F_density
  [../]
  [./V_Magnitude]
    order = FIRST
    family = MONOMIAL
    # initial_from_file_var = V_Magnitude
  [../]
[]

[Postprocessors]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = F_density
    execute_on = 'initial timestep_end'
  [../]
[]

[Kernels]
  # Stokes kernels
  # [./StokesX]
  #   type = StokesX
  #   variable = u
  #   dim = 2
  #   phase_field = c
  #   pressure = p
  #   y_velocity = v
  # [../]
  # [./StokesY]
  #   type = StokesY
  #   variable = v
  #   dim = 2
  #   phase_field = c
  #   pressure = p
  #   x_velocity = u
  # [../]
  # [./Incompressibility]
  #   type = Incompressibility
  #   variable = p
  #   dim = 2
  #   x_velocity = u
  #   y_velocity = v
  # [../]

  # Cahn Hilliard kernels
  [./dt_C]
    type = TimeDerivative
    variable = c
  [../]
  
  [./CH_CoupleV]
    type = CHCoupV
    variable = c
    x_velocity = u
    y_velocity = v
  [../]

  [./CHMob]
    type = CHMob
    variable = c
    coupledvar = mu
  [../]


  [./CHMuFloc]
    type = CHMuFloc
    variable = mu
    coupledvar = c
  [../]
  [./CHMuKap]
    type = CHMuKap
    variable = mu
    coupledvar = c
  [../]
[]

[AuxKernels]
  [./TotalFreeEnergy]
    type = VSTotalFreeEnergy
    variable = F_density
    phase_field = c
  [../]
  [./VelocityMagnitude]
    type = VelocityMagnitude
    variable = V_Magnitude
    dim = 2
    x_velocity = u
    y_velocity = v
  [../]
[]

# [BCs]
#   [./bcs_u]
#     type = DirichletBC
#     variable = u
#     boundary = '0 1 2 3'
#     value = 0
#   [../]
#   [./bcs_v]
#     type = DirichletBC
#     variable = v
#     boundary = '0 1 2 3'
#     value = 0
#   [../]
# []


[Materials]
  [./ViscosityMaterial]
    type = ViscositySinteringMaterial
    cvar = c
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Transient
  solve_type = NEWTON
  # scheme = bdf2

  petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  petsc_options_value = 'lu 1500 superlu_dist'

  # petsc_options_iname = '-pc_type -pc_factor_mat_solver_package -ksp_type'
  # petsc_options_value = 'lu superlu_dist gmres'

  # reuse_preconditioner = true
  # reuse_preconditioner_max_linear_its = 20

  # l_max_its = 500
  # l_tol = 1e-6
  # nl_max_its = 30
  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6

  dt = 0.01
  start_time = 0.0
  end_time = 0.01
[]


[Outputs]
  exodus = true
  time_step_interval = 1
  perf_graph = true
  [./display]
    type = Console
    max_rows = 12
  [../]
[]

[Debug]
  show_material_props = true
  # show_execution_order = true
[]