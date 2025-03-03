
[Mesh]
  file = viscosity_sintering_IC_3D_out.e
[]

[Problem]
  allow_initial_conditions_with_restart = true
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
  [./w]
    order = SECOND
    family = LAGRANGE
    initial_from_file_var = w
  [../]
  [./p]
    order = FIRST
    family = LAGRANGE
    initial_from_file_var = p
  [../]
[]

[AuxVariables]
  [./F_density]
    order = FIRST
    family = MONOMIAL
  [../]
  [./V_Magnitude]
    order = FIRST
    family = MONOMIAL
  [../]
[]

[Postprocessors]
  [./total_energy]
    type = ElementIntegralVariablePostprocessor
    variable = F_density
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[Kernels]
  # Stokes kernels
  [./StokesX]
    type = StokesX
    variable = u
    dim = 3
    phase_field = c
    pressure = p
    y_velocity = v
    z_velocity = w
  [../]
  [./StokesY]
    type = StokesY
    variable = v
    dim = 3
    phase_field = c
    pressure = p
    x_velocity = u
    z_velocity = w
  [../]
  [./StokesZ]
    type = StokesZ
    variable = w
    dim = 3
    phase_field = c
    pressure = p
    x_velocity = u
    y_velocity = v
  [../]
  [./Incompressibility]
    type = Incompressibility
    variable = p
    dim = 3
    x_velocity = u
    y_velocity = v
    z_velocity = w
  [../]

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
    z_velocity = w
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
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
  [./VelocityMagnitude]
    type = VelocityMagnitude
    variable = V_Magnitude
    dim = 3
    x_velocity = u
    y_velocity = v
    z_velocity = w
    execute_on = 'INITIAL TIMESTEP_END'
  [../]
[]

[BCs]
  [./bcs_u]
    type = DirichletBC
    variable = u
    boundary = '0 1 2 3'
    value = 0
  [../]
  [./bcs_v]
    type = DirichletBC
    variable = v
    boundary = '0 1 2 3'
    value = 0
  [../]
  [./bcs_w]
    type = DirichletBC
    variable = w
    boundary = '0 1 2 3'
    value = 0
  [../]
[]


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

  petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  petsc_options_value = 'lu 1500 superlu_dist'

  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6

  dt = 0.01 
  start_time = 0.0
  end_time = 5.0
[]


[Outputs]
  exodus = true
  time_step_interval = 1
  perf_graph = true
  checkpoint = true
  csv = true
  [./display]
    type = Console
    max_rows = 12
  [../]
[]

[Debug]
  show_material_props = true
[]