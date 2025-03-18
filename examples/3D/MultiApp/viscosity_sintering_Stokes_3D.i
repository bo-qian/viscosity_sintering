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
  [./w]
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
[]

[BCs]
  [./bcs_u]
    type = DirichletBC
    variable = u
    boundary = '0 1 2 3 4 5'
    value = 0
  [../]
  [./bcs_v]
    type = DirichletBC
    variable = v
    boundary = '0 1 2 3 4 5'
    value = 0
  [../]
  [./bcs_w]
    type = DirichletBC
    variable = w
    boundary = '0 1 2 3 4 5'
    value = 0
  [../]
[]

[Materials]
  [./ViscosityMaterial]
    type = StokesMaterial
    cvar = c
    x_velocity = u
    y_velocity = v
    z_velocity = w
    pressure = p
  [../]
[]

[Preconditioning]
  [./cw_coupling]
    type = SMP
    full = true
  [../]
[]

[Executioner]
  type = Steady
  solve_type = NEWTON

  petsc_options_iname = '-pc_type -ksp_gmres_restart -pc_factor_mat_solver_type'
  petsc_options_value = 'lu 2500 mumps'

  nl_rel_tol = 1e-15
  nl_abs_tol = 1e-6
[]

[Outputs]
[]