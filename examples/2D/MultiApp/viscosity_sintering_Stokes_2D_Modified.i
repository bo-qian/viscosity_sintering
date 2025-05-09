[Mesh]
[]

[Variables]
  [./u]
    order = SECOND
    family = LAGRANGE
  [../]
  [./v]
    order = SECOND
    family = LAGRANGE
  [../]
  [./p]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxVariables]
  [./c]
    order = FIRST
    family = LAGRANGE
  [../]
  [./mu]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[Kernels]
  # Stokes kernels
  [./StokesX]
    type = StokesXModified
    variable = u
    dim = 2
    phase_field = c
    chemical_potential = mu
    pressure = p
    y_velocity = v

  [../]
  [./StokesY]
    type = StokesYModified
    variable = v
    dim = 2
    phase_field = c
    chemical_potential = mu
    pressure = p
    x_velocity = u
  [../]
  [./Incompressibility]
    type = Incompressibility
    variable = p
    dim = 2
    x_velocity = u
    y_velocity = v
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
[]

[Materials]
  [./ViscosityMaterial]
    type = StokesMaterial
    cvar = c
    x_velocity = u
    y_velocity = v
    pressure = p
    # kappa_C = 33.75
    # mu_volume = 0.2
  [../]
[]

[Preconditioning]
  [./CH_Stokes]
    type = SMP
    full = true
    petsc_options_iname = '-pc_type -pc_factor_mat_solver_type -ksp_gmres_restart'
    petsc_options_value = 'lu mumps 2500'
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON

  l_tol = 1e-7
  l_abs_tol = 1e-10
  l_max_its = 100
  
  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6
[]

[Outputs]
  # perf_graph = true
  # execute_on = 'TIMESTEP_END'
[]

[Debug]
  show_var_residual_norms = true
[]